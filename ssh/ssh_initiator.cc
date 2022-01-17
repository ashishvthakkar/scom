#include "ssh_initiator.h"

#include <config.h>
#include <glog/logging.h>

namespace scom {

SshInitiator::SshEnvMgr::SshEnvMgr() {
  CHECK_EQ(SSH_OK, ssh_init()) << "Could not initiatlize ssh lib";
}

SshInitiator::SshEnvMgr::~SshEnvMgr() {
  CHECK_EQ(0, ssh_finalize()) << "Could not clean up ssh lib";
}

SshInitiator::SshInitiator(
    const std::string &host,
    const std::string &username,
    const std::string &remote_command)
    : ssh_env_mgr_(),  // NOLINT(readability-redundant-member-init)
      ssh_session_(    // NOLINT(readability-redundant-member-init)
          ssh::Session()) {
  SshConnect(host, username);
  SshAuthenticate();
  SshExecuteAtRemote(remote_command);
}

void SshInitiator::SshConnect(
    const std::string &host,
    const std::string &username) {
  CHECK_EQ(
      SSH_OK,
      ssh_session_.setOption(ssh_options_e::SSH_OPTIONS_HOST, host.c_str()));
  CHECK_EQ(
      SSH_OK,
      ssh_session_.setOption(
          ssh_options_e::SSH_OPTIONS_USER,
          username.c_str()));

  int verbosity = kSshVerbosity;
  CHECK_EQ(
      SSH_OK,
      ssh_session_.setOption(SSH_OPTIONS_LOG_VERBOSITY, &verbosity));
  int port = kSshPort;

  CHECK_EQ(
      SSH_OK,
      ssh_session_.setOption(
          SSH_OPTIONS_PORT,
          reinterpret_cast<  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
              void *>(&port)));

  CHECK_EQ(SSH_OK, ssh_session_.connect());
  CHECK(ssh_session_.isServerKnown());
}

void SshInitiator::SshAuthenticate() {
  auto password = std::array<char, kPasswordSize>();
  CHECK(!ssh_getpass(
      "password: ",
      password.data(),
      password.size(),
      false,
      false));
  CHECK_EQ(SSH_AUTH_SUCCESS, ssh_session_.userauthPassword(password.data()));
}

void SshInitiator::SshExecuteAtRemote(const std::string &remote_command) {
  ssh_channel_ = std::make_unique<ssh::Channel>(ssh_session_);
  CHECK_EQ(SSH_OK, ssh_channel_->openSession());
  CHECK_EQ(SSH_OK, ssh_channel_->requestExec(remote_command.c_str()));
}

// Sends size followed by actual message
void SshInitiator::Send(const std::string &buffer) {
  int message_size = buffer.size();
  LOG_ASSERT(sizeof(message_size) == kSizeOfMsgLen)
      << "Unexpected size mismatch";
  ssh_channel_->write(&message_size, sizeof(message_size));
  ssh_channel_->write(buffer.data(), buffer.size());
}

// Receives size followed by actual message
void SshInitiator::Receive(std::string &buffer) {
  int message_size = 0;
  LOG_ASSERT(sizeof(message_size) == kSizeOfMsgLen)
      << "Unexpected size mismatch";
  auto bytes_read =
      ssh_channel_->read(&message_size, sizeof(message_size), kReadTimeoutMs);
  LOG(INFO) << "Expecting next message of size: " << message_size;
  CHECK(bytes_read == sizeof(message_size)) << "Error reading size";
  buffer.resize(message_size);
  bytes_read = ssh_channel_->read(buffer.data(), buffer.size(), kReadTimeoutMs);
  LOG_ASSERT(bytes_read <= buffer.size())
      << "Potential buffer overflow when reading from ssh channel";
  CHECK(bytes_read == message_size) << "Read incomplete message";
  buffer.resize(bytes_read);
}

}  // namespace scom