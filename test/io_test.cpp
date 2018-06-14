#include <gtest/gtest.h>
#include "system_call_mock.hpp"
#include "util/io.h"

using namespace util;
using namespace ::testing;

struct IOTest : Test, SystemCallMockBase {};

TEST_F(IOTest, Pread) {
    auto ret = UninterruptedPread(0, nullptr, 0, 0);
    ASSERT_EQ(0, ret);

    EXPECT_CALL(scm, pread(0, nullptr, 100, 0)).WillOnce(Return(0));
    ret = UninterruptedPread(0, nullptr, 100, 0);
    ASSERT_EQ(0, ret);

    EXPECT_CALL(scm, pread(0, nullptr, 100, 0))
        .WillOnce(SetErrnoAndReturn(EINTR, -1))
        .WillOnce(SetErrnoAndReturn(EAGAIN, -1));
    ret = UninterruptedPread(0, nullptr, 100, 0);
    ASSERT_EQ(-1, ret);

    EXPECT_CALL(scm, pread(0, nullptr, 100, 0)).WillOnce(Return(40));
    EXPECT_CALL(scm, pread(0, nullptr, 60, 40)).WillOnce(Return(30));
    EXPECT_CALL(scm, pread(0, nullptr, 30, 70)).WillOnce(Return(20));
    EXPECT_CALL(scm, pread(0, nullptr, 10, 90)).WillOnce(Return(10));
    ret = UninterruptedPread(0, nullptr, 100, 0);
    ASSERT_EQ(100, ret);
}

TEST_F(IOTest, Pwrite) {
    auto ret = UninterruptedPwrite(0, nullptr, 0, 0);
    ASSERT_EQ(0, ret);

    EXPECT_CALL(scm, pwrite(0, nullptr, 100, 0)).WillOnce(Return(0));
    ret = UninterruptedPwrite(0, nullptr, 100, 0);
    ASSERT_EQ(0, ret);

    EXPECT_CALL(scm, pwrite(0, nullptr, 100, 0))
        .WillOnce(SetErrnoAndReturn(EINTR, -1))
        .WillOnce(SetErrnoAndReturn(EAGAIN, -1));
    ret = UninterruptedPwrite(0, nullptr, 100, 0);
    ASSERT_EQ(-1, ret);

    EXPECT_CALL(scm, pwrite(0, nullptr, 100, 0)).WillOnce(Return(40));
    EXPECT_CALL(scm, pwrite(0, nullptr, 60, 40)).WillOnce(Return(30));
    EXPECT_CALL(scm, pwrite(0, nullptr, 30, 70)).WillOnce(Return(20));
    EXPECT_CALL(scm, pwrite(0, nullptr, 10, 90)).WillOnce(Return(10));
    ret = UninterruptedPwrite(0, nullptr, 100, 0);
    ASSERT_EQ(100, ret);
}

TEST_F(IOTest, Preadv) {
    EXPECT_CALL(scm, preadv(0, nullptr, 0, 0)).WillOnce(Return(0));
    auto ret = UninterruptedPreadv(0, nullptr, 0, 0);
    ASSERT_EQ(0, ret);

    EXPECT_CALL(scm, preadv(0, nullptr, 100, 0))
        .WillOnce(SetErrnoAndReturn(EINTR, -1))
        .WillOnce(SetErrnoAndReturn(EAGAIN, -1));
    ret = UninterruptedPreadv(0, nullptr, 100, 0);
    ASSERT_EQ(-1, ret);

    iovec iov[10];
    for (int i = 0; i < 10; i++){
        iov[i].iov_base = nullptr;
        iov[i].iov_len = 10;
    }
    EXPECT_CALL(scm, preadv(0, iov, 10, 0)).WillOnce(Return(35));
    EXPECT_CALL(scm, preadv(0, iov + 3, 7, 35)).WillOnce(Return(30));
    EXPECT_CALL(scm, preadv(0, iov + 6, 4, 65)).WillOnce(Return(25));
    EXPECT_CALL(scm, preadv(0, iov + 9, 1, 90)).WillOnce(Return(10));
    ret = UninterruptedPreadv(0, iov, 10, 0);
    ASSERT_EQ(100, ret);
}

TEST_F(IOTest, Pwritev) {
    EXPECT_CALL(scm, pwritev(0, nullptr, 0, 0)).WillOnce(Return(0));
    auto ret = UninterruptedPwritev(0, nullptr, 0, 0);
    ASSERT_EQ(0, ret);

    EXPECT_CALL(scm, pwritev(0, nullptr, 100, 0))
        .WillOnce(SetErrnoAndReturn(EINTR, -1))
        .WillOnce(SetErrnoAndReturn(EAGAIN, -1));
    ret = UninterruptedPwritev(0, nullptr, 100, 0);
    ASSERT_EQ(-1, ret);

    iovec iov[10];
    for (int i = 0; i < 10; i++){
        iov[i].iov_base = nullptr;
        iov[i].iov_len = 10;
    }
    EXPECT_CALL(scm, pwritev(0, iov, 10, 0)).WillOnce(Return(35));
    EXPECT_CALL(scm, pwritev(0, iov + 3, 7, 35)).WillOnce(Return(30));
    EXPECT_CALL(scm, pwritev(0, iov + 6, 4, 65)).WillOnce(Return(25));
    EXPECT_CALL(scm, pwritev(0, iov + 9, 1, 90)).WillOnce(Return(10));
    ret = UninterruptedPwritev(0, iov, 10, 0);
    ASSERT_EQ(100, ret);
}

int main(int argc, char **argv) {
    //testing::AddGlobalTestEnvironment(new FooEnvironment);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
