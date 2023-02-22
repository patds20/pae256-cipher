#ifndef MAIN_CPP_ALGORITHM_STEPS_H
#define MAIN_CPP_ALGORITHM_STEPS_H

#include "pae256_helpfunctions.h"

//! Steps for a single encryption round (of 20)
void encryption_steps(char matrix[16][16], char key[16][16]){
    for (int i = 0; i <= 1; i++) {
        for (int j = 0; j <= 12; j+=4) {
            swapElements(matrix, (key[i][j] % 16), (key[i][j+1] % 16), (key[i][j+2] % 16), (key[i][j+3] % 16));
        }
    }
    for (int i = 0; i <= 14; i += 2) {
        switch_rows(matrix, key[2][i] % 16, key[2][i+1] % 16); }
    for (int i = 0; i <= 14; i += 2) {
        switch_columns(matrix, key[3][i] % 16, key[3][i+1] % 16); }
    for (int i = 0; i <= 14; i += 2) {
        singleXOR(matrix, key[9][i] % 16, key[9][i+1] % 16); }
    for (int i = 0; i <= 14; i += 2) {
        invert(matrix, key[4][i] % 16, key[4][i+1] % 16); }
    for (int i = 5; i <= 6; i++) {
        for (int j = 0; j <= 12; j+=4) {
            swapElements(matrix, (key[i][j] % 16), (key[i][j+1] % 16), (key[i][j+2] % 16), (key[i][j+3] % 16));
        }
    }
    for (int i = 0; i <= 7; i++) {
        rowXOR(matrix, (key[7][i] % 16), ((key[7][i]+i+1) % 16)); }
    for (int i = 0; i <= 7; i++) {
        colXOR(matrix, (key[8][i] % 16), ((key[8][i]+i+1) % 16)); }

    matrixXOR(matrix,key);
    transpose(matrix);
}

//! Reverse steps for a single decryption round (of 20)
void decryption_steps(char matrix[16][16], char key[16][16]){

    transpose(matrix);
    matrixXOR(matrix,key);

    for (int i = 7; i >= 0; i--) {
        colXOR(matrix, (key[8][i] % 16), ((key[8][i]+i+1) % 16)); }
    for (int i = 7; i >= 0; i--) {
        rowXOR(matrix, (key[7][i] % 16), ((key[7][i]+i+1) % 16)); }
    for (int i = 6; i >= 5; i--) {
        for (int j = 12; j >= 0; j-=4){
            swapElements(matrix,
                         key[i][j] % 16, key[i][j+1] % 16,
                         key[i][j+2] % 16, key[i][j+3] % 16);
        }
    }
    for (int i = 14; i >= 0; i-=2) {
        invert(matrix, (key[4][i] % 16), (key[4][i+1] % 16)); }
    for (int i = 14; i >= 0; i-=2) {
        singleXOR(matrix, (key[9][i] % 16), (key[9][i+1] % 16)); }
    for (int i = 14; i >= 0; i-=2) {
        switch_columns(matrix, (key[3][i] % 16), (key[3][i+1] % 16)); }
    for (int i = 14; i >= 0; i-=2) {
        switch_rows(matrix, (key[2][i] % 16), (key[2][i+1] % 16)); }
    for (int i = 1; i >= 0; i--) {
        for (int j = 12; j >= 0; j-=4){
            swapElements(matrix,
                         key[i][j] % 16, key[i][j+1] % 16,
                         key[i][j+2] % 16, key[i][j+3] % 16);
        }
    }
}

//! Additional XOR operations before encryption
void pre_enc(char matrix[16][16]){
    for(int col = 0; col < 16; col++){
        for(int ele = 1; ele < 16; ele++){
            matrix[ele][col] = matrix[ele][col] ^ matrix[ele-1][col];
        }
    }
    for(int row = 0; row < 16; row++){
        for(int ele = 1; ele < 16; ele++){
            matrix[row][ele] = matrix[row][ele] ^ matrix[row][ele-1];
        }
    }
}

//! Additional XOR operations after decryption
void pre_dec(char matrix[16][16]){
    for(int row = 0; row < 16; row++){
        for(int ele = 15; ele >= 1; ele--){
            matrix[row][ele] = matrix[row][ele] ^ matrix[row][ele-1];
        }
    }
    for(int col = 0; col < 16; col++){
        for(int ele = 15; ele >= 1; ele--){
            matrix[ele][col] = matrix[ele][col] ^ matrix[ele-1][col];
        }
    }
}

//! Generating a new key for next round
void key_unfold(char key[16][16]){
    pre_dec(key);
    transpose(key);
    for (int i = 1; i >= 0; i--) {
        for (int j = 12; j >= 0; j-=4) {
            swapElements(key, (key[i][j] % 16), (key[i][j+1] % 16), (key[i][j+2] % 16), (key[i][j+3] % 16));
        }
    }
    for (int i = 14; i >= 0; i -= 2) {
        switch_columns(key, key[3][i] % 16, key[3][i+1] % 16); }
    for (int i = 14; i >= 0; i -= 2) {
        switch_rows(key, (key[2][i] % 16), (key[2][i+1] % 16)); }
}

#endif //MAIN_CPP_ALGORITHM_STEPS_H