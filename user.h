#include "lib.h"
#include "transaction.h"  // Include UTXO definition


class User {
private:
    string name;
    string public_key;
    std::vector<UTXO> utxos;  // List of UTXOs owned by the user

public:
    // Constructor
    User(const string &name, const string &public_key, int initial_balance)
        : name(name), public_key(public_key) {
        // Create an initial UTXO for the user with the initial balance
        UTXO initial_utxo(public_key, initial_balance);
        utxos.push_back(initial_utxo);
    }

    // Get the public key of the user
    string getPublicKey() const {
        return public_key;
    }

    // Calculate the total balance by summing all UTXO amounts
    int getBalance() const {
        int balance = 0;
        for (const auto& utxo : utxos) {
            balance += utxo.amount;
        }
        return balance;
    }

    // Add a new UTXO to the user's UTXO set
    void addUTXO(const UTXO& utxo) {
        utxos.push_back(utxo);
    }

    // Remove UTXOs that have been spent in a transaction
    void removeUTXOs(const std::vector<UTXO>& spent_utxos) {
        for (const auto& spent : spent_utxos) {
            utxos.erase(std::remove_if(utxos.begin(), utxos.end(),
                [&spent](const UTXO& utxo) {
                    return utxo.utxo_id == spent.utxo_id;
                }), utxos.end());
        }
    }

    // Select UTXOs to cover a given amount for a transaction
    std::vector<UTXO> selectUTXOs(int amount) {
        std::vector<UTXO> selected_utxos;
        int total = 0;

        // Select UTXOs until the total amount is greater than or equal to the required amount
        for (const auto& utxo : utxos) {
            selected_utxos.push_back(utxo);
            total += utxo.amount;
            if (total >= amount) {
                break;
            }
        }

        // If total selected UTXOs cover the amount, return them, otherwise return an empty list
        if (total >= amount) {
            return selected_utxos;
        } else {
            return {};  // Not enough balance
        }
    }

    // Display user info and their UTXOs
   void display() const {
    cout << "===========================================" << endl;
    cout << "|              User Information           |" << endl;
    cout << "===========================================" << endl;
    cout << "| Name:       " << name << endl;
    cout << "| Public Key: " << public_key << endl;
    cout << "| Balance:    " << getBalance() << endl;
    cout << "-------------------------------------------" << endl;
    cout << "| UTXOs: " << (utxos.empty() ? "No UTXOs available" : "") << endl;

    if (!utxos.empty()) {
        for (const auto& utxo : utxos) {
            cout << "|    ";
            utxo.display(); // Assuming utxo.display() is a single-line output
        }
    }
    cout << "===========================================" << endl << endl;
}

};
