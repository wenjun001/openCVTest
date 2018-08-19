// Copyright (C)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_newdefine_
#define DLIB_newdefine_

#include <string>
#include <sstream>

using namespace std;
namespace std {
    template <typename T> std::string to_string(const T& n) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

namespace std {
    template <typename T> T round(T v) {
        return (v > 0) ? (v + 0.5) : (v - 0.5);
    }
}

#endif // DLIB_newdefine_

