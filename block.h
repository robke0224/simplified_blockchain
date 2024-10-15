#ifndef BLOCK_H
#define BLOCK_H

#include "mylib.h"
#include "merkletree.h"
#include "transakcija.h"
#include <thread>
#include <future>
#include <atomic>

#define BLOCK_VERSION 1

class Block {
private:
    unsigned int version;
    std::string previousHash;
    std::string merkleRoot;
    unsigned long long int timestamp;
    unsigned int difficultyTarget;
    unsigned int nonce;
    std::vector<Transaction> transactions;

public:
    // Constructor
    Block(const std::string& prevHash, unsigned int diffTarget, const std::vector<Transaction>& trans)
        : version(BLOCK_VERSION), previousHash(prevHash), difficultyTarget(diffTarget), transactions(trans), nonce(0) {

        timestamp = static_cast<unsigned long long int>(std::time(nullptr));
        MerkleTree merkleTree(transactions); // Create Merkle tree from transactions
        merkleRoot = merkleTree.createMerkleRoot(); // Generate Merkle root
        nonce = Random::randomUnsignedInt(0, UINT_MAX); // Initialize with random nonce

        // Log block information to file
        logBlockInfo();
    }

    // Logging block information to a file
    void logBlockInfo() const {
        std::ofstream outFile("blocks_info.txt", std::ios::app);
        if (outFile) {
            outFile << toString() << std::endl;
        } else {
            std::cerr << "Unable to open file for writing block information." << std::endl;
        }
    }

    // Getters
    std::string getPreviousHash() const { return previousHash; }
    std::string getHash() const { 
        return keiciaIvesti(previousHash + merkleRoot + std::to_string(timestamp) + std::to_string(difficultyTarget) + std::to_string(nonce), 'j', 2, true, false); 

    }
    unsigned long long int getTimestamp() const { return timestamp; }
    unsigned int getDifficultyTarget() const { return difficultyTarget; }
    unsigned int getNonce() const { return nonce; }
    const std::vector<Transaction>& getTransactions() const { return transactions; }
    std::string getMerkleRoot() const { return merkleRoot; }

    // Setter for nonce
    void setNonce(unsigned int newNonce) { nonce = newNonce; }

    // Check if the block's hash meets the difficulty requirements
    bool meetRequirements() const {
        return getHash().substr(0, difficultyTarget) == std::string(difficultyTarget, '0');
    }

    // Convert block information to a string for display or logging
    std::string toString() const {
        std::ostringstream ss;
        ss << "Block Information\n"
           << "Version: " << version << "\n"
           << "Previous Hash: " << previousHash << "\n"
           << "Merkle Root: " << merkleRoot << "\n"
           << "Timestamp: " << timestamp << "\n"
           << "Difficulty Target: " << difficultyTarget << "\n"
           << "Nonce: " << nonce << "\n"
           << "Number of Transactions: " << transactions.size() << "\n";
        return ss.str();
    }
};

// Miner class that represents a miner attempting to mine blocks with multi-threading
class Miner {
private:
    int id;  // Unique miner ID
    std::atomic<bool> blockMined;  // Flag to indicate if the block is mined
    int blocksMined;  // Counter for how many blocks this miner has mined

public:
    // Constructor that initializes the miner with an ID
    Miner(int minerID) : id(minerID), blockMined(false), blocksMined(0) {}

    // Deleted copy constructor and assignment operator
    Miner(const Miner&) = delete;
    Miner& operator=(const Miner&) = delete;

    // Allow move constructor and move assignment operator
    Miner(Miner&& other) noexcept : id(other.id), blockMined(other.blockMined.load()), blocksMined(other.blocksMined) {
        other.blocksMined = 0;  // Reset other miner's block count after moving
    }

    Miner& operator=(Miner&& other) noexcept {
        if (this != &other) {
            id = other.id;
            blockMined.store(other.blockMined.load());
            blocksMined = other.blocksMined;
            other.blocksMined = 0;
        }
        return *this;
    }

    // Method to simulate mining a block
    Block mineBlock(const std::string& prevHash, unsigned int difficultyTarget, const std::vector<Transaction>& transactions, unsigned int attempts, unsigned int numThreads) {
        // Logic to mine the block, simplified here:
        for (unsigned int i = 0; i < attempts; ++i) {
            // Simulated mining logic, typically you'd calculate a hash here
            if (i % difficultyTarget == 0) {
                blockMined.store(true);
                std::cout << "Miner " << id << " mined a block!" << std::endl;
                return Block(prevHash, difficultyTarget, transactions);  // Return a successfully mined block
            }
        }
        throw std::runtime_error("Mining failed after max attempts");
    }

    // Increment the number of blocks mined
    void incrementBlocksMined() {
        ++blocksMined;
    }

    // Getter for the miner's ID
    int getID() const {
        return id;
    }

    // Getter for the number of blocks this miner has mined
    int getBlocksMined() const {
        return blocksMined;
    }
};

#endif
