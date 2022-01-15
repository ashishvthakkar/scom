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

static void WriteToFile(
    const int version,
    const int request_id,
    const std::string &details,
    const std::filesystem::path &path) {
  std::fstream out(path, std::ios::out | std::ios::trunc | std::ios::binary);
  CHECK(scom::WriteMessage(version, request_id, details, out))
      << "Could not write message to test file";
  out.flush();
}

TEST_F(ProtocolTests, BasicReadTest) {  // NOLINT
  LOG(INFO) << "Using test dir: " << kTestDataDir;
  std::fstream test_file_in(
      kTestDataDir / "protobuf_test_data",
      std::ios::in | std::ios::binary);
  int version = 0;
  int request_id = 0;
  std::string details;
  // Note: Use the below to update test file in case of a conscious change.
  // std::string write_details("Test message");
  // WriteToFile(
  //     kProtocolVersion,
  //     1,
  //     write_details,
  //     kTestDataDir / "protobuf_test_data");
  scom::ReadMessage(test_file_in, version, request_id, details);
  EXPECT_EQ(version, kProtocolVersion);
  EXPECT_EQ(1, request_id);
  LOG(INFO) << "Read details: " << details;
  EXPECT_EQ(0, details.compare("Test message"));
}

TEST_F(ProtocolTests, BasicWriteAndReadTest) {  // NOLINT
  scom::TempPath temp;
  LOG(INFO) << "Using temp path: " << temp.Path();
  std::filesystem::path test_file_path = temp.Path() / "test_data_file";
  const int expected_version = 4;
  const int expected_request_id = 9;
  const std::string expected_details = "Sample message";
  WriteToFile(
      expected_version,
      expected_request_id,
      expected_details,
      test_file_path);
  int version = 0;
  int request_id = 0;
  std::string details;
  std::fstream test_file_in(test_file_path, std::ios::in | std::ios::binary);
  scom::ReadMessage(test_file_in, version, request_id, details);
  EXPECT_EQ(expected_version, version);
  EXPECT_EQ(expected_request_id, request_id);
  LOG(INFO) << "Read details: " << details;
  EXPECT_EQ(0, details.compare(expected_details));
}

TEST_F(ProtocolTests, BasicStringWriteAndReadTest) {  // NOLINT
  const int expected_version = 5;
  const int expected_request_id = 9;
  const std::string expected_details = "Sample message";
  std::string str;
  scom::WriteMessage(
      expected_version,
      expected_request_id,
      expected_details,
      str);
  int version = 0;
  int request_id = 0;
  std::string details;
  scom::ReadMessage(str, version, request_id, details);
  EXPECT_EQ(expected_version, version);
  EXPECT_EQ(expected_request_id, request_id);
  LOG(INFO) << "Read version: " << version;
  LOG(INFO) << "Read details: " << details;
  EXPECT_EQ(0, details.compare(expected_details));
}

}  // namespace scom