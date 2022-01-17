#include "std_io_mgr.h"

#include <config.h>
#include <glog/logging.h>

#include <string>

namespace scom {

StdIoMgr::StdIoMgr(std::ostream& log_file) : log_file_(log_file) {
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  std::freopen(nullptr, "rb", stdin);
  CHECK(!std::ferror(stdin)) << "Could not reopen stdin in binary mode";
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  std::freopen(nullptr, "wb", stdout);
  CHECK(!std::ferror(stdin)) << "Could not reopen stdout in binary mode";
}

void StdIoMgr::Receive(std::string& buffer) {
  int message_size = GetNextMessageSize();
  buffer.resize(message_size);
  auto bytes_read = Read(buffer.data(), buffer.size());
  CHECK(bytes_read == message_size) << "Read incomplete message";
}

int StdIoMgr::GetNextMessageSize() {
  int message_size = 0;
  LOG_ASSERT(sizeof(message_size) == kSizeOfMsgLen)
      << "Unexpected size mismatch";
  auto bytes_read = Read(&message_size, sizeof(message_size));
  CHECK(bytes_read == sizeof(message_size)) << "Error reading size";
  return message_size;
}

int StdIoMgr::Read(void* buffer, int buffer_size) {
  if (buffer_size <= 0) {
    return 0;
  }
  auto bytes_read = std::fread(buffer, 1, buffer_size, stdin);
  CHECK(!std::ferror(stdin)) << "Error reading from stdin";
  log_file_ << "Read message with size: " << bytes_read << std::endl;
  LOG_ASSERT(bytes_read <= buffer_size)
      << "Potential buffer overflow when reading";
  return bytes_read;
}

void StdIoMgr::Send(const std::string& buffer) {
  if (buffer.empty()) {
    return;
  }
  int size = buffer.size();
  std::fwrite(&size, sizeof(size), 1, stdout);
  std::fwrite(buffer.data(), sizeof(buffer[0]), buffer.size(), stdout);
  std::fflush(stdout);
  log_file_ << "Sent " << buffer.size() << " bytes." << std::endl;
}

}  // namespace scom
