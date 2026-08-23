#include <random>
#include <iostream>

int main(){
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::normal_distribution<double> dist(0.0, 1.0);
    double sum = 0.0;
    double num_samples = 100000; 
    double z; 

    for (int i = 0; i < num_samples; ++i) {
        z = dist(gen);
        sum += z;
    }
    double average = sum / num_samples;
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Average of " << num_samples << " samples: " << average << std::endl;
    return 0;
}