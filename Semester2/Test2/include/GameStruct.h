#ifndef GAMESTRUCT_H
#define GAMESTRUCT_H

#include <string>
#include <vector>

namespace test
{
    struct GameReview
    {
        std::string name;
        int score;

        GameReview() = default;

        GameReview(const std::string& name, int score)
        : name(name), score(score)
        {}
    };

    struct VideoGame
    {
        std::string name;
        int year;
        std::vector<GameReview> reviews;
        float profit_ratio;

        VideoGame() = default;

        VideoGame(const std::string& name, int year, const std::vector<GameReview>& reviews, float profit_ratio)
        : name(name), year(year), reviews(reviews), profit_ratio(profit_ratio)
        {}

        void print() const;
        double get_value() const;

        static double get_distance(const VideoGame& game1, const VideoGame& game2);
        static VideoGame make_random();
    };

    bool operator<(const VideoGame& game1, const VideoGame& game2);
    bool operator>(const VideoGame& game1, const VideoGame& game2);
    bool operator<=(const VideoGame& game1, const VideoGame& game2);
    bool operator>=(const VideoGame& game1, const VideoGame& game2);
}

#endif // GAMESTRUCT_H
