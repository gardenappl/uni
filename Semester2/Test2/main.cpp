#include <iostream>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <algorithm>
#include <vector>
#include <map>
#include <stack>
#include <limits>

#include "GameStruct.h"
#include "misc.h"
#include "Tree.h"
#include "Graph.h"
#include "BinarySearchTree.h"

namespace test
{
    void task1()
    {
        const int ELEMENT_COUNT = 139;

        VideoGame games[ELEMENT_COUNT];
        VideoGame* game_pointers[ELEMENT_COUNT];

        for(int i = 0; i < ELEMENT_COUNT; i++)
        {
            games[i] = VideoGame::make_random(std::begin(game_pointers), std::begin(game_pointers) + i);
            game_pointers[i] = &games[i];
        }

        std::sort(std::begin(game_pointers), std::end(game_pointers), [](VideoGame* ptr1, VideoGame* ptr2)
                {
                    return *ptr1 < *ptr2;
                });


        for(const VideoGame* game : game_pointers)
        {
            game->print(std::cout, true);
            std::cout << "Value: " << game->get_value() << "\n\n";
        }

        std::vector<std::tuple<VideoGame*, VideoGame*, double>> pairs;
        for(int i = 0; i < ELEMENT_COUNT; i++)
        {
            for(int i2 = 0; i2 < ELEMENT_COUNT; i2++)
            {
                if(i == i2)
                    continue;
                pairs.push_back(std::make_tuple(&games[i], &games[i2], VideoGame::get_distance(games[i], games[i2])));
            }
        }
        std::sort(pairs.begin(), pairs.end(), [](const auto& pair1, const auto& pair2)
                {
                    return std::get<2>(pair1) < std::get<2>(pair2);
                });

        std::cout << " === Shortest distances: ===\n";
        for(int i = 16; i >= 0; i--)
        {
            auto [ game1, game2, distance ] = pairs[i];
            game1->print(std::cout, true);
            game2->print(std::cout, true);
            std::cout << "   Distance:" << distance << '\n' << std::endl;
        }

        std::cout << " === Longest distances: ===\n";
        for(int i = pairs.size() - 17; i < pairs.size(); i++)
        {
            auto [ game1, game2, distance ] = pairs[i];
            game1->print(std::cout, true);
            game2->print(std::cout, true);
            std::cout << "   Distance:" << distance << '\n' << std::endl;
        }
    }


    void task2()
    {
        const int ELEMENT_COUNT = 32;

        std::vector<TreeNode<VideoGame>*> current_nodes;
        current_nodes.reserve(ELEMENT_COUNT);

        TreeNode<VideoGame>* tree_root = new TreeNode<VideoGame>(VideoGame::make_random());
        current_nodes.push_back(tree_root);

        //create random children
        for(int i = 1; i < ELEMENT_COUNT; i++)
        {
            TreeNode<VideoGame>* new_node = new TreeNode<VideoGame>(VideoGame::make_random());
            if(std::rand() % 6 == 0)
            {
                tree_root->attach(new_node);
            }
            else
            {
                current_nodes[std::rand() % current_nodes.size()]->attach(new_node);
            }
            new_node->value.inspired_by = &new_node->parent->value;
            current_nodes.push_back(new_node);
        }

        VideoGame::print_verbose = false;
        tree_root->print();

        std::cout << "\n\n\n";

        std::vector<TreeNode<VideoGame>*> lvl_2_nodes = tree_root->get_nodes_at_level(2);

        std::map<TreeNode<VideoGame>*, double> average_map;
        for(TreeNode<VideoGame>* node : lvl_2_nodes)
            average_map.emplace(node, node->get_average_value());


        TreeNode<VideoGame>* max_node = *std::max_element(lvl_2_nodes.begin(), lvl_2_nodes.end(),
                [&](TreeNode<VideoGame>* game1, TreeNode<VideoGame>* game2)
                {
                    return average_map[game1] < average_map[game2];
                });

        TreeNode<VideoGame>* node_to_insert = lvl_2_nodes[std::rand() % lvl_2_nodes.size()];
        std::cout << "Moving: " << node_to_insert->value << "\n\n\n";

        if(max_node != node_to_insert)
        {
            node_to_insert->parent->remove(node_to_insert);
            max_node->attach(node_to_insert);
        }

        tree_root->print();

        std::cout << "Height: " << tree_root->get_sub_height() << '\n'
                << "Count:" << tree_root->get_sub_count() << '\n'
                << "Immediate children: " << tree_root->children_count << '\n'
                << "Minimal value: " << tree_root->get_min_value() << '\n'
                << "Maximum value: " << tree_root->get_max_value() << '\n'
                << "Average value: " << tree_root->get_average_value() << '\n';

        delete tree_root;
    }



