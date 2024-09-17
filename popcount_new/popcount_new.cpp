#include <iostream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <cmath>// 包含 log2 函數
#include <chrono>

#include <fstream>
#include <sstream>
using namespace std;

int ceilLog2_atcgmap_Count; //全域變數字母表數量
int Tsize;
int l = 4; //4, 8, 16

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

    unsigned long long result = 0;
    int patternSize = m + 1;  // 模式的長度，例如 100 的長度是 3
    int one_bits = n;
    int bitsPerSlice = l*3;  // 每個切片的位數（如果需要支持其他位數，這裡可以修改）21一切片，不要乘開?

    // 計算應該跳過的位數
    int startBit = sliceIndex * bitsPerSlice;//100<<3 | 100
    int move = (1 << m);
    // 計算模式在切片中的起始位置
    int currentBit = startBit;

    int i;
    // 生成模式並填充到 result 中
    for (i = 0; i < one_bits; ++i) {
        result = result << patternSize | 0b100;
    }
    return result;
}


int popcount(unsigned long long i) {
    i = i - ((i >> 1) & 0x5555555555555555ULL);
    i = (i & 0x3333333333333333ULL) + ((i >> 2) & 0x3333333333333333ULL);
    return (int)((((i + (i >> 4)) & 0xF0F0F0F0F0F0F0FULL) * 0x101010101010101ULL) >> 56);
}

