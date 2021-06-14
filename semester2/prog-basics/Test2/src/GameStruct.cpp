#include "GameStruct.h"
#include "misc.h"

#include <cmath>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <iostream>

namespace test
{
    double VideoGame::get_value() const
    {
        double value = 0;
        for(const GameReview& rating : reviews)
        {
            value += rating.score * rating.score;
        }
        value /= reviews.size();
        value = std::sqrt(value);

        return std::pow(value, VARIANT + 2 * profit_ratio);
    }

    namespace detail
    {
        bool compare_by_name(const GameReview& review1, const GameReview& review2)
        {
//            std::cout << "hhihi";
            return review1.name.compare(review2.name) < 0;
        }
    }

    double VideoGame::get_distance(const VideoGame& game1, const VideoGame& game2)
    {
        GameReview* reviews1 = new GameReview[game1.reviews.size()];
        std::copy(game1.reviews.begin(), game1.reviews.end(), reviews1);
        std::sort(reviews1, reviews1 + game1.reviews.size(), detail::compare_by_name);

        GameReview* reviews2 = new GameReview[game2.reviews.size()];
        std::copy(game2.reviews.begin(), game2.reviews.end(), reviews2);
        std::sort(reviews2, reviews2 + game2.reviews.size(), detail::compare_by_name);

        double distance = 0;

        int min = std::min(game1.reviews.size(), game2.reviews.size());
        for(int i = 0; i < min; i++)
        {
            distance += std::pow(game1.reviews[i].score - game2.reviews[i].score, 2);
        }
        distance += std::pow(game1.year - game2.year, 2);
        distance += 1;

        distance = std::pow(distance, -test::VARIANT * std::min(game1.profit_ratio, game2.profit_ratio) / 2);

        return 1 - distance;
    }

    std::ostream& operator<<(std::ostream& os, const GameReview& review)
    {
        return os << review.name << ": " << review.score;
    }

    std::ostream& operator<<(std::ostream& os, const VideoGame& game)
    {
        game.print(os, VideoGame::print_verbose, false);
        return os;
    }

    bool VideoGame::print_verbose = false;

    void VideoGame::print(std::ostream& os, bool verbose, bool print_new_lines) const
    {
        if(verbose)
        {
            os << '"' << name << '"' << (print_new_lines ? "\n" : ", ")
                    << "Release year: " << year << (print_new_lines ? "\n" : ", ")
                    << "Reviews: ";

            if(!reviews.empty())
                os << reviews[0];
            for(int i = 1; i < reviews.size(); i++)
                os << ", " << reviews[i];

            if(inspired_by)
                os << (print_new_lines ? "\n" : ", ") << "Inspired by: \"" << inspired_by->name << '"';

            os << (print_new_lines ? "\n" : ", ") << "Profit ratio: " << profit_ratio;
            if(print_new_lines)
                os << std::endl;
        }
        else
        {
            os << name;
        }
    }

    bool operator<(const VideoGame& game1, const VideoGame& game2)
    {
        return game1.get_value() < game2.get_value();
    }

    bool operator>(const VideoGame& game1, const VideoGame& game2)
    {
        return game1.get_value() > game2.get_value();
    }

    bool operator<=(const VideoGame& game1, const VideoGame& game2)
    {
        return game1.get_value() <= game2.get_value();
    }

    bool operator>=(const VideoGame& game1, const VideoGame& game2)
    {
        return game1.get_value() >= game2.get_value();
    }

    VideoGame VideoGame::make_random()
    {
        static int franchise_num = 1;

        std::string name = "Game #" + std::to_string(franchise_num);
        franchise_num++;

        int year = test::get_random_int(1980, 2019);

        int reviews_count = test::get_random_int(1, test::VARIANT + 5);
        std::vector<GameReview> reviews;
        reviews.reserve(reviews_count);
        for(int i = 0; i < reviews_count; i++)
        {
            std::string critic_name = (std::rand() % 2) ? "Youtuber #" : "Critic #";
            critic_name += std::to_string(test::get_random_int(1, 1000));

            int review_score = test::get_random_int(0, 100);
            reviews.push_back(test::GameReview(critic_name, review_score));
        }

        float profit_ratio = (std::rand() % 2) ? test::get_random_float(0.1, 1) : test::get_random_float(1, 20);
        return VideoGame(name, year, reviews, profit_ratio);
    }
}
