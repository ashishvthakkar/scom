#include <config.h>
#include <gflags/gflags.h>

#include "../protocol/protocol_adapter.h"
#include "ssh_initiator.h"

class SshRequestor {
public:
  explicit SshRequestor(
      const std::string& host,
      const std::string& user,
      const std::string& command);

  std::string Send(const std::string& request);

  // Note: Targets
  // 1.
  // const std::vector<char>& Request(const std::vector<char>& request);
  // stdin --- read and write (1 and 2 as file descriptor)

  // 2. Perf
  // Timestamps back and forth. Tx per second.
  // ~1m files in <10 sec> --> 100k calls per sec

  // 3. Interface
  // template<typename T, typename U>
  // const U& Request(const T &request);

  // std::vector<std::string> Request(const std::vector<std::string>
  // &request);

private:
  static const int kBufferSize = 1024;

  SshInitiator ssh_initator_;
  int request_id_;
};
