#define SIZE 64
#define KEY_SIZE 56
#include <stdio.h>
void print_arr(char arr[], int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%c ", arr[i * col + j]);
        }
        printf("\n");
    }
}

// These arrays are DES encryption/decryption standard values

int IP[] =
{
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};
 
int E[8][6] =
{
    32,  1,  2,  3,  4,  5,
    4,   5,  6,  7,  8,  9,
    8,   9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};
 
int P[] =
{
    16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};
 
int FP[] =
{
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};

int sboxes[8][4][16] = {
    {
        14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
        0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
        4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
    },
    {
        15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
        3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
        0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
        13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
    },
    {
        10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
        13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
        13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
        1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
    },
    {
        7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
        13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
        10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
        3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
    },
    {
        2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
        4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
    },
    {
        12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
        10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
        9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
        4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
    },
    {
        4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
        13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
        1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
        6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
    },
    {
        13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
        1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
        7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
        2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
    }
};


 
int PC1[] =
{
    57, 49, 41, 33, 25, 17,  9,
    1,  58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7,  62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};
 
int PC2[] =
{
    14, 17, 11, 24,  1,  5,
    3,  28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};
 
int SHIFTS[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

// Copy an array into another array
void copy_arr(char arr1[], char arr2[], int start, int finish){
    for (int i = start; i < finish; i++){
        arr2[i] = arr1[i];
    }
}

// Convert a hex character to 4 binary bits
void hex_to_bin_char(char hex, char bin[]) {
    char code = 0;
    if (hex < 58 && hex >= 48) {
        code = hex - 48;
    }
    else if (hex >= 97 && hex < 103) {
        code = hex - 97 + 10;
    }
    bin[0] = (code >> 3) & 0b0001;
    bin[1] = (code >> 2) & 0b0001;
    bin[2] = (code >> 1) & 0b0001;
    bin[3] = code & 0b0001;
}


// Convert an array of hex characters to an array binary bits
void hex_to_bin(char hex[], char bin[], int n) {
    // accept hex with size n return bin with size n*4
    for (int i = 0; i < n; i++) {
        hex_to_bin_char(hex[i], bin + i * 4);
    }
}

// Convert 4 binary bits to a character
char bin_to_hex_char(char bin[]) {
    char code = (bin[0] << 3) | (bin[1] << 2) | (bin[2] << 1) | bin[3];
    if (code < 10) return code + 48;
    else return code + 97 - 10;
}

// Convert an array of binary bits into an array of characters
void bin_to_hex(char bin[], char hex[], int n) {
    for (int i = 0; i < n; i++) {
        hex[i] = bin_to_hex_char(bin + i * 4);
    }
}

// Permute an array using "perm" and place it into another array
void permute(char old_arr[], char new_arr[], int n, int perm[]) {
    for (int i = 0; i < n; i++) {
        new_arr[i] = old_arr[perm[i] - 1];
    }
}

// Shift an array to the left with a certain amount of shifts
void shift_left(char old_arr[], char new_arr[], int n, int amount) {
    for (int i = 0; i < n; i++) {
        int next = (i + amount) % n;
        new_arr[i] = old_arr[next];
    }
}

// For later use in sbox
char hexes[] = { '0','1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };


// The start of the des algorithm
void des(char input[], char output[], char key[], bool decrypt){
    
    char key_bin[SIZE];
    char input_bin_temp[SIZE];
    char input_bin[SIZE];
    // get the key in binary table 8x8 = 64
    hex_to_bin(key,key_bin,16);
    hex_to_bin(input, input_bin_temp, 16);
    // Compute PC - 1 
    char choice1[KEY_SIZE];
    permute(key_bin, choice1, KEY_SIZE, PC1);
    char c[28], d[28];

    // Permute input
    permute(input_bin_temp, input_bin, SIZE, IP);
    
    // Copy down the left and right bits
    char left[32], right[32];
    copy_arr(input_bin, left, 0, 32);
    copy_arr(input_bin+32, right, 0, 32);
    char choice2_arr[16][48];
    for (int round = 0; round < 16; round++) {

        // Shift the choice 1
        shift_left(choice1, c, 28, SHIFTS[round]);
        shift_left(choice1 + 28, d, 28, SHIFTS[round]);
        copy_arr(c, choice1, 0, 28);
        copy_arr(d, choice1+28, 0, 28);

        // Get choice 2
        char choice2[48];
        permute(choice1, choice2, 48, PC2);
        copy_arr(choice2,choice2_arr[round],0,48);
    }
    for (int round = 0; round < 16; round++) {

        // temp copy left
        char temp_left[32];
        copy_arr(left, temp_left, 0, 32);

        // copy right side and put it in left
        copy_arr(right, left, 0, 32);

        // make the e table values with xor choice2
        char exc[8][6];
        char sbox[32];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 6; j++) {
                if (!decrypt){
                    exc[i][j] = right[E[i][j] - 1] ^ choice2_arr[round][i*6+j];
                    continue;
                }
                    exc[i][j] = right[E[i][j] - 1] ^ choice2_arr[15-round][i*6+j];

            }
            // get row and column of sbox
            int row = (exc[i][0] << 1) | exc[i][5];
            int col = (exc[i][1] << 3) | (exc[i][2] << 2) | (exc[i][3] << 1) | exc[i][4];
            int num= sboxes[i][row][col];
            hex_to_bin_char(hexes[num], sbox + i * 4);
        }
        for (int i = 0; i < 32; i++) {
            right[i] = sbox[P[i]-1] ^ temp_left[i];
        }
    }
    char temp_output[SIZE];
    char bin_output[SIZE];
    copy_arr(left, temp_output+32, 0, 32);
    copy_arr(right, temp_output, 0, 32);
    for (int i = 0; i < SIZE; i++) {
        bin_output[i] = temp_output[FP[i] - 1];
    }
    bin_to_hex(bin_output, output, 16);
    
}

// Convert a hex character to 4 binary bits
char hex_to_char(char hex1, char hex2) {
    char code1 = 0;
    char code2 = 0;
    if (hex1 < 58 && hex1 >= 48) {
        code1 = hex1 - 48;
    }
    else if (hex1 >= 97 && hex1 < 103) {
        code1 = hex1 - 97 + 10;
    }
    if (hex2 < 58 && hex2 >= 48) {
        code2 = hex2 - 48;
    }
    else if (hex2 >= 97 && hex2 < 103) {
        code2 = hex2 - 97 + 10;
    }
    return code1*16+code2;
}


// output in multiples of 16
// input in multiples of 8
// length = length of output i.e the hex encoded string
void string_to_hex(char input[], char output[], int length){
    for (int i = 0; i < length; i+=2){
        output[i] = hexes[input[i/2]/16];
        output[i+1] = hexes[input[i/2]%16];
    }
}

// length of hex
void hex_to_string(char input[], char output[], int length){
    for (int i = 0; i < length; i+=2){
        output[i/2] = hex_to_char(input[i],input[i+1]);
    }
}

// ascii message length 2048 for 1024 bytes
void encrypt(char input[], char output[], char key[], int length){
    for (int i = 0; i < length; i+=16){
        des(input+i,output+i,key,false);
    }
}

// ascii message length 2048 for 1024 bytes
void decrypt(char input[], char output[], char key[], int length){
    for (int i = 0; i < length; i+=16){
        des(input+i,output+i,key,true);
    }
}


