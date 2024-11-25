// Rating.h
#ifndef RATING_H
#define RATING_H

#include <vector>
#include <string>
#include <stdexcept>

class Rating {
private:
    struct RatingComment {
        int rating;
        std::string comment;
        std::string username;
    };

    std::vector<RatingComment> ratings;

public:
    void addRating(int rating, const std::string& comment, const std::string& username);
    void displayRatingsAndComments() const;
    void deleteRatingComment(int index);
    const std::vector<RatingComment>& getRatings() const;
};

#endif // RATING_H
