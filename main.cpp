#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include "algorithm_steps.h"

//PDS Encryption Source Code © 2022 Patrick De Smet

//! Generate new key
void keyg(const std::string& path) {
    // Initialize random seed only once
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,99);

    // Generate Random Key
    char keys[16][16];
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            keys[x][y] = (char)(dist6(rng));
        }
    }

    // Open file and write key
    std::ofstream keyp(path, std::ios::out | std::ios::binary);
    if (keyp.is_open()) {
        for (uint8_t u = 0; u < 16; u++) {
            keyp.write(&keys[u][0], 16);
        }
        keyp.close();
    }
}

//! Encryption with existing key
void encryption_wk(const std::string& inp_path, const std::string& key_path, const std::string& output_path) {
    // Open input, key and output streams
    std::ifstream input(inp_path, std::ifstream::binary);
    std::ofstream output(output_path, std::ios::binary);
    std::ifstream keyp(key_path, std::ifstream::binary);
    if (!input || !output || !keyp) {
        throw std::runtime_error("Failed to open one of the files");
    }

    // Write the input file size to the output file
    uint64_t fsize = getFileSize(inp_path);
    output.write(reinterpret_cast<const char*>(&fsize), sizeof(fsize));

    // Read the encryption keys from the key file
    char keys[16][16];
    for (int i = 0; i < 16; i++) {
        keyp.read(reinterpret_cast<char*>(keys[i]), 16);
    }
    keyp.close();

    // Process input file in chunks of 256 bytes
    while (input) {
        // Read 256 bytes from the input file and convert to matrix
        char clear_data[16][16] = {};
        input.read(reinterpret_cast<char*>(clear_data), 256);

        // Apply encryption algorithm to the matrix
        pre_enc(clear_data);
        for (int i = 0; i < 10; i++) {
            encryption_steps(clear_data, keys);
        }
        mirror_key(keys);
        for (int i = 0; i < 10; i++) {
            encryption_steps(clear_data, keys);
        }
        key_unfold(keys);

        // Write the encrypted matrix to the output file
        output.write(reinterpret_cast<char*>(clear_data), 256);
    }

    // Close input and output streams
    input.close();
    output.close();
}

//! Encryption with new key
void encryption(const std::string& inp_path, const std::string& key_path, const std::string& output_path) {
    // Generate random key
    char keys[16][16];
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,96);
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            keys[x][y] = (char)(dist6(rng));
        }
    }

    // Open input, key and output streams
    std::ifstream input = std::ifstream(inp_path, std::ifstream::binary);
    std::fstream output = std::fstream(output_path, std::ios::out | std::ifstream::binary);
    std::ofstream keyp = std::ofstream(key_path, std::ios::out | std::ifstream::binary);

    // Write the size of the input file to the output stream
    uint64_t fsize = getFileSize(inp_path);
    output.write(reinterpret_cast<const char *>(&fsize), sizeof(uint64_t));

    // Write the key to the key file
    for (uint8_t x = 0; x < 16; x++) {
        keyp.write(reinterpret_cast<const char *>(keys[x]), 16);
    }

    // Iterate through chunks of 256 bytes
    while (input) {
        // Read input into a 16x16 matrix
        char clear_data[16][16];
        for (uint8_t x = 0; x < 16; x++) {
            input.read(clear_data[x], 16);
        }

        // Encrypt the matrix
        pre_enc(clear_data);
        for (int i = 0; i < 10; i++) {
            encryption_steps(clear_data, keys);
        }
        mirror_key(keys);
        for (int i = 0; i < 10; i++) {
            encryption_steps(clear_data, keys);
        }
        key_unfold(keys);

        // Write the encrypted matrix to the output stream
        for (uint8_t x = 0; x < 16; x++) {
            output.write(clear_data[x], 16);
        }
    }

    // Close input, key and output streams
    input.close();
    output.close();
    keyp.close();
}

