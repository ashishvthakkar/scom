#include <config.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

#include "../temp_path/temp_path.h"
#include "protocol_adapter.h"

namespace scom {

class ProtocolTests : public ::testing::Test {
public:
  ProtocolTests() { scom::InitProtobuf(); }

  // Note: Explicit shutdown is not used and instead the default behavior on
  // exit is applied.
  //  ~ProtocolTests() { scom::ShutdownProtobufLibrary(); }
};

TEST_F(ProtocolTests, BasicReadTest) {  // NOLINT
  LOG(INFO) << "Using test dir: " << kTestDataDir;
  std::fstream test_file_in(
      kTestDataDir / "protobuf_test_data",
      std::ios::in | std::ios::binary);
  unsigned int version = 1;
  std::string details;
  scom::ReadMessage(&version, &details, &test_file_in);
  EXPECT_EQ(version, kProtocolVersion);
  LOG(INFO) << "Read details: " << details;
  EXPECT_EQ(0, details.compare("Test message"));
}

void WriteToFile(
    const unsigned int version,
    const std::string &details,
    const std::filesystem::path &path) {
  std::fstream out(path, std::ios::out | std::ios::trunc | std::ios::binary);
  CHECK(scom::WriteMessage(version, details, &out))
      << "Could not write message to test file";
  out.flush();
}

TEST_F(ProtocolTests, BasicWriteAndReadTest) {  // NOLINT
  scom::TempPath temp;
  LOG(INFO) << "Using temp path: " << temp.Path();
  std::filesystem::path test_file_path = temp.Path() / "test_data_file";
  const unsigned int expected_version = 4;
  const std::string expected_details = "Sample message";
  WriteToFile(expected_version, expected_details, test_file_path);
  unsigned int version = 1;
  std::string details;
  std::fstream test_file_in(test_file_path, std::ios::in | std::ios::binary);
  scom::ReadMessage(&version, &details, &test_file_in);
  EXPECT_EQ(expected_version, version);
  LOG(INFO) << "Read details: " << details;
  EXPECT_EQ(0, details.compare(expected_details));
}

}  // namespace scom