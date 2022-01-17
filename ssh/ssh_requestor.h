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
  std::string ReceiveAsStr();
  std::string SendReceive(const std::string& message);

  // As protobuf internally uses std::string instead of std::vector<char>, the
  // std::string version of this function is more performant than this one for
  // both text and binary data
  void Send(const std::vector<char>& message);
  std::vector<char> ReceiveAsBuffer();
  std::vector<char> SendReceive(const std::vector<char>& message);

  // Note: Targets
  // 2. Perf
  // Timestamps back and forth. Tx per second.
  // ~1m files in <10 sec> --> 100k calls per sec
  // 3. Interface
  // template<typename T, typename U>
  // const U& Request(const T &request);

private:
  SshInitiator ssh_initator_;
  int request_id_;
  std::string buffer_;
};

}  // namespace scom