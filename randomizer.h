#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include "mylib.h"
#include <random>
#include <string>
#include <stdexcept>

// Random class containing static methods for generating random values of various types.
class Random {
private:
    static thread_local std::mt19937 generator;

public:
    // Initialize the random number generator with a seed based on std::random_device.
    static void init() {
        std::random_device rd;
        generator.seed(rd());
    }

    // Initialize the random number generator with a custom seed.
    static void init(unsigned int seed) {
        generator.seed(seed);
    }

    // Generates a random string of a given length using a default or custom character set.
    static std::string randomString(size_t length, const std::string& characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") {
        if (characters.empty()) {
            throw std::invalid_argument("Character set cannot be empty.");
        }

        std::uniform_int_distribution<> dist(0, characters.size() - 1);
        std::string random_string;
        random_string.reserve(length);

        for (size_t i = 0; i < length; ++i) {
            random_string += characters[dist(generator)];
        }

        return random_string;
    }

    // Generates a random double value between min and max.
    static double randomDouble(double min, double max) {
        if (min > max) {
            throw std::invalid_argument("min should be <= max");
        }

        std::uniform_real_distribution<double> dist(min, max);
        return dist(generator);
    }

    // Generates a random integer value between min and max.
    static int randomInt(int min, int max) {
        if (min > max) {
            throw std::invalid_argument("min should be <= max");
        }

        std::uniform_int_distribution<> dist(min, max);
        return dist(generator);
    }

    // Generates a random unsigned integer value between min and max.
    static unsigned int randomUnsignedInt(unsigned int min, unsigned int max) {
        if (min > max) {
            throw std::invalid_argument("min should be <= max");
        }

        std::uniform_int_distribution<unsigned int> dist(min, max);
        return dist(generator);
    }
};

// Initialize the static thread-local generator with a non-deterministic seed.
thread_local std::mt19937 Random::generator(std::random_device{}());

#endif
