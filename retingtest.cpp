#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "reting.h" // Include the header file where your Rating class is defined

TEST_CASE("Rating class tests") {
    Rating ratingSystem;

    SECTION("Adding valid ratings") {
        REQUIRE_NOTHROW(ratingSystem.addRating(5, "Excellent!", "User1"));
        REQUIRE_NOTHROW(ratingSystem.addRating(7, "Average.", "User2"));
        REQUIRE_NOTHROW(ratingSystem.addRating(1, "Poor.", "User3"));
    }

    SECTION("Adding invalid ratings") {
        REQUIRE_THROWS_AS(ratingSystem.addRating(0, "Invalid rating.", "User4"), std::invalid_argument);
        REQUIRE_THROWS_AS(ratingSystem.addRating(6, "Invalid rating.", "User5"), std::invalid_argument);
    }

    SECTION("Displaying ratings and comments") {
        ratingSystem.addRating(5, "Excellent!", "User1");
        ratingSystem.addRating(3, "Average.", "User2");

        std::stringstream buffer;
        std::streambuf *oldCout = std::cout.rdbuf(buffer.rdbuf()); // Redirect cout to buffer

        ratingSystem.displayRatingsAndComments();

        std::cout.rdbuf(oldCout); // Restore cout
        std::string output = buffer.str();

        REQUIRE(output.find("User1 rated: 5 | Comment: Excellent!") != std::string::npos);
        REQUIRE(output.find("User2 rated: 3 | Comment: Average.") != std::string::npos);
    }

    SECTION("Deleting a rating comment") {
        ratingSystem.addRating(5, "Excellent!", "User1");
        ratingSystem.addRating(3, "Average.", "User2");

        REQUIRE_NOTHROW(ratingSystem.deleteRatingComment(1)); // Should delete User1's comment
        REQUIRE_THROWS_AS(ratingSystem.deleteRatingComment(3), std::invalid_argument); // Invalid index
        REQUIRE(ratingSystem.getRatings().size() == 1); // Only one rating should remain
    }

    SECTION("Deleting invalid index") {
        ratingSystem.addRating(5, "Excellent!", "User1");
        REQUIRE_THROWS_AS(ratingSystem.deleteRatingComment(0), std::invalid_argument); // Invalid index
        REQUIRE_THROWS_AS(ratingSystem.deleteRatingComment(2), std::invalid_argument); // Invalid index
    }
}
