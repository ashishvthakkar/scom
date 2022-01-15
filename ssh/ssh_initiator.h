#include <glog/logging.h>

#include <array>
#include <functional>
#include <iostream>
#include <libssh/libsshpp.hpp>

// TODO(ashish): Convert to no except mode.
class SshInitiator {
private:
  static const int kBufferSize = 1024;
  static const int kReadTimeoutMs = 5'000;

public:
  using ChannelReadCallback = std::function<void(
      const char * /*buffer*/,
      int /*number of bytes read*/,
      ssh::Channel & /*channel*/)>;

  SshInitiator(
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

  ~SshInitiator() {
    // TODO(ashish): In case of an exception in the constructor after ssh_init,
    // this cleanup may not run and consequently there can be a resource leak.
    // This is to be addressed as part of the change for no_except.
    ssh_finalize();
  }

  SshInitiator(const SshInitiator &) = delete;
  SshInitiator &operator=(const SshInitiator &) = delete;
  SshInitiator(SshInitiator &&) = delete;
  SshInitiator &operator=(const SshInitiator &&) = delete;

  static void SshExecuteAtRemote(
      const std::string &remote_command,
      const ChannelReadCallback &read_callback,
      ssh::Session &session) {
    auto channel = ssh::Channel(session);
    channel.openSession();
    channel.requestExec(remote_command.c_str());

    std::string buffer;
    buffer.resize(kBufferSize);
    while (true) {
      auto bytes_read =
          channel.read(buffer.data(), buffer.size() - 1, kReadTimeoutMs);
      LOG(INFO) << "Read " << bytes_read << " bytes";
      if (bytes_read == 0) {
        break;
      }
      buffer.at(bytes_read) = '\0';
      LOG(INFO) << "Printing read data:";
      LOG(INFO) << buffer.data();
      read_callback(buffer.data(), bytes_read, channel);
    }
  }

private:
  static void SshConnect(
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

  static void SshAuthenticate(ssh::Session &session) {
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
};
