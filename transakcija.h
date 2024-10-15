#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "mylib.h"
#include "functions.h"
#include "naudotojas.h"

// Class representing a transaction between a sender and a receiver
class Transaction {
private:
    std::string id;
    std::string sender;
    std::string receiver;
    double sentAmount;
    std::string timeStamp;

public:
    // Constructors
    Transaction() = default;

    // Parameterized constructor that initializes a transaction and calculates its hash ID
    Transaction(const std::string& from, const std::string& to, double amount, const std::string& time)
        : sender(from), receiver(to), sentAmount(amount), timeStamp(time) {
        id = getHashString(from + to + std::to_string(amount));
    }

    // Getters
    std::string getSender() const { return sender; }
    std::string getReceiver() const { return receiver; }
    std::string getID() const { return id; }
    long double getAmount() const { return sentAmount; }
    std::string getTimeStamp() const { return timeStamp; }

    // Converts the transaction details to a string for display or logging
    std::string toString() const {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << sentAmount;
        return "Transaction: " + id + "\nFrom: " + sender + "\nTo: " + receiver + "\nAmount: " + ss.str() + "\nAT: " + timeStamp + "\n";
    }
};

// Class responsible for building a list of transactions
class TransactionBuilder {
private:
    std::vector<Transaction> transactions;

    // Helper method to create a new transaction from random users
    Transaction newTransaction(const std::vector<User>& users) {
        std::string recipientPublicKey = getRandomPublicKey(users);
        std::string senderPublicKey;
        do {
            senderPublicKey = getRandomPublicKey(users);
        } while (senderPublicKey == recipientPublicKey); // Ensure sender and receiver are not the same

        double amount = Random::randomDouble(100, 100000);
        amount = std::floor(amount * 100.0 + 0.5) / 100.0; // Round to two decimal places

        std::string timeStamp = getCurrentTime(); // Get current timestamp
        return Transaction(senderPublicKey, recipientPublicKey, amount, timeStamp);
    }

    // Method to retrieve a random user's public key
    std::string getRandomPublicKey(const std::vector<User>& users) {
        if (users.empty()) {
            throw std::runtime_error("No users in the system");
        }
        int randomIndex = Random::randomUnsignedInt(0, users.size() - 1);
        return users[randomIndex].getPublicKey();
    }

    // Helper method to get the current time as a formatted string
    std::string getCurrentTime() {
        auto time = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(time);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %X");
        return ss.str();
    }

public:
    // Constructor that generates a specified number of transactions
    TransactionBuilder(unsigned int n, UserManager& userManager) {
        auto users = userManager.getUsers();
        std::ofstream outFile("transactions.txt", std::ios::out | std::ios::app);
        if (!outFile) {
            throw std::runtime_error("Unable to open file for writing transactions.");
        }

        unsigned int validTransactionsCount = 0;
        for (unsigned int i = 0; i < n; i++) {
            Transaction trans = newTransaction(users);
            std::string validationMessage;
            if (validateTransaction(trans, userManager, validationMessage)) {
                transactions.push_back(trans);
                validTransactionsCount++;
                outFile << "Valid transaction added: " << trans.getID() << std::endl;
                outFile << trans.toString() << std::endl;
            } else {
                outFile << "Invalid transaction detected: " << trans.getID() << std::endl;
                outFile << validationMessage << std::endl;
            }
        }

        outFile << "Total number of valid transactions added to the pool: " << validTransactionsCount << std::endl;
        outFile.close();
    }

    // Getter for the list of transactions
    const std::vector<Transaction>& getTransactions() const { return transactions; }

    // Method to validate a transaction's integrity and legitimacy
    bool validateTransaction(const Transaction& transaction, const UserManager& userManager, std::string& message) {
        const User* sender = userManager.findUser(transaction.getSender());
        if (!sender) {
            message = "Sender not found: " + transaction.getSender();
            return false;
        }

        std::string recalculatedHash = getHashString(transaction.getSender() + transaction.getReceiver() + std::to_string(transaction.getAmount()));

        if (transaction.getID() != recalculatedHash) {
            message = "Transaction hash mismatch: Expected hash = " + recalculatedHash + ", Actual hash = " + transaction.getID();
            return false;
        }

        if (sender->getBalance() < transaction.getAmount()) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << sender->getBalance();
            message = "Insufficient funds for transaction: " + transaction.getID() + ". Balance: " + ss.str() + ", Amount to send: " + std::to_string(transaction.getAmount());
            return false;
        }

        message.clear();
        return true;
    }
};

#endif
