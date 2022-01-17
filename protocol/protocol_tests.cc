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
  HeaderWriteFields header{
      .version = version,
      .request_id = request_id,
      .payload = details};
  CHECK(scom::WriteMessage(header, out))
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
  HeaderReadFields header;
  scom::ReadMessage(test_file_in, header);
  EXPECT_EQ(header.version, kProtocolVersion);
  EXPECT_EQ(1, header.request_id);
  LOG(INFO) << "Read details: " << details;
  EXPECT_EQ(0, header.payload.compare("Test message"));
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
  HeaderReadFields header;
  scom::ReadMessage(test_file_in, header);
  EXPECT_EQ(expected_version, header.version);
  EXPECT_EQ(expected_request_id, header.request_id);
  LOG(INFO) << "Read details: " << header.payload;
  EXPECT_EQ(0, header.payload.compare(expected_details));
}

TEST_F(ProtocolTests, BasicStringWriteAndReadTest) {  // NOLINT
  const int expected_version = 5;
  const int expected_request_id = 9;
  const std::string expected_details = "Sample message";
  std::string str;
  HeaderWriteFields header{
      .version = expected_version,
      .request_id = expected_request_id,
      .payload = expected_details};
  scom::WriteMessage(header, str);
  HeaderReadFields header_read;
  scom::ReadMessage(str, header_read);
  EXPECT_EQ(expected_version, header_read.version);
  EXPECT_EQ(expected_request_id, header_read.request_id);
  LOG(INFO) << "Read version: " << header_read.version;
  LOG(INFO) << "Read details: " << header_read.payload;
  EXPECT_EQ(0, header_read.payload.compare(expected_details));
}

}  // namespace scom