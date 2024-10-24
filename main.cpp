#include "user.h"
#include "transaction.h"
#include "blockchain.h"
#include <chrono>
#include <random>
#include <vector>
#include <iostream>

using namespace std::chrono;

// Helper function to generate a random balance
int generateBalance() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(100, 1000000);
    return distr(gen);
}

int main() {
    bool endProgram = false;

    while (!endProgram) {
        std::cout << "Starting user creation..." << std::endl;

        // Create users
        std::vector<User> users;
        for (int i = 0; i < 1000; ++i) {
            std::cout << "Creating user " << i << std::endl;
            int initialBalance = generateBalance();
            User user("User" + std::to_string(i), custom_hash("User" + std::to_string(i)), initialBalance);
            users.push_back(user);

            // Create initial UTXO for each user
            UTXO initialUTXO(user.getPublicKey(), initialBalance);
            user.addUTXO(initialUTXO);

            std::cout << "Displaying user info for User" << i << std::endl;
            user.display();  // Display user info
        }

        std::cout << "Users created successfully!" << std::endl;

        // Create blockchain with difficulty level 4
        Blockchain blockchain(4);
        std::cout << "Blockchain created with difficulty level 4." << std::endl;

        // Create transactions
        std::vector<Transaction> transactions;
        std::cout << "Starting transaction creation..." << std::endl;

        for (int i = 0; i < 10000; ++i) {
            std::cout << "Creating transaction " << i << std::endl;

            // Randomly select sender and receiver
            int senderIndex = rand() % users.size();
            int receiverIndex;
            do {
                receiverIndex = rand() % users.size();
            } while (senderIndex == receiverIndex);

            User& sender = users[senderIndex];
            User& receiver = users[receiverIndex];

            // Determine the amount to send
            int amount = rand() % 10000 + 1;
            std::cout << "Sender: " << sender.getPublicKey() << ", Receiver: " << receiver.getPublicKey() << ", Amount: " << amount << std::endl;

            // Collect sender's UTXOs to cover the amount
            std::vector<UTXO> selectedInputs = sender.selectUTXOs(amount);
            if (selectedInputs.empty()) {
                std::cout << "Skipping transaction " << i << ": Sender does not have enough balance." << std::endl;
                continue;  // If sender doesn't have enough balance, skip the transaction
            }

            // Create outputs: One for the receiver, one for the change (if any)
            std::vector<UTXO> outputs;
            outputs.emplace_back(receiver.getPublicKey(), amount);

            // Calculate total input and change
            int totalInput = 0;
            for (const auto& input : selectedInputs) {
                totalInput += input.amount;
            }
            if (totalInput > amount) {
                int change = totalInput - amount;
                outputs.emplace_back(sender.getPublicKey(), change);
            }

            // Create the transaction
            Transaction tx(selectedInputs, outputs);
            transactions.push_back(tx);

            // Update users' UTXO pools
            sender.removeUTXOs(selectedInputs);
            receiver.addUTXO(outputs[0]);  // Receiver gets the first output

            if (outputs.size() > 1) {
                sender.addUTXO(outputs[1]);  // Sender gets the change UTXO
            }

            std::cout << "Transaction " << i << " created and displayed." << std::endl;
            tx.display();  // Display transaction details
        }

        std::cout << "All transactions created!" << std::endl;
///////////////////////////////////////////////////////////////////////////////////////////////////////
     // Start measuring block processing time
auto start = high_resolution_clock::now();

try {
    std::cout << "Starting to process transactions into blocks..." << std::endl;
    size_t blockCount = 0;
    size_t transactionCount = 0;

    while (!transactions.empty()) {
        size_t blockSize = std::min((size_t)100, transactions.size());
        std::vector<Transaction> blockTxs;
        blockTxs.reserve(blockSize);
        blockTxs.assign(transactions.begin(), transactions.begin() + blockSize);

        transactions.erase(transactions.begin(), transactions.begin() + blockSize);

        auto block_start = high_resolution_clock::now();

        // Adding block to the blockchain with exception handling
        try {
            blockchain.addBlock(blockTxs);
        } catch (const std::exception& e) {
            std::cerr << "Error while adding block: " << e.what() << std::endl;
            break;  // Exit the loop if there is an error adding the block
        }

        transactionCount += blockSize;

        if (transactionCount % 25 == 0) {
            std::cout << transactionCount << " transactions processed so far." << std::endl;
        }

        auto block_duration = duration_cast<milliseconds>(high_resolution_clock::now() - block_start);
        std::cout << "Block " << ++blockCount << " processed in " << block_duration.count() << " ms with " << blockSize << " transactions." << std::endl;
    }

} catch (const std::exception& e) {
    std::cerr << "Unexpected error during block processing: " << e.what() << std::endl;
}

auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
std::cout << "Total block processing took: " << duration.count() << " milliseconds" << std::endl;

std::cout << "All transactions processed into blocks." << std::endl;

blockchain.displayBlockchain();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Ask the user if they want to end the program
        char userInput;
        std::cout << "Do you want to end the program? (y/n): ";
        std::cin >> userInput;

        if (userInput == 'y' || userInput == 'Y') {
            endProgram = true;
            std::cout << "Ending program..." << std::endl;
        } else {
            std::cout << "Restarting the process..." << std::endl;
        }
    }

    return 0;
}