    void task3()
    {
        const int ELEMENT_COUNT = 32;

        std::vector<VideoGame> games;
        games.reserve(ELEMENT_COUNT);
        for(int i = 0; i < ELEMENT_COUNT; i++)
            games.push_back(VideoGame::make_random());

        const double MIN_DISTANCE = 0.8;
        test::MatrixGraph<VideoGame, ELEMENT_COUNT>* graph = test::MatrixGraph<VideoGame, ELEMENT_COUNT>::
                construct(games.begin(), games.end(), VideoGame::get_distance, MIN_DISTANCE);

        graph->print(std::cout);
        std::cout << "Tree contains distances >= " << MIN_DISTANCE << std::endl;

        std::cout << "Component count: " << graph->get_component_count() << std::endl;

        //Get minimum distance to node in current component

        //get_component_count() used BFS, so this should use DFS :)

        int start_index = std::rand() % games.size(); //random start

        double current_min = std::numeric_limits<double>::infinity();

        bool visited[ELEMENT_COUNT];
        for(int i = 0; i < ELEMENT_COUNT; i++)
            visited[i] = false;

        std::stack<int> dfs_stack;
        int current_min_element = start_index;
        //start at first neighbour (otherwise the DFS will match the start element with itself and return 0)
        for(int i = 0; i < ELEMENT_COUNT; i++)
        {
            if(graph->distance[start_index][i] != MatrixGraph<VideoGame, ELEMENT_COUNT>::NOT_CONNECTED)
            {
                dfs_stack.push(i);
                break;
            }
        }
        visited[start_index] = true;

        while(!dfs_stack.empty())
        {
            int current_index = dfs_stack.top();
            dfs_stack.pop();
            visited[current_index] = true;

            double current_distance = graph->distance[current_index][start_index];
            if(current_distance == MatrixGraph<VideoGame, ELEMENT_COUNT>::NOT_CONNECTED)
                current_distance = VideoGame::get_distance(graph->index_to_value[current_index], graph->index_to_value[start_index]);

            if(current_distance < current_min)
            {
                current_min = current_distance;
                current_min_element = current_index;
            }

            for(int i = 0; i < ELEMENT_COUNT; i++)
            {
                if(!visited[i] && graph->distance[current_index][i] != MatrixGraph<VideoGame, ELEMENT_COUNT>::NOT_CONNECTED)
                {
                    dfs_stack.push(i);
                }
            }
        }
        std::cout << "Minimal distance: " << current_min
                << " (Starting point: " << graph->index_to_value[start_index]
                << ", end point: " << graph->index_to_value[current_min_element] << ")" << std::endl;
    }

    bool pair_compare_func(const std::pair<const VideoGame&, const VideoGame&>& game_pair1, const std::pair<const VideoGame&, const VideoGame&>& game_pair2)
    {
        return VideoGame::get_distance(game_pair1.first, game_pair1.second) <
                VideoGame::get_distance(game_pair2.first, game_pair2.second);
    }

    void task4()
    {
        BinaryPairSearchTree<VideoGame, pair_compare_func> tree;

        auto print_func = [](const std::pair<const VideoGame&, const VideoGame&>& pair)
                {
                    std::cout << "{ " << pair.first << ", " << pair.second << ", distance = "
                            << VideoGame::get_distance(pair.first, pair.second) << " }\n";
                };

        for(int i = 0; i < 8; i++)
        {
            tree.insert(VideoGame::make_random());
            tree.print(print_func);
            std::cout << std::endl;
        }
        tree.print(print_func);
    }

}
int main()
{
    std::srand(std::time(0));

    test::task4();

//    test::VideoGame game1 = test::VideoGame::make_random();
//    game1.print();
//    test::VideoGame game2 = test::VideoGame::make_random();
//    game2.print();
//    std::cout << test::VideoGame::get_distance(game1, game2);

    return 0;
}
