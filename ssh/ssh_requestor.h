#include <config.h>
#include <gflags/gflags.h>

#include <vector>

#include "ssh_initiator.h"

namespace scom {

class SshRequestor {
public:
  explicit SshRequestor(
      const std::string& host,
      const std::string& user,
      const std::string& command);

  // NOTE: An optimization is to keep the same buffer internally and only
  // provide a const reference to the response to the caller. Depending on the
  // use case, that is likely to be a reasonable interface while providing
  // performance benefits.
  void Send(const std::string& message);
  std::string Receive();
  std::string SendReceive(const std::string& message);

private:
  SshInitiator ssh_initator_;
  int request_id_;
  std::string buffer_;
};

}  // namespace scom