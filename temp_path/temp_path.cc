#include "temp_path.h"

#include <glog/logging.h>

#include <atomic>

namespace scom {

static std::filesystem::path GetUniqueTempPath() {
  static std::atomic<uint32_t> counter{0};
  auto ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();

  return std::filesystem::temp_directory_path() /
         ("tmp_" + std::to_string(ts) + "_" + std::to_string(counter++));
}

TempPath::TempPath() : path_(GetUniqueTempPath()) {
  CHECK(!std::filesystem::exists(path_)) << path_ << "already exists";
  std::filesystem::create_directories(path_);
  LOG(INFO) << "Using temporary path: " << path_;
}

TempPath::~TempPath() {
  std::error_code ec{};
  std::filesystem::remove_all(path_, ec);
  LOG_IF(ERROR, ec) << " " << ec.message();
}

}  // namespace scom