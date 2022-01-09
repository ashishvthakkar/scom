#include <glog/logging.h>

#include <array>
#include <iostream>
#include <libssh/libsshpp.hpp>

int main() {
  try {
    ssh_init();

    auto session = ssh::Session();

    int verbosity = SSH_LOG_PROTOCOL;
    int port = 22;  // NOLINT(cppcoreguidelines-avoid-magic-numbers)

    session.setOption(ssh_options_e::SSH_OPTIONS_HOST, "ash-alienw-r11");
    session.setOption(ssh_options_e::SSH_OPTIONS_USER, "ashish");
    session.setOption(SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    // session.setOption(SSH_OPTIONS_PORT, &port);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    session.setOption(SSH_OPTIONS_PORT, reinterpret_cast<void *>(&port));

    session.connect();
    // CHECK(session.isServerKnown());

    const int password_size = 32;
    auto password = std::array<char, password_size>();
    CHECK(!ssh_getpass(
        "password: ",
        password.data(),
        password.size(),
        false,
        false));
    CHECK_EQ(SSH_AUTH_SUCCESS, session.userauthPassword(password.data()));

    auto channel = ssh::Channel(session);
    channel.openSession();
    channel.requestExec("ps aux");

    const int buffer_size = 1024;
    auto buffer = std::array<char, buffer_size>();

    while (true) {
      auto nbytes = channel.read(buffer.data(), buffer.size() - 1, -1);
      if (nbytes == 0) {
        break;
      }
      buffer.at(nbytes) = '\0';
      std::cout << buffer.data();
    }
  } catch (...) {
    ssh_finalize();
  }
}