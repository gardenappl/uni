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
//            std::cout << value << std::endl;
        }
        value /= reviews.size();
//            std::cout << value << std::endl;
        value = std::sqrt(value);
//            std::cout << value << std::endl;

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
        distance++;

        distance = std::pow(distance, -test::VARIANT * std::min(game1.profit_ratio, game2.profit_ratio) / 2);

        return 1 - distance;
    }

    VideoGame VideoGame::make_random()
    {
//        std::random_device rd;     // only used once to initialise (seed) engine
//        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
//        std::uniform_int_distribution<int> name_distribution(1, 50); // guaranteed unbiased
//        std::uniform_int_distribution<int> year_distribution(1980, 2019);
//        std::uniform_int_distribution<int> bool_distribution(0, 1);
//        std::uniform_int_distribution<float> profit_ratio_distribution(0, 100);
//        std::uniform_int_distribution<int> score_distribution(1, 100);

        std::string name;
        if(std::rand() % 2)
            name = "Final Fantasy ";
        else
            name = "Fortnite ";
        name += std::to_string(test::get_random_int(1, 50));

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

        float profit_ratio = (std::rand() % 2) ? test::get_random_float(0, 1) : test::get_random_float(1, 20);
        return VideoGame(name, year, reviews, profit_ratio);
    }

    std::ostream& operator<<(std::ostream& os, const GameReview& review)
    {
        return os << review.name << ": " << review.score;
    }

    void VideoGame::print() const
    {
        std::cout << '"' << name << "\"\n"
                << "Release year: " << year << '\n'
                << "Reviews: ";

        if(!reviews.empty())
            std::cout << reviews[0];
        for(int i = 1; i < reviews.size(); i++)
            std::cout << ", " << reviews[i];

        std::cout << "\nProfit ratio: " << profit_ratio << std::endl;
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
}
