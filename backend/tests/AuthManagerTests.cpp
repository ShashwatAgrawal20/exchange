#include "include/AuthManager.hpp"
#include "tec.h"
#include <chrono>
#include <string>

static AuthManager auth;
TEC_SETUP(auth_manager){};

TEC(auth_manager, ValidCredentialsAuthenticate) {
    bool result = auth.authenticate("shashwat", "shashwat");
    TEC_ASSERT_TRUE(result);

    result = auth.authenticate("nero", "nero");
    TEC_ASSERT_TRUE(result);
}
TEC(auth_manager, InvalidCredentialsFail) {

    TEC_ASSERT_FALSE(auth.authenticate("wrong", "credentials"));
    TEC_ASSERT_FALSE(auth.authenticate("shashwat", "WRONG"));
    TEC_ASSERT_FALSE(auth.authenticate("unknown", "shashwat"));
}

TEC(auth_manager, EmptyCredentialsFail) {
    TEC_ASSERT_FALSE(auth.authenticate("", ""));
    TEC_ASSERT_FALSE(auth.authenticate("", "something"));
    TEC_ASSERT_FALSE(auth.authenticate("someone", ""));
}

TEC(auth_manager, UsernameCaseSensitive) {
    bool lower = auth.authenticate("shashwat", "shashwat");
    bool upper = auth.authenticate("SHASHWAT", "shashwat");
    TEC_ASSERT(lower);
    TEC_ASSERT_FALSE(upper);
}

TEC(auth_manager, PasswordCaseSensitive) {
    bool exact = auth.authenticate("nero", "nero");
    bool mixed = auth.authenticate("nero", "NeRo");
    TEC_ASSERT(exact);
    TEC_ASSERT_FALSE(mixed);
}

TEC(auth_manager, WhitespaceInCredentialsInvalidates) {
    TEC_ASSERT_FALSE(auth.authenticate(" shashwat", "shashwat"));
    TEC_ASSERT_FALSE(auth.authenticate("shashwat ", "shashwat"));
    TEC_ASSERT_FALSE(auth.authenticate("shashwat", " shashwat"));
    TEC_ASSERT_FALSE(auth.authenticate("shashwat", "shashwat "));
}

TEC(auth_manager, RepeatedCallsAreIdempotent) {
    bool first = auth.authenticate("shashwat", "shashwat");
    bool second = auth.authenticate("shashwat", "shashwat");
    TEC_ASSERT_EQ(first, second);
}

TEC(auth_manager, PerformanceSanity) {
    auto start = std::chrono::high_resolution_clock::now();
    auth.authenticate("shashwat", "shashwat");
    auth.authenticate("nero", "nero");
    auth.authenticate("wrong", "credentials");
    auto end = std::chrono::high_resolution_clock::now();

    /* THIS SHIT USED TO BE UNDER 3 microseconds, but we had a check on the safe
     * side to be around 10. But now that we've added those fsanitize compile
     * flags, the performance has degraded quite a bit, so to be safe we're
     * making this 100.
     */
    auto elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    TEC_ASSERT_LT(elapsed, 100);
}
