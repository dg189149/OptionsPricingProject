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
    EuropeanOption(double S, double K, double T, double r, double sigma); // constructor to intialize the option parameters
    double getStockPrice() const; // Getter for stock price
    double getStrikePrice() const; // Getter for strike price
    double getTimeToMaturity() const; // Getter for time to maturity
    double getRiskFreeRate() const; // Getter for risk-free interest rate
    double getVolatility() const; // Getter for volatility
    double d1() const; // Calculates d1 for the Black-Scholes formula
    double d2() const; // Calculates d2 for the Black-Scholes formula
    double delta() const; // Calculates the delta of the option
    double vega() const; // Calculates the vega of the option
    double gamma() const; // Calculates the gamma of the option
    double theta() const; // Calculates the theta of the option
    double rho() const; // Calculates the rho of the option
    double callPrice() const; // Calculates the price of a European call option using the Black-Scholes formula
    double putPrice() const; // Calculates the price of a European put option using the Black-Scholes formula
    double monteCarloCallPrice(int numSimulations) const; // Calculates the price of a European call option using Monte Carlo simulation
    double monteCarloCallPriceAntiThetic(int numSimulations) const; // Calculates the price of a European call option using Monte Carlo simulation with antithetic variates
};

EuropeanOption::EuropeanOption(double S, double K, double T, double r, double sigma){
    this->stockPrice = S; // Current stock price
    this->strikePrice = K; // Strike price of the option
    this->timeToMaturity = T; // Time to maturity in years
    this->riskFreeRate = r; // Risk-free interest rate
    this->volatility = sigma; // Volatility of the underlying stock
}

double EuropeanOption::getStockPrice() const {
    return stockPrice; // Getter for stock price
}

double EuropeanOption::getStrikePrice() const {
    return strikePrice; // Getter for strike price
}

double EuropeanOption::getTimeToMaturity() const {
    return timeToMaturity; // Getter for time to maturity
}

double EuropeanOption::getRiskFreeRate() const {
    return riskFreeRate; // Getter for risk-free interest rate
}

double EuropeanOption::getVolatility() const {
    return volatility; // Getter for volatility
}

