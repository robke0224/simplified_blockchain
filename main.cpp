#include <iostream>
#include <vector>
#include "blockchain.h"  

int main() {
    Blockchain blockchain;

    // Add transactions
    std::vector<Transaction> transactions;
    transactions.push_back(Transaction("Alice", "Bob", 50));
    transactions.push_back(Transaction("Bob", "Charlie", 20));

    // Mine a block with these transactions
    Block newBlock(blockchain.getLastBlock().calculateBlockHash(), transactions);
    newBlock.mineBlock(4);  // Set difficulty level

    blockchain.addBlock(newBlock);

    // Print the blockchain
    for (const Block& block : blockchain.chain) {
        std::cout << "Block Hash: " << block.calculateBlockHash() << std::endl;
    }

    return 0;
}
