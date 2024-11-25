// Rating.cpp
#include "reting.h"
#include <iostream>

void Rating::addRating(int rating, const std::string& comment, const std::string& username) {
    if (rating < 1 || rating > 5) {
        throw std::invalid_argument("Rating must be between 1 and 5.");
    }
    ratings.push_back({rating, comment, username});
}

void Rating::displayRatingsAndComments() const {
    if (ratings.empty()) {
        std::cout << "No ratings or comments yet.\n";
    } else {
        std::cout << "Ratings and Comments:\n";
        for (size_t i = 0; i < ratings.size(); ++i) {
            const auto& rc = ratings[i];
            std::cout << i + 1 << ". " << rc.username << " rated: " << rc.rating
                      << " | Comment: " << rc.comment << std::endl;
        }
    }
}

void Rating::deleteRatingComment(int index) {
    if (index < 1 || index > static_cast<int>(ratings.size())) {
        throw std::invalid_argument("Invalid index.");
    }
    ratings.erase(ratings.begin() + (index - 1));
}

const std::vector<Rating::RatingComment>& Rating::getRatings() const {
    return ratings;
}
