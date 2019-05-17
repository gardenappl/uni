#include "misc.h"

#include <cstdlib>

namespace test
{
    int get_random_int(int min, int max)
    {
        return min + (std::rand() % (max - min));
    }

    float get_random_float(float min, float max)
    {
        return min + std::rand() / (RAND_MAX / (max - min));
    }
}
