#ifndef SCOM_SSH_INITIATOR_H_
#define SCOM_SSH_INITIATOR_H_

#include <glog/logging.h>

#include <array>
#include <functional>
#include <iostream>
#include <libssh/libsshpp.hpp>
#include <memory>

class SshInitiator {
public:
  SshInitiator(
      const std::string &host,
      const std::string &username,
      const std::string &remote_command);
  void Send(const std::string &buffer);
  void Receive(std::string &buffer);

private:
  // NOTE: The below class exists only to call init and finalize.
  // Depending on the libssh version used, it may be possible to remove
  // this.
  class SshEnvMgr {
  public:
    SshEnvMgr();
    ~SshEnvMgr();
  };

  void SshConnect(const std::string &host, const std::string &username);
  void SshAuthenticate();
  void SshExecuteAtRemote(const std::string &remote_command);

  static const int kReadTimeoutMs = 5'000;  // milliseconds, -1 for indefinite
  static const int kSshVerbosity = SSH_LOG_PROTOCOL;
  static const int kSshPort = 22;
  static const int kPasswordSize = 1024;  // TODO(ashish): update

  SshEnvMgr ssh_env_mgr_;
  ssh::Session ssh_session_;
  // NOTE: We store a unique ptr to ssh::Channel as ssh:Channel cannot be moved
  std::unique_ptr<ssh::Channel> ssh_channel_;
};

#endif