// tests/unit/test_tracker.cpp

#include <gtest/gtest.h>
#include "../../src/application/ConnectionTracker.h"

TEST(ConnectionTrackerBasicTest, StartsEmpty) {
    ConnectionTracker tracker;

    EXPECT_EQ(tracker.getActiveConnectionsCount(), 0u);
}