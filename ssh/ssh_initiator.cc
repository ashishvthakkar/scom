#include "ssh_initiator.h"

SshInitiator::SshInitiator(
    const std::string &host,
    const std::string &username,
    const std::string &remote_command,
    const ChannelReadCallback &read_callback) {
  ssh_init();
  auto session = ssh::Session();
  SshConnect(host, username, session);
  SshAuthenticate(session);
  SshExecuteAtRemote(remote_command, read_callback, session);
}

SshInitiator::~SshInitiator() {
  // TODO(ashish): In case of an exception in the constructor after ssh_init,
  // this cleanup may not run and consequently there can be a resource leak.
  // This is to be addressed as part of the change for no_except.
  ssh_finalize();
}

void SshInitiator::SshExecuteAtRemote(
    const std::string &remote_command,
    const ChannelReadCallback &read_callback,
    ssh::Session &session) {
  auto channel = ssh::Channel(session);
  channel.openSession();
  channel.requestExec(remote_command.c_str());

  std::string buffer;
  buffer.resize(kBufferSize);
  auto bytes_read = 0;
  while (true) {
    auto done = read_callback(buffer.data(), bytes_read, channel);
    if (done) {
      break;
    }
    // TODO(ashish): Evaluate whether the next two lines can be removed.
    buffer.clear();
    buffer.resize(kBufferSize);
    bytes_read = channel.read(buffer.data(), buffer.size() - 1, kReadTimeoutMs);
    if (bytes_read == 0) {
      break;
    }
  }
}

void SshInitiator::SshConnect(
    const std::string &host,
    const std::string &username,
    ssh::Session &session) {
  int verbosity = SSH_LOG_PROTOCOL;
  int port = 22;  // NOLINT(cppcoreguidelines-avoid-magic-numbers)

  session.setOption(ssh_options_e::SSH_OPTIONS_HOST, host.c_str());
  session.setOption(ssh_options_e::SSH_OPTIONS_USER, username.c_str());
  session.setOption(SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  session.setOption(SSH_OPTIONS_PORT, reinterpret_cast<void *>(&port));

  session.connect();
  // CHECK(session.isServerKnown());
}

void SshInitiator::SshAuthenticate(ssh::Session &session) {
  const int password_size = 32;
  auto password = std::array<char, password_size>();
  CHECK(!ssh_getpass(
      "password: ",
      password.data(),
      password.size(),
      false,
      false));
  CHECK_EQ(SSH_AUTH_SUCCESS, session.userauthPassword(password.data()));
}
