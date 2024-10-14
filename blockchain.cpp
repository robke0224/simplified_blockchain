#include <string> 

using namespace std;

class Transaction {
public:
    string transaction_id;
    string sender;
    string recipient;
    double amount;
    
    Transaction(string sender, string recipient, double amount);
    string calculateTransactionHash() const;
};

class Block {
public:
    string prev_block_hash;
    time_t timestamp;
    string merkle_root_hash;
    int nonce;
    vector<Transaction> transactions;

    Block(string prev_block_hash, vector<Transaction> transactions);
    string calculateBlockHash() const;
    void mineBlock(int difficulty);
};

class Blockchain {
public:
    vector<Block> chain;
    Blockchain();

    void addBlock(Block newBlock);
    Block getLastBlock() const;
};

Block Blockchain::createGenesisBlock() {
    std::vector<Transaction> transactions;
    return Block("0", transactions);
};

std::string Block::calculateBlockHash() const {
    std::stringstream ss;
    ss << prev_block_hash << timestamp << nonce << merkle_root_hash;
    std::string block_data = ss.str();
   //////cia mano hashas turi eit
};


std::vector<Transaction> transactionPool;  // Stores pending transactions


void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0');
    do {
        nonce++;
        block_hash = calculateBlockHash();
    } while (block_hash.substr(0, difficulty) != target);
};
