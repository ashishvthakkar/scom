#ifndef SCOM_TEMP_PATH_H_
#define SCOM_TEMP_PATH_H_

#include <filesystem>

namespace scom {

class TempPath final {
public:
  TempPath();
  ~TempPath();
  [[nodiscard]] const std::filesystem::path& Path() const { return path_; }

  TempPath(const TempPath&) = delete;
  TempPath& operator=(const TempPath&) = delete;
  TempPath(TempPath&&) = delete;
  TempPath& operator=(const TempPath&&) = delete;

private:
  std::filesystem::path path_;
};

}  // namespace scom

#endif
