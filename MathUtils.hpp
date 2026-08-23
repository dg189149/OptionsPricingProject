#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include <cmath>
#include <random>
#include <algorithm>

double normalCDF(double x) {
    return 0.5 * (1.0 + std::erf(x * M_SQRT1_2));
}

#endif // MATHUTILS_HPP