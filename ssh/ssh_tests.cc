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

  std::vector<std::string> requests_{"5+2", "3*7"};  // NOLINT
  SshRequestor ssh_requestor_;                       // NOLINT
};

TEST_F(SshTests, BasicStringTxTest) {  // NOLINT
  const auto& response = ssh_requestor_.SendReceive(requests_[0]);
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
        const auto& response = ssh_requestor_.SendReceive(element.first);
        LOG(INFO) << "Got response: " << response;
        EXPECT_EQ(0, response.compare(std::to_string(element.second)));
      });
}

}  // namespace scom