//int calculateHammingDistance(unsigned long long* buffer1, unsigned long long* buffer2) {
//    int hammingDistance = 0;
//    unsigned long long B_WitnessBit, BinvWitnessBit, num1, num2, xorResult;
//    int m = ceilLog2_atcgmap_Count; // 幾個0
//    int n = Tsize; // 100重複幾次
//
//    int totalSlices = (n + 20) / 21; // 計算切片 64bit 3個1組，一次處理21組
//
//    for (int i = 0; i < totalSlices; ++i) {
//
//        //取得i切片計算xor
//        num1 = buffer1[i];
//        num2 = buffer2[i];
//
//        xorResult = num1 ^ num2; // XOR 運算
//
//        //計算證人位、逆證人位
//        B_WitnessBit = generate_WitnessBit(m, n, i);
//        //bitset<64> B_WitnessBitbinary(B_WitnessBit);
//        //cout << "B_WitnessBitbinary" << ":" << B_WitnessBitbinary << endl;
//        BinvWitnessBit = ~B_WitnessBit;
//
//        hammingDistance += popcount((xorResult + BinvWitnessBit) & B_WitnessBit);
//    }
//
//    return hammingDistance;
//}
vector<int> SlidingWindow(unsigned long long* buffer1, int l,int buffer1Sizes) {
    int hammingDistance = 0;
    unsigned long long B_WitnessBit, BinvWitnessBit, xorResult;
    int m = ceilLog2_atcgmap_Count; // 幾個0
    int n = Tsize; // 100重複幾次

    unsigned long long b1 = buffer1[0];
    /*bitset<12> b1binary(b1);
    cout << b1binary << endl;*/
    std::vector<int> result;
    /*cout << "Tsize :" << Tsize << endl;
    cout << "Tsize - l + 1 :" << Tsize - l + 1 << endl;*/
    for (int i = 0; i < Tsize - l + 1; i++) {
        unsigned long long bi = buffer1[i];
        /*bitset<12> bibinary(bi);
        cout << i << " :" << bibinary << endl;
        //cout <<i<<" :" << s << endl;*/
        int distance = 0;
        xorResult = b1 ^ bi;

        B_WitnessBit = generate_WitnessBit(m, l, i);
        //bitset<64> B_WitnessBitbinary(B_WitnessBit);
        //cout << "B_WitnessBitbinary" << ":" << B_WitnessBitbinary << endl;
        BinvWitnessBit = ~B_WitnessBit;

        distance = popcount((xorResult + BinvWitnessBit) & B_WitnessBit);
        //cout << "distance :" << distance<<endl;
        result.push_back(distance);

    }
    return result;
}
int main() {
    auto s = chrono::high_resolution_clock::now();//計算字串轉換時間
    l = 5; //4, 8, 16
    unordered_map<char, unsigned char> atcgmapToBit = {
        {'A', 0},
        {'T', 1},
        {'C', 2},
        {'G', 3}
    };
    size_t atcgmap_Count = atcgmapToBit.size();

    double log2_atcgmap_Count = log2(static_cast<double>(atcgmap_Count));
    ceilLog2_atcgmap_Count = static_cast<int>(ceil(log2_atcgmap_Count));

    //int ceillog2_atcgmap_Count = ceil(log2(static_cast<int>(atcgmap_Count)));

    //string T = "GAGTCAGAGTAGAGTCAGAGTAGAGTCAGAGTAGAGTCAGAGTA";//32

    //string T = "AAAATTTTCCCCGGGG";// 16/12

    //string T = "GAGTCAGAGTA";//8 

    //string T = "GGCC";//4

    //檔案名稱
    string filename1 = "dna_500M.txt";
    ifstream infile1(filename1);       // 以讀取模式打開檔案

    // 檢查檔案是否成功打開
    if (!infile1) {
        cerr << "無法打開檔案 " << filename1 << endl;
        return 1;
    }

    stringstream buffer1;

    buffer1 << infile1.rdbuf();  // 將檔案內容讀入到 stringstream 中

    string T = buffer1.str();  // 將 stringstream 中的內容轉換為 string

    Tsize = T.length();
    //read str to ULL
    //int buffer1Sizes = (Tsize - l + 2)*(Tsize - l + 1) / 2;  // 計算需要的 buffer 數量
    int buffer1Sizes = (Tsize - l + 1);  // 計算需要的 buffer 數量
    unsigned long long* encodedBuffer1 = new unsigned long long[buffer1Sizes];
   
    for (int i = 0; i < buffer1Sizes; i++) {
        encodedBuffer1[i] = 0;
    }

    encodedBuffer1[0] = 0b111111111111;
    for (int i = 0; i < l ; i++)
        encodedBuffer1[0] = (encodedBuffer1[0]<<3 |base_lookup(T[i]));
        /*bitset<15> Buffer1binary(encodedBuffer1[0]);//l*3
        cout << "Buffer" << 0 << " :" << Buffer1binary << endl;*/
    // 對字串進行編碼
    for (int i = 1; i < Tsize -l+1; ++i) {
        unsigned long long baseBit = base_lookup(T[i+l-1]) & 0b111;  // 獲取字符的 3 位編碼
        encodedBuffer1[i] = (encodedBuffer1[i-1] << 3|baseBit);  // 將 baseBit 左移並存入對應的位置
        /*bitset<15> Buffer1binary(encodedBuffer1[i]);//l*3
        cout << "Buffer"<<i << " :" << Buffer1binary << endl;*/
    }

    auto e = chrono::high_resolution_clock::now();//計算字串轉換時間
    chrono::duration<double> da = e - s; //計算字串轉換時間

    double at = 0;
    int j = 0;
    std::vector<int> result;
    for (j = 0; j < 1; j++) {
        auto start = chrono::high_resolution_clock::now();
        result = SlidingWindow(encodedBuffer1, l, buffer1Sizes);
        auto end = chrono::high_resolution_clock::now();
        //輸出結果
        //cout << T << " 的二進制表示: "<<endl;
        //for (size_t i = 0; i < buffer1Sizes; ++i) {
        //    bitset<15> baseBitbinary(encodedBuffer1[i]);//l*3
        //    cout << baseBitbinary << endl;
        //}

        //for (int i = 0; i < result.size(); ++i) {// one substring
        //    cout << result[i] << " ";
        //}

        // 計算經過的時間
        chrono::duration<double> duration = end - start;

        // 輸出結果（以秒為單位）
        //cout << "程式執行時間: " << duration.count() << " s" << endl;
        at += duration.count();
    }
    at+= da.count();
    cout << "長度為 " << Tsize << " time : " << at << endl;
    return 0;

}