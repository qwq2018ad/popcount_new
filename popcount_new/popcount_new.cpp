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
unsigned long long generate_WitnessBit(int m, int l) {

    unsigned long long result = 0;
    int patternSize = m + 1;  // 模式的長度，例如 100 的長度是 3
    
    // 生成模式並填充到 result 中
    for (int i = 0; i < l; ++i) {
        result = result << patternSize | 0b100;
    }
    return result;
}


int popcount(unsigned long long i) {
    i = i - ((i >> 1) & 0x5555555555555555ULL);
    i = (i & 0x3333333333333333ULL) + ((i >> 2) & 0x3333333333333333ULL);
    return (int)((((i + (i >> 4)) & 0xF0F0F0F0F0F0F0FULL) * 0x101010101010101ULL) >> 56);
}

void SlidingWindow(unsigned long long* buffer1, int* result, int l,int buffer1Sizes) {
    unsigned long long B_WitnessBit, BinvWitnessBit, xorResult;
    int m = ceilLog2_atcgmap_Count; // 幾個0

    unsigned long long b1 = buffer1[0];

    B_WitnessBit = generate_WitnessBit(m, l);
    BinvWitnessBit = ~B_WitnessBit;//準備 逆證人位 popcount 計算

    for (int i = 0; i < buffer1Sizes; i++) {
        unsigned long long bi = buffer1[i];
        /*bitset<24> bibinary(bi);
        cout << i << " :" << bibinary << endl;*/
        xorResult = b1 ^ bi;

        result[i] = popcount((xorResult + BinvWitnessBit) & B_WitnessBit);

    }
}
int main() {
    //for (int l = 4; l < 17; l++) {
    int l = 4;
        auto rt1 = chrono::high_resolution_clock::now();//read file time

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

        //string T = "GAGTCAGAGTA";//11 
        //cout << "T :" << T<<endl;

        //檔案名稱
        string filename1 = "dna_1G.txt";
        ifstream infile1(filename1);       // 以讀取模式打開檔案

        // 檢查檔案是否成功打開
        if (!infile1) {
            cerr << "無法打開檔案 " << filename1 << endl;
            return 1;
        }

        stringstream buffer1;

        buffer1 << infile1.rdbuf();  // 將檔案內容讀入到 stringstream 中

        string T = buffer1.str();  // 將 stringstream 中的內容轉換為 string

        auto rt2 = chrono::high_resolution_clock::now();//read file time

        auto cont1 = chrono::high_resolution_clock::now();//convert time
        int Tsize = T.length();
        //read str to ULL
        //int buffer1Sizes = (Tsize - l + 2)*(Tsize - l + 1) / 2;  // 計算需要的 buffer 數量
        int buffer1Sizes = (Tsize - l + 1);  // 計算需要的 buffer 數量
        unsigned long long* encodedBuffer1 = new unsigned long long[buffer1Sizes];



        for (int i = 0; i < buffer1Sizes; i++) {
            encodedBuffer1[i] = 0;
        }//maybe 可刪除

        encodedBuffer1[0] = 0;
        for (int i = 0; i < l; i++)
            encodedBuffer1[0] = (encodedBuffer1[0] << 3 | base_lookup(T[i]));
        auto abc1 = chrono::high_resolution_clock::now();//read file time


        int temp = l - 1;
        // 對字串進行編碼
        for (int i = 1; i < buffer1Sizes; ++i) {
            unsigned long long baseBit = base_lookup(T[i + temp]) & 0b111;  // 獲取字符的 3 位編碼
            encodedBuffer1[i] = (encodedBuffer1[i - 1]) << 3 | baseBit;  // 將 encodedBuffer1 左移
        }
        auto abc2 = chrono::high_resolution_clock::now();//read file time

        auto cont2 = chrono::high_resolution_clock::now();//convert time

        chrono::duration<double> rt3 = rt2 - rt1; //read file time
        chrono::duration<double> cont3 = cont2 - cont1; //convert time
        chrono::duration<double> abc3 = abc2 - abc1; //convert time
        int j = 0;
        double distancetime = 0;
        int* result = new int[buffer1Sizes];
        for (j = 0; j < 1; j++) {
            auto start = chrono::high_resolution_clock::now();
            SlidingWindow(encodedBuffer1, result, l, buffer1Sizes);
            auto end = chrono::high_resolution_clock::now();
            //輸出結果
            //cout << T << " 的二進制表示: "<<endl;
            //for (size_t i = 0; i < buffer1Sizes; ++i) {
            //    bitset<15> baseBitbinary(encodedBuffer1[i]);//l*3
            //    cout << baseBitbinary << endl;
            //}

            //for (int i = 0; i < buffer1Sizes; ++i) {// one substring
            //    cout << result[i] << " ";
            //}


            // 寫入答案 確認用
            // std::ofstream outFile("output.txt");
            //if (outFile.is_open()) {
            //    // 將陣列的每個元素寫入文件，並用空格分隔
            //    for (int i = 0; i < buffer1Sizes; ++i) {
            //        outFile << result[i];
            //    }
            //    outFile.close(); // 完成後關閉文件
            //    std::cout << "陣列已成功寫入 output.txt 文件！" << std::endl;
            //}
            //else {
            //    std::cout << "無法打開文件。" << std::endl;
            //}

            cout << endl;
            // 計算經過的時間
            chrono::duration<double> duration = end - start;

            // 輸出結果（以秒為單位）
            //cout << "程式執行時間: " << duration.count() << " s" << endl;
            distancetime += duration.count();
        }
        double converttime, readtime, ttest = 0;
        readtime = rt3.count();
        converttime = cont3.count();

        ttest = abc3.count();
        cout << "l= " << l << " read file time : " << readtime << endl;
        cout << "l= " << l << " convert time : " << converttime << endl;
        cout << "l= " << l << " distance time : " << distancetime << endl;
        cout << "l= " << l << " ttest : " << ttest << endl;
        delete[] encodedBuffer1;
        encodedBuffer1 = nullptr; // 將指針設為 nullptr，避免懸空指針
        delete[] result;
        result = nullptr; // 將指針設為 nullptr，避免懸空指針
        //}
        return 0;
    
}