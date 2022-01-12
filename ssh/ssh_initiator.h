#include <glog/logging.h>

#include <array>
#include <iostream>
#include <libssh/libsshpp.hpp>

// TODO(ashish): Convert to no except mode.
class SshInitiator {
public:
  SshInitiator(
      const std::string &host,
      const std::string &username,
      const std::string &remote_command) {
    ssh_init();
    auto session = ssh::Session();
    SshConnect(host, username, session);
    SshAuthenticate(session);
    SshExecuteAtRemote(remote_command, session);
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
      ssh::Session &session) {
    auto channel = ssh::Channel(session);
    channel.openSession();
    channel.requestExec(remote_command.c_str());

    const int buffer_size = 1024;
    auto buffer = std::array<char, buffer_size>();

    while (true) {
      auto nbytes = channel.read(buffer.data(), buffer.size() - 1, -1);
      LOG(INFO) << "Read " << nbytes << " bytes";
      if (nbytes == 0) {
        break;
      }
      buffer.at(nbytes) = '\0';
      LOG(INFO) << "Printing read data:";
      LOG(INFO) << buffer.data();
      std::string addition_input("additional_input\n");
      channel.write(addition_input.data(), addition_input.size());
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
