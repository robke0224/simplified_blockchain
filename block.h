#ifndef BLOCK_H
#define BLOCK_H

#include "lib.h"
#include "hash.h"
#include "transaction.h"

#define BLOCK_VERSION 1
const int NUM_THREADS = 4;
bool blockMinedGlobal = false;

string getCurrentTime() {
    // Get current time as time_t
    std::time_t now = std::time(nullptr);

    // Convert to tm struct for local time
    std::tm* localTime = std::localtime(&now);

    // Create a string stream to format the time
    std::stringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS

    // Return the formatted time as a string
    return ss.str();
}

class Block {
private:
    int index;
    string previousHash;
    string hash;
    unsigned int nonce;
    string timestamp;
    unsigned int version;
    unsigned int difficulty;
    vector<Transaction> transactions;

    bool Mined = false; // Added flag to check if the block is mined
    bool Valid = false; // Added flag to mark block validity

public:
    Block() {};

    // Constructor that takes previousHash, difficulty, and transactions
    Block(int index, const string& previousHash, const vector<Transaction>& txs, unsigned int difficulty)
        : index(index), previousHash(previousHash), transactions(txs), difficulty(difficulty), version(BLOCK_VERSION), nonce(0) {
        timestamp = getCurrentTime(); // Get current time for timestamp
        hash = calculateBlockHash();  // Calculate initial hash
    }

    // Method to calculate block hash
    string calculateBlockHash() const {
        string blockData = to_string(version) + to_string(index) + previousHash + to_string(nonce) + timestamp;
        for (const auto& tx : transactions) {
            blockData += tx.GetID(); // Append transaction IDs
        }
        return custom_hash(blockData); // Hash the block data
    }

    // Mining function that adjusts the nonce until the hash meets the difficulty target
  void Mine() {
    nonce = 0; // Start nonce at 0
    string guessHash = calculateBlockHash(); // Calculate initial hash

    // Mine until a valid hash is found or blockMinedGlobal is set
    while (!Mined && !blockMinedGlobal) {
        if (!hashMeetDifficultyTarget(guessHash)) {
            nonce++;  // Increment nonce by 1
            guessHash = calculateBlockHash(); // Recalculate hash with new nonce
        } else {
            hash = guessHash;     // Set the valid hash
            Mined = true;         // Mark the block as mined
            #pragma omp critical
            {
                blockMinedGlobal = true; // Signal that the block is mined
            }
        }
    }
}



    // Method to check if the hash meets the difficulty target
    bool hashMeetDifficultyTarget(const string& guessHash) const {
        return guessHash.substr(0, difficulty) == string(difficulty, '0');
    }

    // Method to set the block as valid
    void setValidBlock(bool valid) {
        Valid = valid;
    }

    // Display method to print block details
   void display(std::ostream& os) const {
    os << "=============================================================" << endl;
    os << "                      Block " << index << " Details" << endl;
    os << "=============================================================" << endl;
    os << "| Index:             " << index << endl;
    os << "| Previous Hash:     " << previousHash << endl;
    os << "| Hash:              " << hash << endl;
    os << "| Nonce:             " << nonce << endl;
    os << "| Timestamp:         " << timestamp << endl;
    os << "| Version:           " << version << endl;
    os << "| Difficulty:        " << difficulty << endl;
    os << "| Number of TXs:     " << transactions.size() << endl;

    os << "-------------------------------------------------------------" << endl;
    os << "| Transactions:" << endl;
    if (transactions.empty()) {
        os << "|    No transactions in this block." << endl;
    } else {
        for (const auto& tx : transactions) {
            os << "|    Transaction ID: " << tx.GetID() << endl;
        }
    }
    os << "=============================================================" << endl << endl;
}


    // Getters for block attributes
    int getIndex() const { return index; }
    string getPreviousHash() const { return previousHash; }
    string getHash() const { return hash; }
    unsigned int getNonce() const { return nonce; }
    string getTimestamp() const { return timestamp; }
    unsigned int getVersion() const { return version; }
    unsigned int getDifficulty() const { return difficulty; }
    vector<Transaction>& getTransactions() { return transactions; }
    const vector<Transaction>& getTransactions() const { return transactions; }
    bool getMined() const { return Mined; }
    bool getValid() const { return Valid; }
};

#endif
