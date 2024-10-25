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
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(100, 1000000);
    return distr(gen);
}

int main() {
    bool endProgram = false;
    vector<User> users;
    vector<Transaction> transactions;
    Blockchain blockchain(4);  // Initialize the blockchain with difficulty level 4

    while (!endProgram) {
        int userChoice;
        cout << "Choose an option: \n";
        cout << "1. Create Users \n";
        cout << "2. Create Transactions \n";
        cout << "3. Process Transactions into Blocks \n";
        cout << "4. Display Blockchain \n";
        cout << "5. End Program \n";
        cin >> userChoice;

        switch (userChoice) {
            case 1: {
                // Create users
                cout << "Starting user creation..." << endl;
                users.clear();  // Clear previous users if any

                for (int i = 0; i < 1000; ++i) {
                    cout << "Creating user " << i << endl;
                    int initialBalance = generateBalance();
                    User user("User" + to_string(i), custom_hash("User" + to_string(i)), initialBalance);
                    users.push_back(user);

                    // Create initial UTXO for each user
                    UTXO initialUTXO(user.getPublicKey(), initialBalance);
                    user.addUTXO(initialUTXO);

                    cout << "Displaying user info for User" << i << endl;
                    user.display();  // Display user info
                }
                cout << "Users created successfully!" << endl;
                break;
            }
            case 2: {
                // Create transactions
                cout << "Starting transaction creation..." << endl;
                transactions.clear();  // Clear previous transactions if any

                for (int i = 0; i < 10000; ++i) {
                    cout << "Creating transaction " << i << endl;

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

                    cout << "===================================" << endl;
                    cout << "|        Transaction Details      |" << endl;
                    cout << "===================================" << endl;
                    cout << "| Sender:   " << sender.getPublicKey() << endl;
                    cout << "| Receiver: " << receiver.getPublicKey() << endl;
                    cout << "| Amount:   " << amount << endl;
                    cout << "===================================" << endl;

                    // Collect sender's UTXOs to cover the amount
                    vector<UTXO> selectedInputs = sender.selectUTXOs(amount);
                    if (selectedInputs.empty()) {
                        cout << "Skipping transaction " << i << ": Sender does not have enough balance." << endl;
                        continue;
                    }

                    // Create outputs: One for the receiver, one for the change (if any)
                    vector<UTXO> outputs;
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
                    receiver.addUTXO(outputs[0]);

                    if (outputs.size() > 1) {
                        sender.addUTXO(outputs[1]);  // Sender gets the change UTXO
                    }

                    cout << "Transaction " << i << " created and displayed." << endl;
                    tx.display();  // Display transaction details
                }
                cout << "All transactions created!" << endl;
                break;
            }
            case 3: {
                // Process transactions into blocks
                if (transactions.empty()) {
                    cout << "No transactions to process!" << endl;
                    break;
                }

                // Start measuring block processing time
                auto start = high_resolution_clock::now();
                size_t blockCount = 0;
                size_t transactionCount = 0;

                cout << "Starting to process transactions into blocks..." << endl;

                while (!transactions.empty()) {
                    size_t blockSize = std::min((size_t)100, transactions.size());  // Each block can contain up to 100 transactions
                    vector<Transaction> blockTxs(transactions.begin(), transactions.begin() + blockSize);

                    transactions.erase(transactions.begin(), transactions.begin() + blockSize);

                    auto block_start = high_resolution_clock::now();

                    // Adding block to the blockchain
                    try {
                        blockchain.addBlock(blockTxs);
                    } catch (const exception& e) {
                        cerr << "Error while adding block: " << e.what() << endl;
                        break;
                    }

                    transactionCount += blockSize;

                    if (transactionCount % 25 == 0) {
                        cout << transactionCount << " transactions processed so far." << endl;
                    }

                    auto block_duration = duration_cast<milliseconds>(high_resolution_clock::now() - block_start);
                    cout << "Block " << ++blockCount << " processed in " << block_duration.count() << " ms with " << blockSize << " transactions." << endl;
                }

                auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
                cout << "Total block processing took: " << duration.count() << " milliseconds" << endl;
                cout << "All transactions processed into blocks." << endl;
                break;
            }
            case 4: {
                // Display the blockchain
                blockchain.displayBlockchain();
                break;
            }
            case 5: {
                // End the program
                endProgram = true;
                cout << "Ending program..." << endl;
                break;
            }
            default: {
                cout << "Invalid option. Please choose again." << endl;
            }
        }
    }

    return 0;
}
