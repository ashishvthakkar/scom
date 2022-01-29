#include <config.h>
#include <gflags/gflags.h>

#include <vector>

#include "msg_io_mgr.h"
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

private:
  SshInitiator ssh_initator_;
  MsgIoMgr<SshInitiator> msg_io_mgr_;
  std::string buffer_;
};

}  // namespace scom