#include "misc.h"

#include <string>

namespace lists
{
    int str_compare(const std::string& s1, const std::string& s2)
    {
        int length_diff = s1.length() - s2.length();
        if(length_diff != 0)
            return length_diff;
        if(s1 == s2)
            return 0;
        return s1 < s2 ? -1 : 1;
    }
}
