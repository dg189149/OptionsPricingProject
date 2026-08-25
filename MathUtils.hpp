#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include <cmath>
#include <random>
#include <algorithm>

double normalCDF(double x) {
    return 0.5 * (1.0 + std::erf(x * M_SQRT1_2));
}

double DNormalCDF(double x) {
    return std::exp(-0.5 * x * x) / std::sqrt(2.0 * M_PI);
}

#endif // MATHUTILS_HPP