double EuropeanOption::d1() const {
    // Calculate d1 using the Black-Scholes formula
    // d1 = (ln(S/K) + (r + 0.5 * sigma^2) * T) / (sigma * sqrt(T))
    // In layman terms:
    // d1 measures how many s.d the stock price is above the strike price, adjusted for time and volatility.
    return (std::log(stockPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) / (volatility * std::sqrt(timeToMaturity));
}

double EuropeanOption::d2() const {
    // Calculate d2 using the Black-Scholes formula
    // d2 = d1 - sigma * sqrt(T)
    // In layman terms:
    // d2 is similar to d1 but adjusted downwards by the expected volatility
    // over the time to maturity. It represents the expected position of the stock price 
    // relative to the strike price at expiration.
    return d1() - volatility * std::sqrt(timeToMaturity);
}

double EuropeanOption::delta() const {
    // Calculate the delta of the option using the Black-Scholes formula
    // Delta is the rate of change of the option price with respect to changes in the underlying
    // stock price. For a call option, delta ranges from 0 to 1, and for a put option, 
    // it ranges from -1 to 0.
    return normalCDF(d1());
}

double EuropeanOption::vega() const {
    // Calculate the vega of the option using the Black-Scholes formula
    // Vega measures the sensitivity of the option price to changes in the 
    // volatility of the underlying stock 
    // It represents the amount by which the option price is expected to change for a 1% change in volatility.
    // In layman terms:
    // Vega is highest for at-the-money options and decreases as the option becomes deep in-the
    // money or out-of-the-money. It is also higher for options with longer time to maturity.
    return stockPrice * DNormalCDF(d1()) * std::sqrt(timeToMaturity);
}

double EuropeanOption::gamma() const {
    // Calculate the gamma of the option using the Black-Scholes formula
    // Gamma measures the rate of change of delta with respect to changes in the underlying stock price.
    // It represents the curvature of the option price with respect to changes in the stock price.
    return DNormalCDF(d1()) / (stockPrice * volatility * std::sqrt(timeToMaturity));
}

double EuropeanOption::theta() const {
    // Calculate the theta of the option using the Black-Scholes formula
    // Theta measures the sensitivity of the option price to the passage of time, 
    // also known as time decay. It represents the amount by which the option price is expected to decrease 
    // for a one-year decrease in time to expiration.
    double d1_val = d1();
    double d2_val = d2();
    double term1 = - (stockPrice * DNormalCDF(d1_val) * volatility) / (2 * std::sqrt(timeToMaturity));
    double term2 = riskFreeRate * strikePrice * std::exp(-riskFreeRate * timeToMaturity) * normalCDF(d2_val);
    return term1 - term2;
}

double EuropeanOption::rho() const {
    // Calculate the rho of the option using the Black-Scholes formula
    // Rho measures the sensitivity of the option price to changes in the risk-free interest rate.
    // It represents the amount by which the option price is expected to change for a 1% change in the risk-free rate.
    double d2_val = d2();
    return strikePrice * timeToMaturity * std::exp(-riskFreeRate * timeToMaturity) * normalCDF(d2_val);
}

double EuropeanOption::callPrice() const {
    // Calculate the price of a European call option using the Black-Scholes formula
    // Call Price = S * N(d1) - K * e^(-rT) * N(d2)
    // In layman terms:
    // The call price is the present value of the expected payoff of the option at expiration.
    // N(d1) represents the probability that the option will be exercised, and N(d2) represents the
    // probability that the option will expire in-the-money. The first term represents the expected
    // value of the stock price at expiration, and the second term represents the present value of
    // the strike price that must be paid to exercise the option.
    double d1_val = d1();
    double d2_val = d2();
    return stockPrice * normalCDF(d1_val) - strikePrice * std::exp(-riskFreeRate * timeToMaturity) * normalCDF(d2_val);
}

double EuropeanOption::putPrice() const {
    // Calculate the price of a European put option using the Black-Scholes formula
    // Put Price = K * e^(-rT) * N(-d2) - S * N(-d1)
    // In layman terms:
    // The put price is the present value of the expected payoff of the option at expiration.
    // N(-d1) represents the probability that the option will be exercised, 
    // and N(-d2) represents the probability that the option will expire in-the-money. The first term represents
    // the present value of the strike price that can be received by exercising the option,
    // and the second term represents the expected value of the stock price at expiration.
    double d1_val = d1();
    double d2_val = d2();
    return strikePrice * std::exp(-riskFreeRate * timeToMaturity) * normalCDF(-d2_val) - stockPrice * normalCDF(-d1_val);
}

double EuropeanOption::monteCarloCallPrice(int numSimulations) const {
    // Calculate the price of a European call option using Monte Carlo simulation
    // In layman terms:
    // Monte Carlo simulation is a statistical method used to estimate the expected value of an option's
    // payoff by simulating a large number of possible future stock prices at expiration.
    // The average of these simulated payoffs is then discounted back to the present value 
    // to obtain the option price.
    // The more simulations you run, the more accurate the estimate will be, 
    // but it will also take longer to compute.

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

double EuropeanOption::monteCarloCallPriceAntiThetic(int numSimulations) const {
    // Calculate the price of a European call option using Monte Carlo simulation
    // In layman terms:
    // Monte Carlo simulation is a statistical method used to estimate the expected value of an option's
    // payoff by simulating a large number of possible future stock prices at expiration.
    // The average of these simulated payoffs is then discounted back to the present value 
    // to obtain the option price.
    // The more simulations you run, the more accurate the estimate will be, 
    // but it will also take longer to compute.


    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::normal_distribution<double> dist(0.0, 1.0);

    double sumPayoffs = 0.0;

    for (int i = 0; i < numSimulations; ++i) {
        double z = dist(gen);
        double S_T = stockPrice * std::exp((riskFreeRate - 0.5 * volatility * volatility) * timeToMaturity + volatility * std::sqrt(timeToMaturity) * z);
        double antiS_T = stockPrice * std::exp((riskFreeRate - 0.5 * volatility * volatility) * timeToMaturity + volatility * std::sqrt(timeToMaturity) * (-z));
        double payoff = std::max(S_T - strikePrice, 0.0);
        double antiPayoff = std::max(antiS_T - strikePrice, 0.0);
        sumPayoffs += (payoff + antiPayoff) / 2.0; // Use antithetic variates to reduce variance
    }
    double averagePayoff = sumPayoffs / numSimulations;
    return std::exp(-riskFreeRate * timeToMaturity) * averagePayoff;
}
#endif // EURPEANOPTION_HPP