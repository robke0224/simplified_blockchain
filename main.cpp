#include "mylib.h"
#include "randomizer.h"
#include "functions.h"
#include "naudotojas.h"
#include "transakcija.h"
#include "merkletree.h"
#include "block.h"


#define TRANSACTIONS_PER_BLOCK 100
#define MINE_ATTEMPTS 100000
#define CANDIDATE_BLOCKS 5


vector<Transaction> getRandomTransactions(vector<Transaction>& allTransactions, int numTransactions, const vector<User>& users) {
    std::shuffle(allTransactions.begin(), allTransactions.end(), std::mt19937(std::random_device()()));
    if (allTransactions.size() <= numTransactions) {
        return allTransactions;
    }
    vector<Transaction> randomTransactions(allTransactions.begin(), allTransactions.begin() + numTransactions);
    return randomTransactions;
}

void removeApprovedTransactions(vector<Transaction>& allTrans, const std::vector<Transaction>& approvedTrans) {
    allTrans.erase(std::remove_if(allTrans.begin(), allTrans.end(), [&approvedTrans](const Transaction& t) {
        return std::find_if(approvedTrans.begin(), approvedTrans.end(), [&t](const Transaction& at) {
            return t.GetID() == at.GetID();
        }) != approvedTrans.end();
    }), allTrans.end());
}

void verifyAndPushBlock(std::stack<Block>& blockchain, Block& block, const vector<Transaction>& blockTransactions, vector<Transaction>& transactions, UserManager& userManager) {
    if (block.getHash() == getHashString(block.getPreviousHash() + block.getMerkleRoot() + std::to_string(block.getTimestamp()) + std::to_string(block.getDifficultyTarget()) + std::to_string(block.getNonce())) && block.meetRequirements()) {
        for (const Transaction& tx : blockTransactions) {
            userManager.updateBalance(tx.GetSender(), -tx.GetAmount()); 
            userManager.updateBalance(tx.GetReceiver(), tx.GetAmount());
        }
        blockchain.push(std::move(block));
        removeApprovedTransactions(transactions, blockTransactions);
        cout << "Block " << blockchain.size() << ": " << block.getHash() << endl;
        cout << "Transactions left in pool: " << transactions.size() << endl;
    } else {
        cout << "Fake block detected!" << endl;
    }
}

int main() {
    UserManager userManager(1000);
    vector<User> users = userManager.getUsers();
    TransactionBuilder transactionBuilder(10000, userManager);
    vector<Transaction> transactions = transactionBuilder.getTransactions();
    
    vector<Transaction> genesisTransactions;
    Block genesisBlock("0", 4, genesisTransactions);
    
    std::stack<Block> blockchain;
    blockchain.push(genesisBlock);
    
    unsigned int difficultyTarget = 4;
    unsigned int attemptsPerMiner = MINE_ATTEMPTS;
    
    vector<Mine> miners;
    for (int i = 0; i < 5; ++i) {
        miners.emplace_back(i + 1);
    }
    
    while (!transactions.empty()) {
        std::vector<Transaction> blockTransactions = getRandomTransactions(transactions, TRANSACTIONS_PER_BLOCK, users);
        Block candidateBlock(blockchain.top().getHash(), difficultyTarget, blockTransactions);
        bool blockMined = false;
        
        for (auto &miner : miners) {
            if (blockMined) break;
            try {
                Block minedBlock = miner.mineBlock(candidateBlock.getPreviousHash(), difficultyTarget, blockTransactions, attemptsPerMiner);
                verifyAndPushBlock(blockchain, minedBlock, blockTransactions, transactions, userManager);
                miner.incrementBlocksMined(); 
                cout << "Miner " << miner.getID() << " successfully mined the block!" << endl;
                blockMined = true;
            } catch (const std::runtime_error& e) {
                cout << e.what() << endl;  // Mining attempt failed
            }
        }

        if (!blockMined) {
            cout << "All miners failed to mine the block in" << attemptsPerMiner << "attempts. Increasing the attempts" << endl;
            attemptsPerMiner *= 2;
            
        }
    }
    
    cout << "Mining process completed. Blockchain contains " << blockchain.size() << " blocks." << endl;
    for (const auto& miner : miners) {
        cout << "Miner " << miner.getID() << " mined " << miner.getBlocksMined() << " blocks." << endl;
    }
    return 0;
}
