#include "ssh_initiator.h"

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
  // CHECK(session.isServerKnown());
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
  ssh_channel_->openSession();
  ssh_channel_->requestExec(remote_command.c_str());
}

void SshInitiator::Send(const std::string &request, std::string &response) {
  ssh_channel_->write(request.data(), request.size());
  auto bytes_read =
      ssh_channel_->read(response.data(), response.size(), kReadTimeoutMs);
  CHECK(bytes_read <= response.size())
      << "Potential buffer overflow when reading from ssh channel";
  response.resize(bytes_read);
}

// void SshInitiator::SshRequest(const std::vector<char> request) {
//   std::string buffer;
//   buffer.resize(kBufferSize);
//   auto bytes_read = 0;
//   while (true) {
//     // auto done = read_callback(buffer.data(), bytes_read, channel);
//     // if (done) {
//     //   break;
//     // }
//     // TODO(ashish): Evaluate whether the next two lines can be removed.
//     buffer.clear();
//     buffer.resize(kBufferSize);
//     bytes_read =
//         ssh_channel_->read(buffer.data(), buffer.size() - 1, kReadTimeoutMs);
//     if (bytes_read == 0) {
//       break;
//     }
//   }
// }
