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
        return keiciaIvesti(previousHash + merkleRoot + std::to_string(timestamp) + std::to_string(difficultyTarget) + std::to_string(nonce)); 

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
    unsigned int id;
    unsigned int blocksMined;
    std::atomic<bool> blockMined;  // Shared flag to indicate if the block is mined

    // Function for each thread to mine a block by trying different nonces
    Block mineBlockThread(Block block, unsigned int attempts, std::promise<Block>& resultPromise) {
        for (unsigned int i = 0; i < attempts && !blockMined; ++i) {
            unsigned int nonce = Random::randomUnsignedInt(0, UINT_MAX);
            block.setNonce(nonce);

            if (block.meetRequirements()) {
                blockMined = true;  // Set flag to true when block is mined
                resultPromise.set_value(block);  // Send mined block result
                return block;
            }
        }

        throw std::runtime_error("Failed to mine block in thread.");
    }

public:
    // Constructor
    Miner(unsigned int minerId) : id(minerId), blocksMined(0), blockMined(false) {}

    // Method to mine a block using multiple threads
    Block mineBlock(const std::string& prevHash, unsigned int difficultyTarget, const std::vector<Transaction>& transactions, unsigned int attempts, unsigned int numThreads) {
        Block block(prevHash, difficultyTarget, transactions);
        std::cout << "Starting multi-threaded mining process for Block by Miner ID " << id << std::endl;

        // Calculate attempts per thread
        unsigned int attemptsPerThread = attempts / numThreads;

        // Vector to store thread promises
        std::vector<std::future<Block>> futures;
        std::vector<std::thread> threads;

        // Launch threads to mine the block in parallel
        for (unsigned int i = 0; i < numThreads; ++i) {
            std::promise<Block> resultPromise;
            futures.push_back(resultPromise.get_future());
            threads.emplace_back(&Miner::mineBlockThread, this, block, attemptsPerThread, std::ref(resultPromise));
        }

        // Wait for any thread to successfully mine the block
        Block minedBlock = futures[0].get();
        for (auto& t : threads) {
            t.join();
        }

        blocksMined++;
        std::cout << "Block mined successfully by Miner " << id << " with hash: " << minedBlock.getHash() << std::endl;

        return minedBlock;
    }

    // Increment the number of blocks mined by the miner
    void incrementBlocksMined() { blocksMined++; }

    // Getters
    unsigned int getBlocksMined() const { return blocksMined; }
    unsigned int getID() const { return id; }
};

#endif
