#ifndef MERKLETREE_H
#define MERKLETREE_H

#include "mylib.h"
#include "functions.h"
#include "transakcija.h"

// Class representing a Merkle Tree for a set of transactions
class MerkleTree {
private:
    // Vector of transactions to build the Merkle tree from
    std::vector<Transaction> transactions;

    // Helper function that combines two hashes (left and right) and returns the hash of their concatenation
    std::string hashCombinedTransactions(const std::string& left, const std::string& right) {
        return getHashString(left + right);
    }

public:
    // Constructor that initializes the Merkle tree with a vector of transactions
    MerkleTree(const std::vector<Transaction>& transactions) : transactions(transactions) {}

    // Method to create the Merkle root from the transactions
    std::string createMerkleRoot() {
        // If there are no transactions, return a placeholder hash for an empty tree
        if (transactions.empty()) {
            return getHashString("Empty Tree Placeholder Hash");
        }

        // Initialize the tree vector with the transaction IDs (hashes of transactions)
        std::vector<std::string> tree;
        for (const auto& transaction : transactions) {
            tree.push_back(transaction.getID());
        }

        // Build the tree by hashing pairs of nodes until only one root node remains
        while (tree.size() > 1) {
            std::vector<std::string> newLevel;
            for (size_t i = 0; i < tree.size(); i += 2) {
                if (i + 1 < tree.size()) {
                    // Hash adjacent nodes together
                    newLevel.push_back(hashCombinedTransactions(tree[i], tree[i + 1]));
                } else {
                    // If there's an odd number of nodes, hash the last node with itself
                    newLevel.push_back(hashCombinedTransactions(tree[i], tree[i]));
                }
            }
            tree = newLevel;
        }

        // Return the root hash (the only remaining element)
        return tree.front();
    }
};

#endif