//! Decryption algorithm
void decryption(const std::string& input_path, const std::string& key_path, const std::string& output_path) {
    // Open input, key, and output streams
    std::ifstream input(input_path, std::ios::binary);
    std::ifstream key(key_path, std::ios::binary);
    std::ofstream output(output_path, std::ios::binary);

    // Read the size of the input file from the first 8 bytes
    uint64_t input_size;
    input.read(reinterpret_cast<char*>(&input_size), sizeof(uint64_t));

    // Read the key from the key file
    char key_matrix[16][16];
    char key_byte;
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            key.read(&key_byte, 1);
            key_matrix[x][y] = key_byte;
        }
    }
    key.close();

    // Process the input data in 256-byte chunks
    while (input) {
        // Read 256 bytes from the input file into the encrypted_data matrix
        char encrypted_data[16][16] = {0};
        input.read(reinterpret_cast<char*>(encrypted_data), 256);

        // Perform decryption steps
        mirror_key(key_matrix);
        for (int i = 0; i < 10; i++) {
            decryption_steps(encrypted_data, key_matrix);
        }
        mirror_key(key_matrix);
        for (int i = 0; i < 10; i++) {
            decryption_steps(encrypted_data, key_matrix);
        }
        pre_dec(encrypted_data);
        key_unfold(key_matrix);

        // Write the decrypted data to the output file
        const uint64_t bytes_to_write = std::min<uint64_t>(256, input_size);
        output.write(reinterpret_cast<char*>(encrypted_data), bytes_to_write);
        input_size -= bytes_to_write;
    }

    // Close the input, key, and output streams
    input.close();
    output.close();
}

//! Print help in CLI
void print_help() {
    std::cout << "Usage: program_name [mode] [input_file] [key_file] [output_file]\n"
              << "\n"
              << "Modes:\n"
              << "-e : Encryption\n"
              << "-d : Decryption\n"
              << "-ek: Encryption with existing key\n"
              << "-k : Generate a key file\n";
}

//! Start Encryption with New Key
void handle_encryption(const std::string& input_path, const std::string& key_path, const std::string& output_path) {
    std::cout << ">> Operating Mode: Encryption\n";
    std::cout << ">> Input File: " << input_path << "\n";
    std::cout << ">> Key File: " << key_path << "\n";
    std::cout << ">> Output File: " << output_path << "\n\n";

    encryption(input_path, key_path, output_path);
}

//! Start Decryption
void handle_decryption(const std::string& input_path, const std::string& key_path, const std::string& output_path) {
    std::cout << ">> Operating Mode: Decryption\n";
    std::cout << ">> Input File: " << input_path << "\n";
    std::cout << ">> Key File: " << key_path << "\n";
    std::cout << ">> Output File: " << output_path << "\n\n";

    decryption(input_path, key_path, output_path);
}

//! Start Encryption with Existing Key
void handle_encryption_with_existing_key(const std::string& input_path, const std::string& key_path, const std::string& output_path) {
    std::cout << ">> Operating Mode: Encryption with existing key\n";
    std::cout << ">> Input File: " << input_path << "\n";
    std::cout << ">> Key File: " << key_path << "\n";
    std::cout << ">> Output File: " << output_path << "\n\n";

    encryption_wk(input_path, key_path, output_path);
}

//! Start Key Generation without Encryption
void handle_key_file(const std::string& key_path) {
    std::cout << ">> Operating Mode: Key File\n";
    std::cout << ">> Key File: " << key_path << "\n\n";

    keyg(key_path);
}

//! Main Function
int main(int argc, char *argv[]) {
    if (argc == 1 || argc > 5) {
        print_help();
        return 1;
    }
    printstart();
    std::string mode = argv[1];
    if (mode == "-e" && argc == 5) {
        handle_encryption(argv[2], argv[3], argv[4]);
    } else if (mode == "-d" && argc == 5) {
        handle_decryption(argv[2], argv[3], argv[4]);
    } else if (mode == "-ek" && argc == 5) {
        handle_encryption_with_existing_key(argv[2], argv[3], argv[4]);
    } else if (mode == "-k" && argc == 3) {
        handle_key_file(argv[2]);
    } else if (mode == "-v") {
        std::cout << "PDS Algorithm for Encryption  © 2022 Patrick De Smet (V2.2.0)" << std::endl;
        return 0;
    } else if (mode == "--help") {
        print_help();
        return 0;
    } else {
        std::cout << ">> error: Invalid arguments\n";
        print_help();
        return 1;
    }

    return 0;
}
