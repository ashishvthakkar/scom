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
    const std::string &remote_command,
    const bool public_key_auth)
    : public_key_auth_(public_key_auth),
      ssh_env_mgr_(),  // NOLINT(readability-redundant-member-init)
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
  if (public_key_auth_) {
    CHECK_EQ(SSH_AUTH_SUCCESS, ssh_session_.userauthPublickeyAuto());
  } else {
    auto password = std::array<char, kPasswordSize>();
    CHECK(!ssh_getpass(
        "password: ",
        password.data(),
        password.size(),
        false,
        false));
    CHECK_EQ(SSH_AUTH_SUCCESS, ssh_session_.userauthPassword(password.data()));
  }
}

void SshInitiator::SshExecuteAtRemote(const std::string &remote_command) {
  ssh_channel_ = std::make_unique<ssh::Channel>(ssh_session_);
  CHECK_EQ(SSH_OK, ssh_channel_->openSession());
  CHECK_EQ(SSH_OK, ssh_channel_->requestExec(remote_command.c_str()));
}

// Sends size followed by actual message
void SshInitiator::Send(const std::string &buffer) {
  int32_t message_size = buffer.size();
  LOG_ASSERT(sizeof(message_size) == kSizeOfMsgLen)
      << "Unexpected size mismatch";
  CHECK(buffer.size() <= std::numeric_limits<int32_t>::max())
      << "Message size of " << buffer.size() << " too large to be sent with "
      << sizeof(message_size) << "bytes";
  ssh_channel_->write(&message_size, sizeof(message_size));
  ssh_channel_->write(buffer.data(), buffer.size());
}

// Receives size followed by actual message
void SshInitiator::Receive(std::string &buffer) {
  int32_t message_size = 0;
  LOG_ASSERT(sizeof(message_size) == kSizeOfMsgLen)
      << "Unexpected size mismatch";
  auto bytes_read =
      ssh_channel_->read(&message_size, sizeof(message_size), kReadTimeoutMs);
  CHECK(bytes_read == sizeof(message_size)) << "Error reading size";
  buffer.resize(message_size);
  auto total_bytes_read = 0;
  while (total_bytes_read < message_size) {
    bytes_read = ssh_channel_->read(
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        buffer.data() + total_bytes_read,
        buffer.size() - total_bytes_read,
        kReadTimeoutMs);
    total_bytes_read += bytes_read;
    if (bytes_read == 0) {
      LOG(ERROR) << "Unexpected read of 0 bytes.";
      break;
    }
  }
  LOG_ASSERT(total_bytes_read <= buffer.size())
      << "Potential buffer overflow when reading from ssh channel";
  CHECK(total_bytes_read == message_size) << "Read incomplete message";
}

}  // namespace scom