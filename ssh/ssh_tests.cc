#include <config.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include <chrono>
#include <string>
#include <vector>

#include "ssh_requestor.h"

namespace scom {
class SshTests : public ::testing::Test {
public:
  SshTests() : ssh_requestor_(kSshTestHost, kSshTestUser, kResponderCommand) {}

  std::vector<std::string> requests_{"5+2", "3*7"};  // NOLINT
  SshRequestor ssh_requestor_;                       // NOLINT
};

TEST_F(SshTests, BasicStringTxTest) {  // NOLINT
  ssh_requestor_.Send(requests_[0]);
  const auto& response = ssh_requestor_.Receive();
  LOG(INFO) << "Got response: " << response;
  EXPECT_LT(0, response.size());
}

TEST_F(SshTests, SimpleExpressionEval) {  // NOLINT
  std::map<std::string, int> expression_to_value = {
      {"5+2", 7},       // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      {"5+3*5+1", 21},  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      {"5+3*5-1", 19}   // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  };
  std::for_each(
      expression_to_value.begin(),
      expression_to_value.end(),
      [this](const std::pair<std::string, int>& element) {
        ssh_requestor_.Send(element.first);
        const auto& response = ssh_requestor_.Receive();
        LOG(INFO) << "Got response: " << response;
        EXPECT_EQ(0, response.compare(std::to_string(element.second)));
      });
}

TEST_F(SshTests, BandwidthPerfTest) {  // NOLINT
  SshRequestor ssh_echo_requestor(
      kSshTestHost,
      kSshTestUser,
      kEchoResponderCommand);
  const int64_t num_requests = 5000;
  const int64_t request_size = 100000;
  const int max_seconds = 10;
  const int min_bandwidth_mbps = 150;

  namespace chrono = std::chrono;
  std::string message(request_size, '1');
  auto start = chrono::high_resolution_clock::now();
  for (auto i = 0; i < num_requests; i++) {
    ssh_echo_requestor.Send(message);
    ssh_echo_requestor.Receive();
  }
  auto end = chrono::high_resolution_clock::now();

  const auto to_mega = 1024 * 1024;
  auto seconds = chrono::duration_cast<chrono::seconds>(end - start).count();
  auto size = (request_size * num_requests) / to_mega;
  auto bandwidth = size / seconds;
  LOG(INFO) << "Completing total requests with size " << size
            << " MB and received responses with the same total size in "
            << seconds << " seconds for a bandwidth of " << bandwidth
            << "MB/s.";

  EXPECT_LE(seconds, max_seconds);
  EXPECT_GE(bandwidth, min_bandwidth_mbps);
}

}  // namespace scom