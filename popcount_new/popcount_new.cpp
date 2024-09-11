﻿#include <iostream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <cmath>// 包含 log2 函數
#include <chrono>

#include <fstream>
#include <sstream>
using namespace std;

int ceilLog2_atcgmap_Count; //全域變數字母表數量
int atcgStr1_Count, atcgStr2_Count;

enum DNA_Base {
    A = 0, // 對應數字 0
    T = 1, // 對應數字 1
    C = 2, // 對應數字 2
    G = 3  // 對應數字 3
};
int base_lookup(char base) {
    switch (base) {
    case 'A': return A;
    case 'T': return T;
    case 'C': return C;
    case 'G': return G;
    default: throw std::invalid_argument("Invalid DNA base");
    }
}
unsigned long long generate_WitnessBit(int m, int n, int sliceIndex) {
    //unsigned long long result = 0;
    //int patternSize = m + 1;  // 模式的長度，例如 100 的長度是 3
    //int totalBits = patternSize * (n);  // 總位數，即模式長度乘以重複次數
    //int bitsPerSlice = 64;  // 每個切片的位數（如果需要支持其他位數，這裡可以修改）

    //// 計算應該跳過的位數
    //int startBit = sliceIndex * bitsPerSlice;//100<<3 | 100

    //// 計算模式在切片中的起始位置
    //int currentBit = startBit;

    //int i;
    //// 生成模式並填充到 result 中
    //for (i = 0; i < totalBits; ++i) {
    //    // 如果當前位是在模式中的 '1' 的位置
    //    if ((currentBit - (currentBit / patternSize) * patternSize) == 0) {
    //        result |= (1ULL << (i));
    //    }
    //    currentBit++;

    //}
    //result = result << m;

    unsigned long long result = 0;
    int patternSize = m + 1;  // 模式的長度，例如 100 的長度是 3
    int totalBits = patternSize * (n);  // 總位數，即模式長度乘以重複次數
    int bitsPerSlice = 63;  // 每個切片的位數（如果需要支持其他位數，這裡可以修改）21一切片，不要乘開?

    // 計算應該跳過的位數
    int startBit = sliceIndex * bitsPerSlice;//100<<3 | 100

    // 計算模式在切片中的起始位置
    int currentBit = startBit;

    int i;
    // 生成模式並填充到 result 中
    for (i = 0; i < 21; ++i) {
        result = result << 3 | 0b100;
    }
    return result;
}



int popcount(unsigned long long i) {
    i = i - ((i >> 1) & 0x5555555555555555ULL);
    i = (i & 0x3333333333333333ULL) + ((i >> 2) & 0x3333333333333333ULL);
    return (int)((((i + (i >> 4)) & 0xF0F0F0F0F0F0F0FULL) * 0x101010101010101ULL) >> 56);
}

int calculateHammingDistance(unsigned long long* buffer1, unsigned long long* buffer2) {
    int hammingDistance = 0;
    unsigned long long B_WitnessBit, BinvWitnessBit, num1, num2, xorResult;
    int m = ceilLog2_atcgmap_Count; // 幾個0
    int n = atcgStr1_Count; // 100重複幾次

    //int totalSlices = (n * (m + 1) + 63) / 64; // 計算需要分成多少個60位切片
    int totalSlices = (n + 20) / 21; // 計算切片 64bit 3個1組，一次處理21組

    string BinvWitnessBit_str, B_WitnessBit_str, chunk1, chunk2;

    for (int i = 0; i < totalSlices; ++i) {
        B_WitnessBit = generate_WitnessBit(m, n, i);
        bitset<64> B_WitnessBitbinary(B_WitnessBit);
        cout << "B_WitnessBitbinary" << ":" << B_WitnessBitbinary << endl;
        BinvWitnessBit = ~B_WitnessBit;

        num1 = buffer1[i];
        num2 = buffer2[i];

        xorResult = num1 ^ num2; // XOR 運算

        B_WitnessBit = generate_WitnessBit(m, n, i);
        BinvWitnessBit = ~B_WitnessBit;
        hammingDistance += popcount((xorResult + BinvWitnessBit) & B_WitnessBit);
    }

    return hammingDistance;
}

