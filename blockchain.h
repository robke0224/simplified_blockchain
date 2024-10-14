#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>


class Transaction {
public:
    std::string transaction_id;
    std::string sender;
    std::string recipient;
    double amount;

    Transaction(std::string sender, std::string recipient, double amount);
    std::string calculateTransactionHash() const;
};

class Block {
public:
    std::string prev_block_hash;
    time_t timestamp;
    std::string merkle_root_hash;
    int nonce;
    std::vector<Transaction> transactions;
    
    // Constructor
    Block(std::string prev_block_hash, std::vector<Transaction> transactions);
    
    // Methods
    std::string calculateBlockHash() const;
    void mineBlock(int difficulty);
};

class Blockchain {
public:
    std::vector<Block> chain;

    // Constructor
    Blockchain();

    // Methods
    void addBlock(Block newBlock);
    Block getLastBlock() const;
    Block createGenesisBlock();
};

#endif // BLOCKCHAIN_H
