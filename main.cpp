#include <iostream>
#include "MathUtils.hpp"
#include "EuropeanOption.hpp"

int main() {
    EuropeanOption option(100, 100, 1, 0.05, 0.2);
    std::cout << "Stock Price: " << option.getStockPrice() << std::endl;
    std::cout << "Strike Price: " << option.getStrikePrice() << std::endl;
    std::cout << "Time to Maturity: " << option.getTimeToMaturity() << std::endl;
    std::cout << "Risk-Free Rate: " << option.getRiskFreeRate() << std::endl;
    std::cout << "Volatility: " << option.getVolatility() << std::endl;
    std::cout << "d1: " << option.d1() << std::endl;
    std::cout << "d2: " << option.d2() << std::endl;
    std::cout << "Normal CDF of d1: " << normalCDF(option.d1()) << std::endl;
    std::cout << "Normal CDF of d2: " << normalCDF(option.d2()) << std::endl;
    std::cout << "Call Price: " << option.callPrice() << std::endl;
    std::cout << "Put Price: " << option.putPrice() << std::endl;
    double call = option.callPrice();
    double put = option.putPrice();
    double lhs = call - put;
    double rhs = option.getStockPrice() - option.getStrikePrice() * std::exp(-option.getRiskFreeRate() * option.getTimeToMaturity());  
    std::cout << "Call - Put: " << lhs << std::endl;
    std::cout << "S - K * exp(-r * T): " << rhs << std::endl;
    int numSimulations = 1000000;
    double monteCarloCallPrice = option.monteCarloCallPrice(numSimulations);
    std::cout << "Monte Carlo Call Price with " << numSimulations << " simulations: " << monteCarloCallPrice << std::endl;
    return 0;
}