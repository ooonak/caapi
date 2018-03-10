#include "cautils.hpp"
#include "gtest/gtest.h"

/*
TEST(CaUtilsTest, squareTest) {
  CaUtils u;
  EXPECT_EQ(42, u.square(6, 7));
}
*/

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
