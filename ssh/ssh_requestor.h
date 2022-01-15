#include <config.h>
#include <gflags/gflags.h>

#include "../protocol/protocol_adapter.h"
#include "ssh_initiator.h"

class SshRequestor {
public:
  explicit SshRequestor(
      const std::vector<std::string>& requests,
      const std::string& host,
      const std::string& user,
      const std::string& command);

private:
  bool
  SshResponseHandler(const char* buffer, int bytes_read, ssh::Channel& channel);

  const std::vector<std::string>& requests_;
  int next_request_id_;
};
