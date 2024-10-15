#ifndef USER_H
#define USER_H

#include "mylib.h"
#include "randomizer.h"

// Class representing a user with a name, public key, and balance
class User {
private:
    std::string name;
    std::string public_key;
    long double balance;

public:
    // Default constructor
    User() = default;

    // Parameterized constructor
    User(const std::string& name, const std::string& publicKey, long double balance)
        : name(name), public_key(publicKey), balance(balance) {}

    // Method to update the balance
    void updateBalance(long double amount) {
        balance += amount;
    }

    // Copy constructor, move constructor, copy assignment, and move assignment
    User(const User& other) = default;
    User(User&& other) noexcept = default;
    User& operator=(const User& other) = default;
    User& operator=(User&& other) noexcept = default;

    // Getters
    long double getBalance() const { return balance; }
    const std::string& getPublicKey() const { return public_key; }
};

// Class to manage users, create them, and update their balance
class UserManager {
private:
    std::vector<User> users;

    // Method to create a user with random details
    User createUser() {
        std::string name = Random::randomString(10);
        std::string public_key = Random::randomString(50);
        double balance = Random::randomDouble(100, 1000000);
        return User(name, public_key, balance);
    }

public:
    // Constructor that creates a specified number of users
    UserManager(int numberOfUsers) {
        users.reserve(numberOfUsers);
        for (int i = 0; i < numberOfUsers; i++) {
            users.push_back(createUser());
        }
    }

    // Method to update the balance of a user identified by their public key
    void updateBalance(const std::string& publicKey, long double amount) {
        for (auto& user : users) {
            if (user.getPublicKey() == publicKey) {
                user.updateBalance(amount);
                return;
            }
        }
    }

    // Getter for the list of users
    const std::vector<User>& getUsers() const { return users; }

    // Method to find a user by their public key and return a pointer to the user object
    const User* findUser(const std::string& publicKey) const {
        for (const auto& user : users) {
            if (user.getPublicKey() == publicKey) {
                return &user;
            }
        }
        return nullptr;
    }
};

#endif
