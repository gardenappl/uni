#ifndef GAMESTRUCT_H
#define GAMESTRUCT_H

#include <string>
#include <vector>
#include "misc.h"

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
        static bool print_verbose;

        std::string name;
        int year;
        std::vector<GameReview> reviews;
        float profit_ratio;
        VideoGame* inspired_by = nullptr;

        VideoGame() = default;

        VideoGame(const std::string& name, int year, const std::vector<GameReview>& reviews, float profit_ratio)
        : name(name), year(year), reviews(reviews), profit_ratio(profit_ratio)
        {}

        void print(std::ostream& os, bool verbose, bool print_new_lines = true) const;
        double get_value() const;

        static double get_distance(const VideoGame& game1, const VideoGame& game2);

        template<typename Iterator>
        static VideoGame make_random(Iterator other_games_start, Iterator other_games_end);

        static VideoGame make_random();
    };

    std::ostream& operator<<(std::ostream& os, const VideoGame& game);

    bool operator<(const VideoGame& game1, const VideoGame& game2);
    bool operator>(const VideoGame& game1, const VideoGame& game2);
    bool operator<=(const VideoGame& game1, const VideoGame& game2);
    bool operator>=(const VideoGame& game1, const VideoGame& game2);

    template<typename Iterator>
    VideoGame VideoGame::make_random(Iterator other_games_start, Iterator other_games_end)
    {
        VideoGame game = VideoGame::make_random();
        if(std::rand() % 2 && other_games_start != other_games_end)
            game.inspired_by = other_games_start[std::rand() % (other_games_end - other_games_start)];
        return game;
    }
}

#endif // GAMESTRUCT_H
