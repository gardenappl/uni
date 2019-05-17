#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "GameStruct.h"
#include "misc.h"

namespace test
{
    void task1()
    {
        const int ELEMENT_COUNT = 139;

        VideoGame games[ELEMENT_COUNT];
        for(int i = 0; i < ELEMENT_COUNT; i++)
            games[i] = VideoGame::make_random();

        std::cout << (games[0] < games[1]);
        std::sort(std::begin(games), std::end(games));

        for(const VideoGame& game : games)
        {
            game.print();
            std::cout << "========Value: " << game.get_value() << std::endl;
        }
    }
}

int main()
{
    std::srand(std::time(0));

    test::task1();

//    test::VideoGame game1 = test::VideoGame::make_random();
//    game1.print();
//    test::VideoGame game2 = test::VideoGame::make_random();
//    game2.print();
//    std::cout << test::VideoGame::get_distance(game1, game2);

    return 0;
}
