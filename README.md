# simplified_blockchain
# Blockchain Project

## Overview

This project is a basic implementation of a blockchain system. It includes components such as blocks, transactions, users, and hashing mechanisms to simulate the functionality of a blockchain. The project is written in C++ and provides a simple structure for managing users, transactions, and blocks using cryptographic hashes.

## Features

- **Blocks**: Represents a block in the blockchain with a list of transactions and a unique hash.
- **Blockchain**: A chain of blocks that manages block creation and validation.
- **Transactions**: Represents transactions between users, including details such as sender, receiver, and amount.
- **Hashing**: Cryptographic hash functions used to ensure the integrity and immutability of blocks and transactions.
- **User Management**: Handles users, their balances, and unspent transaction outputs (UTXOs).
- **UTXO Model**: Supports an unspent transaction output model to manage user balances and facilitate transactions.

## File Structure

- **`block.h`**: Contains the `Block` class, which defines the structure of a block and includes functionalities like generating hashes and validating blocks.
  
- **`blockchain.h`**: Defines the `Blockchain` class, responsible for managing the chain of blocks, adding new blocks, and verifying the integrity of the chain.

- **`transaction.h`**: Defines the structure of a transaction, including inputs, outputs, and digital signatures for security.

- **`hash.cpp` and `hash.h`**: Implements cryptographic hashing functions used throughout the blockchain for generating secure, tamper-proof hashes for blocks and transactions.

- **`user.h`**: Contains the `User` class, which manages user data, public and private keys, and UTXOs (Unspent Transaction Outputs). This file also handles user balances and the selection of UTXOs for transactions.

- **`lib.h`**: Contains utility functions and helpers used throughout the project, such as file handling and random data generation.

- **`main.cpp`**: The main entry point of the application where the blockchain is initialized, and users can create and verify transactions, add blocks to the chain, and interact with the system.

## How to Run

1. **Clone the Repository**: 
   ```bash
   git clone https://github.com/your-repo/blockchain-project.git
   cd blockchain-project
2. **Compile the Code:** Use a C++ compiler to compile the code. For example, using `g++`:

    ```bash
    g++ main.cpp hash.cpp -o blockchain
    ```

3. **Run the Application:**

    ```bash
    ./blockchain
    ```

4. **Interact with the Blockchain:** The application allows you to create users, perform transactions, and view the state of the blockchain.

### Dependencies

- C++11 or higher
- Standard C++ libraries (`iostream`, `vector`, `string`, `chrono`, etc.)

### Future Improvements

- Add support for mining rewards and proof-of-work consensus.
- Implement a more sophisticated cryptographic system for digital signatures.
- Expand the transaction validation rules.
- Improve performance with optimized data structures.
