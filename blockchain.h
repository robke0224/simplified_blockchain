#include "lib.h"
#include "block.h"
#include <vector>

class Blockchain {
private:
    vector<Block> chain;
    int difficulty;

public:
    // Constructor initializing the blockchain with difficulty and creating the genesis block
    Blockchain(int difficulty) : difficulty(difficulty) {
        // Create the genesis block
        vector<Transaction> genesisTransactions; // Initialize an empty transaction list
        Block genesisBlock(0, "0000", genesisTransactions, difficulty); // Create the genesis block
        genesisBlock.Mine(); // Mine the genesis block using the Mine() method
        chain.push_back(genesisBlock); // Add the mined genesis block to the chain
    }

    // Add a new block to the blockchain
    void addBlock(const vector<Transaction> &transactions) {
        // Create a new block with index as the current size of the chain, the previous block's hash, and difficulty
        Block newBlock(chain.size(), chain.back().getHash(), transactions, difficulty); 
        newBlock.Mine(); // Mine the block using the Mine() method
        chain.push_back(newBlock); // Add the mined block to the chain
    }

    // Display all the blocks in the blockchain
    void displayBlockchain() const {
        for (const auto &block : chain) {
            block.display(std::cout); // Display each block's details
        }
    }

    // Get the latest block in the chain
    Block getLatestBlock() const {
        return chain.back();
    }

    // New method: Get all blocks in the chain
    const vector<Block>& getBlocks() const {
        return chain;
    }
};
