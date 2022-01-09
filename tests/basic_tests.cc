#include <glog/logging.h>
#include <gtest/gtest.h>

TEST(BasicTest, SampleTest) {  // NOLINT
  EXPECT_NE(5 * 6, 20);
  EXPECT_EQ(5 * 6, 30);
}
