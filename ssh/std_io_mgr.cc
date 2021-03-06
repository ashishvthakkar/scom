#include "std_io_mgr.h"

#include <config.h>
#include <glog/logging.h>

#include <string>

namespace scom {

StdIoMgr::StdIoMgr(std::ostream& log_file, bool enable_logging)
    : log_file_(log_file),
      enable_logging_(enable_logging) {
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  in = std::freopen(nullptr, "rb", stdin);
  CHECK(in != nullptr && !std::ferror(in))
      << "Could not reopen input resource in binary mode";
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  out = std::freopen(nullptr, "wb", stdout);
  CHECK(out != nullptr && !std::ferror(out))
      << "Could not reopen output resource in binary mode";
}

void StdIoMgr::Receive(std::string& buffer) {
  int32_t message_size = GetNextMessageSize();
  buffer.resize(message_size);
  auto bytes_read = Read(buffer.data(), buffer.size());
  CHECK(bytes_read == message_size) << "Read incomplete message";
}

int32_t StdIoMgr::GetNextMessageSize() {
  int32_t message_size = 0;
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
  auto bytes_read = std::fread(buffer, 1, buffer_size, in);
  CHECK(!std::ferror(in)) << "Error reading from input";
  if (enable_logging_) {
    log_file_ << "Read message with size: " << bytes_read << std::endl;
  }
  LOG_ASSERT(bytes_read <= buffer_size)
      << "Potential buffer overflow when reading";
  return bytes_read;
}

void StdIoMgr::Send(const std::string& buffer) {
  if (buffer.empty()) {
    return;
  }
  int32_t size = buffer.size();
  CHECK(buffer.size() <= std::numeric_limits<int32_t>::max())
      << "Message size of " << buffer.size() << " too large to be sent with "
      << sizeof(size) << "bytes";
  std::fwrite(&size, sizeof(size), 1, out);
  std::fwrite(buffer.data(), sizeof(buffer[0]), buffer.size(), out);
  std::fflush(out);
  if (enable_logging_) {
    log_file_ << "Sent " << buffer.size() << " bytes." << std::endl;
  }
}

}  // namespace scom
