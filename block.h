#include "lib.h"
#include "hash.h"
#include "transaction.h"

#pragma once

class Block {
private:
    int index;
    string previous_hash;
    string hash;
    int nonce;
    vector<Transaction> transactions;

public:
    Block(int index, const string &previous_hash, const vector<Transaction> &transactions)
        : index(index), previous_hash(previous_hash), transactions(transactions), nonce(0) {
        hash = calculateBlockHash();
    }

    string calculateBlockHash() {
        string blockData = to_string(index) + previous_hash + to_string(nonce);
        for (const auto &tx : transactions) {
            blockData += tx.getTransactionID();
        }
        return custom_hash(blockData);
    }

    string mineBlock(int difficulty) {
        string target(difficulty, '0');
        while (hash.substr(0, difficulty) != target) {
            nonce++;
            hash = calculateBlockHash();
        }
        return hash;
    }

    void display() const {
        cout << "Block Index: " << index << ", Previous Hash: " << previous_hash
                  << ", Hash: " << hash << ", Nonce: " << nonce << endl;
        for (const auto &tx : transactions) {
            tx.display();
        }
    }
};
