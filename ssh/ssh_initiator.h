#ifndef SCOM_SSH_INITIATOR_H_
#define SCOM_SSH_INITIATOR_H_

#include <glog/logging.h>

#include <array>
#include <functional>
#include <iostream>
// #define SSH_NO_CPP_EXCEPTIONS 1
#include <libssh/libsshpp.hpp>

// TODO(ashish): Convert to no except mode.
class SshInitiator {
public:
  using ChannelReadCallback = std::function<bool(
      const char * /*buffer*/,
      int /*number of bytes read*/,
      ssh::Channel & /*channel*/)>;

  SshInitiator(
      const std::string &host,
      const std::string &username,
      const std::string &remote_command,
      const ChannelReadCallback &read_callback);
  ~SshInitiator();

  SshInitiator(const SshInitiator &) = delete;
  SshInitiator &operator=(const SshInitiator &) = delete;
  SshInitiator(SshInitiator &&) = delete;
  SshInitiator &operator=(const SshInitiator &&) = delete;

  static void SshExecuteAtRemote(
      const std::string &remote_command,
      const ChannelReadCallback &read_callback,
      ssh::Session &session);

private:
  static void SshConnect(
      const std::string &host,
      const std::string &username,
      ssh::Session &session);

  static void SshAuthenticate(ssh::Session &session);

  static const int kBufferSize = 1024;
  static const int kReadTimeoutMs = -1;  // 5'000;
};

#endif