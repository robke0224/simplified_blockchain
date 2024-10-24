#include "lib.h"
#include "hash.h"

#pragma once

using namespace std;

// UTXO Structure
struct UTXO {
    string utxo_id;  // Unique ID for the UTXO (generated via hash)
    string owner;    // Owner of the UTXO
    int amount;      // Amount of the UTXO

    UTXO(const string& owner, int amount)
        : owner(owner), amount(amount) {
        utxo_id = custom_hash(owner + to_string(amount));
    }
    
    void display() const {
        cout << "UTXO ID: " << utxo_id 
             << ", Owner: " << owner 
             << ", Amount: " << amount << endl;
    }
};

// Transaction Class with UTXO Model
class Transaction {
private:
    string transaction_id;
    vector<UTXO> inputs;   // List of UTXOs used as inputs
    vector<UTXO> outputs;  // List of UTXOs generated as outputs

public:
    // Constructor for a transaction
    Transaction(const vector<UTXO>& inputs, const vector<UTXO>& outputs)
        : inputs(inputs), outputs(outputs) {
        // Concatenate all input UTXO IDs and output details to create a transaction hash
        string data;
        for (const auto& input : inputs) {
            data += input.utxo_id;
        }
        for (const auto& output : outputs) {
            data += output.owner + to_string(output.amount);
        }
        transaction_id = custom_hash(data);
    }

    string getTransactionID() const {
        return transaction_id;
    }

    // Display transaction details
    void display() const {
        cout << "Transaction ID: " << transaction_id << endl;
        cout << "Inputs: " << endl;
        for (const auto& input : inputs) {
            input.display();
        }
        cout << "Outputs: " << endl;
        for (const auto& output : outputs) {
            output.display();
        }
    }
};
