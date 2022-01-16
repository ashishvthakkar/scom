#include <config.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "ssh_requestor.h"

class SshTests : public ::testing::Test {
public:
  SshTests() : ssh_requestor_(kSshTestHost, kSshTestUser, kResponderCommand) {}

  std::vector<std::string> requests_{"Request1", "Request2"};  // NOLINT
  SshRequestor ssh_requestor_;                                 // NOLINT
};

TEST_F(SshTests, BasicConnectionTest) {  // NOLINT
  const auto& response = ssh_requestor_.Send(requests_[0]);
  LOG(INFO) << "Got response: " << response;
  EXPECT_LT(0, response.size());
}
