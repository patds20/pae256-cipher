#ifndef MAIN_CPP_PAE256_HELPFUNCTIONS_H
#define MAIN_CPP_PAE256_HELPFUNCTIONS_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

//! read filesize from ifstream
unsigned int getFileSize(const std::string& file_name) {
    std::ifstream file(file_name);
    file.seekg(0, std::ios::end);
    return file.tellg();
}

//! switch rows in matrix
inline void switch_rows(char x[16][16], int a, int b) {
    char tmp;
    for (int i = 0; i < 16; i++) {
        tmp = x[a][i];
        x[a][i] = x[b][i];
        x[b][i] = tmp;
    }
}

//! switch columns in matrix
inline void switch_columns(char x[16][16], int a, int b) {
    char tmp;
    if (a < 16 && b < 16) {
        for (int i = 0; i < 16; i++) {
            tmp = x[i][a];
            x[i][a] = x[i][b];
            x[i][b] = tmp;
        }
    }
}

//! xor each cell in two matrices
inline void matrixXOR(char x[16][16], char y[][16]) {
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t u = 0; u < 16; u++) {
            x[i][u] = x[i][u] ^ y[i][u];
        }
    }
}

//! switch cells in matrix
inline void swapElements(char x[16][16], int a, int b, int c, int d) {
    char tmp = x[a][b];
    x[a][b] = x[c][d];
    x[c][d] = tmp;
}

//! perform XOR of two rows in matrix
inline void rowXOR(char x[16][16], int a, int b) {
    for (uint8_t u = 0; u < 16; u++) {
        x[u][a] ^= x[u][b];
    }
}

//! perform XOR of two columns in matrix
inline void colXOR(char x[16][16], int a, int b) {
    for (uint8_t u = 0; u < 16; u++) {
        x[a][u] ^= x[b][u];
    }
}

//! perform XOR of two cells in matrix
inline void singleXOR(char x[16][16], int a, int b){
    x[a][b] ^= x[15-a][15-b];
}

//! transpose matrix
inline void transpose(char matrix[16][16]){
    char tmp[16][16];
    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            tmp[x][y] = matrix[y][x];
        }
    }
    for(int u = 0; u < 16; u++){
        for(int v = 0; v < 16; v++){
            matrix[u][v] = tmp[u][v];
        }
    }
}

//! mirrors the key
inline void mirror_key(char matrix[16][16]){
    char tmp;
    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            tmp = matrix[x][y];
            matrix[x][y] = matrix[15-x][y];
            matrix[15-x][y] = tmp;
        }
    }
}

//! perform inversion of cell in matrix
inline void invert(char matrix[16][16], int a, int b){
    matrix[a][b] = ~matrix[a][b];
}

//! print start screen
void printstart() {
    std::cout << "\n";
    std::cout << "########################################################################\n";
    std::cout << "### PDS Algorithm for Encryption  © 2022 Patrick De Smet      (V2.2) ###\n";
    std::cout << "########################################################################\n";
    std::cout << "\n";
}

#endif //MAIN_CPP_PAE256_HELPFUNCTIONS_H