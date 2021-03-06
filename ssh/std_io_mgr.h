#ifndef SCOM_STD_IO_MGR_H_
#define SCOM_STD_IO_MGR_H_

#include <string>

namespace scom {
class StdIoMgr {
public:
  StdIoMgr(std::ostream& log_file, bool enable_logging);
  void Send(const std::string& buffer);
  void Receive(std::string& buffer);

private:
  int Read(void* buffer, int buffer_size);
  int32_t GetNextMessageSize();

  // Note: Logging can be controlled by a config parameter for performance.
  std::ostream& log_file_;
  bool enable_logging_;
  FILE* in;
  FILE* out;
};

}  // namespace scom

#endif