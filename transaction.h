#include "lib.h"
#include "hash.h"
#include <vector>
#include <iostream>
#include <string>

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

    // Getter for the transaction ID
    string GetID() const {
        return transaction_id;
    }

    // Display transaction details
    void display() const {
        cout << "=================================================" << endl;
        cout << "|               Transaction Details             |" << endl;
        cout << "=================================================" << endl;
        cout << "| Transaction ID: " << transaction_id << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "| Inputs: " << endl;
        if (inputs.empty()) {
            cout << "|    No inputs available." << endl;
        } else {
            for (const auto& input : inputs) {
                cout << "|    ";
                input.display();  // Assuming input.display() prints on a single line
            }
        }
        cout << "-------------------------------------------------" << endl;
        cout << "| Outputs: " << endl;
        if (outputs.empty()) {
            cout << "|    No outputs available." << endl;
        } else {
            for (const auto& output : outputs) {
                cout << "|    ";
                output.display();  // Assuming output.display() prints on a single line
            }
        }
        cout << "=================================================" << endl << endl;
    }
};