int main() {
    unordered_map<char, unsigned char> atcgmapToBit = {
        {'A', 0},
        {'T', 1},
        {'C', 2},
        {'G', 3}
    };
    size_t atcgmap_Count = atcgmapToBit.size();

    double log2_atcgmap_Count = log2(static_cast<double>(atcgmap_Count));
    ceilLog2_atcgmap_Count = static_cast<int>(ceil(log2_atcgmap_Count));

    //string atcgStr1 = "GGTCGAGTGAAATCTTTGAGAAGAAGGGGGGAACCTCTGTAGTTTGTGCTACTGCTAAGGGAGG";//64
    //string atcgStr2 = "GGTCGAGTGAAATCTTTGAGAAGAAGGGGGGAACCTCTGTAGTTTGTGCTACTGCTAAGGGAGG";//64

    //string atcgStr1 = "TGTAACATTGTTCTAGGGACAAGCGGGAAGCA";//32
    //string atcgStr2 = "TGTAACATTGTTCTAGGGACAAGCGGGAAGCA";//32 

    //string atcgStr1 = "AAAATTTTCCCCGGGG";// 16/12
    //string atcgStr2 = "ATCGATCGATCGATCG";// 16

    //string atcgStr1 = "ATCTGTAG";//8
    //string atcgStr2 = "ATCGATCG";//8 

    //string atcgStr1 = "AAAA";//12
    //string atcgStr2 = "ATCG";//12
    // 
    // 檔案名稱
    string filename1 = "d1_4.txt";
    string filename2 = "d2_4.txt";

    ifstream infile1(filename1);       // 以讀取模式打開檔案
    ifstream infile2(filename2);       // 以讀取模式打開檔案

    // 檢查檔案是否成功打開
    if (!infile1) {
        cerr << "無法打開檔案 " << filename1 << endl;
        return 1;
    }

    // 檢查檔案是否成功打開
    if (!infile2) {
        cerr << "無法打開檔案 " << filename2 << endl;
        return 1;
    }

    stringstream buffer1;
    stringstream buffer2;

    buffer1 << infile1.rdbuf();  // 將檔案內容讀入到 stringstream 中
    buffer2 << infile2.rdbuf();  // 將檔案內容讀入到 stringstream 中

    string atcgStr1 = buffer1.str();  // 將 stringstream 中的內容轉換為 string
    string atcgStr2 = buffer2.str();


    atcgStr1_Count = atcgStr1.length();
    atcgStr2_Count = atcgStr2.length();

    string bitStr1, bitStr2;
    //read str to ULL
    int charsPerULL = 21;  // 每個 unsigned long long 存儲 21 個字符
    int strLength = atcgStr1.length();
    int buffer1Sizes = (strLength + charsPerULL - 1) / charsPerULL;  // 計算需要的 ULL 數量
    unsigned long long* encodedBuffer1 = new unsigned long long[buffer1Sizes];
   
    for (int i = 0; i < buffer1Sizes; i++) {
        encodedBuffer1[i] = 0;
    }

    // 對字串進行編碼
    for (int i = 0; i < strLength; ++i) {
        unsigned long long baseBit = base_lookup(atcgStr1[i]) & 0b111;;  // 獲取字符的 3 位編碼
        int bufferIndex = i / charsPerULL;  // 計算當前字符對應的 ULL 緩衝區索引
        int bitShift = (charsPerULL - 1 - (i % charsPerULL)) * 3;  // 計算該字符在 ULL 中的偏移量
        encodedBuffer1[bufferIndex] |= (baseBit << bitShift);  // 將 baseBit 左移並存入對應的位置
    }

    //int buffer2Sizes = (atcgStr2_Count + 21 - 1) / 21;//無條件進位 分為n串 1串處理21字元
    //unsigned long long* atcgBitBuffer2 = new unsigned long long[buffer2Sizes];

    //jSize = (atcgStr2_Count < 21) ? atcgStr2_Count : 21;
    //for (int i = 0; i < buffer2Sizes; i++) {
    //    unsigned long long kk = 0;//?
    //    for (int j = 0; j < jSize; j++) {
    //        char base = atcgStr2[i * jSize + j];
    //        unsigned long long baseBit = base_lookup(base) & 0b111;
    //        if (i != 0)
    //            kk = baseBit;
    //        else
    //            kk = (kk << 3) | baseBit;
    //        //kk = (kk << ((i != 0) ? 3:0)) &baseBit;
    //        bitset<12> baseBitbinary(baseBit);
    //        bitset<12> kkbinary(kk);
    //        /*cout << base << ":" << baseBitbinary << endl;
    //        cout << "kk" << ":" << kkbinary << endl;*/

    //    }
    //    /*cout << kk;*/
    //    atcgBitBuffer2[i] = kk;
    //}

    int str2Length = atcgStr2.length();
    int buffer2Sizes = (str2Length + charsPerULL - 1) / charsPerULL;  // 計算需要的 ULL 數量
    unsigned long long* encodedBuffer2 = new unsigned long long[buffer2Sizes];

    for (int i = 0; i < buffer2Sizes; i++) {
        encodedBuffer2[i] = 0;
    }
    // 對字串進行編碼
    for (int i = 0; i < str2Length; ++i) {
        unsigned long long baseBit = base_lookup(atcgStr2[i]) & 0b111;;  // 獲取字符的 3 位編碼
        int buffer2Index = i / charsPerULL;  // 計算當前字符對應的 ULL 緩衝區索引
        int bitShift2 = (charsPerULL - 1 - (i % charsPerULL)) * 3;  // 計算該字符在 ULL 中的偏移量
        encodedBuffer2[buffer2Index] |= (baseBit << bitShift2);  // 將 baseBit 左移並存入對應的位置
    }

    /*for (char base : atcgStr1) {
        if (atcgmapToBit.find(base) != atcgmapToBit.end()) {
            bitStr1 += atcgmapToBit[base];
        }
        else {
            cout << "無效字符: " << base << endl;
            return 1;
        }
    }*/


    //for (char base : atcgStr2) {
    //    if (atcgmapToBit.find(base) != atcgmapToBit.end()) {
    //        bitStr2 += atcgmapToBit[base];
    //    }
    //    else {
    //        cout << "無效字符: " << base << endl;
    //        return 1;
    //    }
    //}

    //if (bitStr1.length() != bitStr2.length()) {
    //    cout << "字符串長度不匹配，無法計算漢明距離" << endl;
    //    return 1;
    //}

    double at = 0;
    int j = 0;
    for (j = 0; j < 1; j++) {
        auto start = chrono::high_resolution_clock::now();
        int hammingDistance = calculateHammingDistance(encodedBuffer1, encodedBuffer2);
        auto end = chrono::high_resolution_clock::now();
        // 輸出結果
        cout << atcgStr1 << " 的二進制表示: "<<endl;
        for (size_t i = 0; i < buffer1Sizes; ++i) {
            bitset<64> baseBitbinary(encodedBuffer1[i]);
            //cout << "1234567890123456789012345678901234567890123456789012345678901234567890" << endl;
            //cout << "ULL[ " << i << " ]: " << encodedBuffer1[i] << endl;
            cout << baseBitbinary << endl;
        }
        cout << atcgStr2 << " 的二進制表示: " << endl;
        for (size_t i = 0; i < buffer2Sizes; ++i) {
            bitset<64> baseBit2binary(encodedBuffer2[i]);
            //cout << "1234567890123456789012345678901234567890123456789012345678901234567890" << endl;
            //cout << "ULL[ " << i << " ]: " << encodedBuffer1[i] << endl;
            cout << baseBit2binary << endl;
        }
        cout << endl;
        cout << "漢明距離: " << hammingDistance << endl;

        // 計算經過的時間
        chrono::duration<double> duration = end - start;

        // 輸出結果（以秒為單位）
        //cout << "程式執行時間: " << duration.count() << " s" << endl;
        at += duration.count();
    }
    cout << "長度為 " << atcgStr1.length() << " time : " << at << endl;
    
    return 0;

}