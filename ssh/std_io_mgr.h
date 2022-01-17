#ifndef SCOM_STD_IO_MGR_H_
#define SCOM_STD_IO_MGR_H_

#include <string>

namespace scom {
class StdIoMgr {
public:
  StdIoMgr(std::ostream& log_file);
  void Send(const std::string& buffer);
  void Receive(std::string& buffer);

private:
  int Read(void* buffer, int buffer_size);
  int GetNextMessageSize();

  std::ostream& log_file_;
};

}  // namespace scom

#endif