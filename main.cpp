#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "block.h"
#include "functions.h"
#include "merkletree.h"
#include "mylib.h"
#include "naudotojas.h"
#include "randomizer.h"
#include "transakcija.h"
#include <chrono>

// Function to print a transaction in detail
void printTransaction(const Transaction& transaction) {
    std::cout << "Transaction ID: " << transaction.getID() << std::endl;
    std::cout << "Sender: " << transaction.getSender() << std::endl;
    std::cout << "Receiver: " << transaction.getReceiver() << std::endl;
    std::cout << "Amount: " << transaction.getAmount() << std::endl;
    std::cout << "Timestamp: " << transaction.getTimeStamp() << std::endl;  // Corrected to getTimeStamp
    std::cout << "---------------------------------" << std::endl;
}

// Function to print block details
void printBlock(const Block& block) {
    std::cout << "Block Hash: " << block.getHash() << std::endl;  // Assuming getHash is the correct method
    std::cout << "Previous Block Hash: " << block.getPreviousHash() << std::endl;
    std::cout << "Block Transactions:" << std::endl;

    for (const auto& transaction : block.getTransactions()) {
        printTransaction(transaction);
    }

    std::cout << "---------------------------------" << std::endl;
}

int main() {
    // Initialize the user manager with a number of users (e.g., 10 users)
    int numberOfUsers = 10;

    // Create a list of users with names, public keys, and initial balances
    std::vector<User> users;
    for (int i = 0; i < numberOfUsers; ++i) {
        std::string name = "User" + std::to_string(i);
        std::string publicKey = "PublicKey" + std::to_string(i);  // Generate a dummy public key
        long double balance = 1000.0;  // Default balance for all users
        users.push_back(User(name, publicKey, balance));  // Corrected to match the 3-parameter constructor
    }

    // Vector to hold blockchain blocks
    std::vector<Block> blockchain;

    // Initialize a previous hash (for the first block, it can be "0")
    std::string previousBlockHash = "0";

    // Create a new block with a difficulty target and an empty list of transactions
    unsigned int difficultyTarget = 2;
    std::vector<Transaction> transactions;

    // Generate random transactions
    int numTransactions = 5;
    for (int i = 0; i < numTransactions; ++i) {
        // Select random sender and receiver from users
        std::string sender = users[i % numberOfUsers].getPublicKey();   // Using publicKey as sender identifier
        std::string receiver = users[(i + 1) % numberOfUsers].getPublicKey();  // Using publicKey as receiver identifier
        double amount = (rand() % 100) + 1;  // Random amount between 1 and 100

        // Create a timestamp for the transaction
        std::string timestamp = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

        // Create the transaction
        Transaction transaction(sender, receiver, amount, timestamp);

        // Assuming a simple validation logic for the transaction (you can replace it with your logic)
        if (amount <= users[i % numberOfUsers].getBalance()) {
            transactions.push_back(transaction);
            std::cout << "Added transaction " << i + 1 << " to the block." << std::endl;
        } else {
            std::cerr << "Transaction failed: Insufficient funds." << std::endl;
        }
    }

    // Create the block with the transactions, previous hash, and difficulty target
    Block currentBlock(previousBlockHash, difficultyTarget, transactions);

    // Add the block to the blockchain
    blockchain.push_back(currentBlock);

    // Print the created block
    std::cout << "Block successfully created!" << std::endl;
    printBlock(currentBlock);

    return 0;
}
