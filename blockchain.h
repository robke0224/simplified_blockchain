#include "lib.h"
#include "block.h"

class Blockchain {
private:
    vector<Block> chain;
    int difficulty;

public:
    Blockchain(int difficulty) : difficulty(difficulty) {
        // Create the genesis block
        vector<Transaction> genesisTransactions;
        Block genesisBlock(0, "0000", genesisTransactions);
        chain.push_back(genesisBlock);
    }

    void addBlock(const std::vector<Transaction> &transactions) {
        Block newBlock(chain.size(), chain.back().calculateBlockHash(), transactions);
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }

    void displayBlockchain() const {
        for (const auto &block : chain) {
            block.display();
        }
    }
};
