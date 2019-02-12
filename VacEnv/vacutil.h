//
// Created by Oliver on 12.02.2019.
//

#ifndef VACBOTSIM_UTIL_H
#define VACBOTSIM_UTIL_H

#include <utility>

enum {
    TDC_LEFT = 0x1,
    TDC_RIGHT = 0x2,
    TDC_UP = 0x4,
    TDC_DOWN = 0x8
};

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

template<typename tVal>
inline tVal map_value(std::pair<tVal, tVal> a, std::pair<tVal, tVal> b, tVal inVal)
{
    tVal inValNorm = inVal - a.first;
    tVal aUpperNorm = a.second - a.first;
    tVal normPosition = inValNorm / aUpperNorm;

    tVal bUpperNorm = b.second - b.first;
    tVal bValNorm = normPosition * bUpperNorm;
    tVal outVal = b.first + bValNorm;

    return outVal;
}

#endif //VACBOTSIM_UTIL_H
