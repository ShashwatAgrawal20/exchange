#include "include/MarketData.hpp"
#include "tec.h"

#include <string>

static std::string snapshot;

TEC_SETUP(market_data) { snapshot = create_market_snapshot(); }

TEC(market_data, SnapshotCreatesValidJson) {
    TEC_ASSERT_NE(snapshot.length(), (size_t)0);
    TEC_ASSERT(snapshot.starts_with("{"));
    TEC_ASSERT(snapshot.ends_with("}"));

    TEC_ASSERT_NE(snapshot.find("\"ask\":"), std::string::npos);
    TEC_ASSERT_NE(snapshot.find("\"bid\":"), std::string::npos);

    TEC_ASSERT_EQ(snapshot.find("\"timestamp\":"), std::string::npos);
    TEC_ASSERT_EQ(snapshot.find("null"), std::string::npos);
}

TEC(market_data, AskHasAllFiveLevels) {
    for (int i = 1; i <= 5; ++i) {
        std::string level_key = "\"level_" + std::to_string(i) + "\":";
        size_t ask_start = snapshot.find("\"ask\":");
        size_t bid_start = snapshot.find("\"bid\":");

        size_t level_pos = snapshot.find(level_key, ask_start);
        TEC_ASSERT_NE(level_pos, std::string::npos);
        TEC_ASSERT_LT(level_pos, bid_start);
    }
}

TEC(market_data, BidHasAllFiveLevels) {
    for (int i = 1; i <= 5; ++i) {
        std::string level_key = "\"level_" + std::to_string(i) + "\":";
        size_t bid_start = snapshot.find("\"bid\":");
        size_t closing = snapshot.find("}", bid_start);

        size_t level_pos = snapshot.find(level_key, bid_start);
        TEC_ASSERT_NE(level_pos, std::string::npos);
        TEC_ASSERT_LT(level_pos, closing);
    }
}
TEC(market_data, SnapshotIsRepeatable) {
    std::string snapshot2 = create_market_snapshot();

    TEC_ASSERT(snapshot.starts_with("{"));
    TEC_ASSERT(snapshot2.starts_with("{"));
    TEC_ASSERT(snapshot.ends_with("}"));
    TEC_ASSERT(snapshot2.ends_with("}"));

    for (int i = 1; i <= 5; ++i) {
        std::string level_key = "\"level_" + std::to_string(i) + "\":";
        TEC_ASSERT_NE(snapshot.find(level_key), std::string::npos);
        TEC_ASSERT_NE(snapshot2.find(level_key), std::string::npos);
    }
}
