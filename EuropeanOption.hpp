#ifndef EURPEANOPTION_HPP
#define EURPEANOPTION_HPP

#include <cmath>
#include <random>
#include <algorithm>
#include "MathUtils.hpp"


class EuropeanOption {
private:
    double stockPrice; // Current stock price
    double strikePrice; // Strike price of the option
    double timeToMaturity; // Time to maturity in years
    double riskFreeRate; // Risk-free interest rate
    double volatility; // Volatility of the underlying stock

public:
    EuropeanOption(double S, double K, double T, double r, double sigma);
    double getStockPrice() const;
    double getStrikePrice() const;
    double getTimeToMaturity() const;
    double getRiskFreeRate() const;
    double getVolatility() const;
    double d1() const;
    double d2() const;
    double callPrice() const;
    double putPrice() const;
    double monteCarloCallPrice(int numSimulations) const;
};

EuropeanOption::EuropeanOption(double S, double K, double T, double r, double sigma){
    this->stockPrice = S;
    this->strikePrice = K;
    this->timeToMaturity = T;
    this->riskFreeRate = r;
    this->volatility = sigma;
}

double EuropeanOption::getStockPrice() const {
    return stockPrice;
}

double EuropeanOption::getStrikePrice() const {
    return strikePrice;
}

double EuropeanOption::getTimeToMaturity() const {
    return timeToMaturity;
}

double EuropeanOption::getRiskFreeRate() const {
    return riskFreeRate;
}

double EuropeanOption::getVolatility() const {
    return volatility;
}

double EuropeanOption::d1() const {
    return (std::log(stockPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) / (volatility * std::sqrt(timeToMaturity));
}

double EuropeanOption::d2() const {
    return d1() - volatility * std::sqrt(timeToMaturity);
}

double EuropeanOption::callPrice() const {
    double d1_val = d1();
    double d2_val = d2();
    return stockPrice * normalCDF(d1_val) - strikePrice * std::exp(-riskFreeRate * timeToMaturity) * normalCDF(d2_val);
}

double EuropeanOption::putPrice() const {
    double d1_val = d1();
    double d2_val = d2();
    return strikePrice * std::exp(-riskFreeRate * timeToMaturity) * normalCDF(-d2_val) - stockPrice * normalCDF(-d1_val);
}

double EuropeanOption::monteCarloCallPrice(int numSimulations) const {
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::normal_distribution<double> dist(0.0, 1.0);

    double sumPayoffs = 0.0;

    for (int i = 0; i < numSimulations; ++i) {
        double z = dist(gen);
        double S_T = stockPrice * std::exp((riskFreeRate - 0.5 * volatility * volatility) * timeToMaturity + volatility * std::sqrt(timeToMaturity) * z);
        double payoff = std::max(S_T - strikePrice, 0.0);
        sumPayoffs += payoff;
    }
    double averagePayoff = sumPayoffs / numSimulations;
    return std::exp(-riskFreeRate * timeToMaturity) * averagePayoff;
}
#endif // EURPEANOPTION_HPP