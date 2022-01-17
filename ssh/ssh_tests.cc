#include <config.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "ssh_requestor.h"

namespace scom {
class SshTests : public ::testing::Test {
public:
  SshTests() : ssh_requestor_(kSshTestHost, kSshTestUser, kResponderCommand) {}

  std::vector<std::string> requests_{"Request1", "Request2"};  // NOLINT
  SshRequestor ssh_requestor_;                                 // NOLINT
};

TEST_F(SshTests, BasicStringTxTest) {  // NOLINT
  const auto& response = ssh_requestor_.SendReceive(requests_[0]);
  LOG(INFO) << "Got response: " << response;
  EXPECT_LT(0, response.size());
}

TEST_F(SshTests, BasicVectorTxTest) {  // NOLINT
  std::vector<char> request{'a', 'b', 'c'};
  const auto& response = ssh_requestor_.SendReceive(request);
  LOG(INFO) << "Got response with size: " << response.size();
  for (const auto& c : response) {
    LOG(INFO) << c;
  }
  EXPECT_LT(0, response.size());
}

}  // namespace scom