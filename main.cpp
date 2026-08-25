#include <iostream>
#include <chrono>
#include "MathUtils.hpp"
#include "EuropeanOption.hpp"


int main() {
    EuropeanOption option(100, 100, 1, 0.05, 0.2);
    std::cout << "Delta: " << option.delta() << std::endl;
    std::cout << "Vega: " << option.vega() << std::endl;
    std::cout << "Gamma: " << option.gamma() << std::endl;
    std::cout << "Theta: " << option.theta() << std::endl;
    std::cout << "Rho: " << option.rho() << std::endl;
    
    int n = 1000;
    std::cout << "--- Plain Monte Carlo, n=" << n << " ---" << std::endl;
    for (int trial = 0; trial < 8; ++trial) {
        std::cout << option.monteCarloCallPrice(n) << std::endl;
    }
    std::cout << "--- Antithetic, n=" << n << " ---" << std::endl;
    for (int trial = 0; trial < 8; ++trial) {
        std::cout << option.monteCarloCallPriceAntiThetic(n) << std::endl;
    } 
    return 0;
}