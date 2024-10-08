#include <iostream>
#include <string>
#include <bitset>
#include <cmath>// 包含 log2 函數
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <sstream>
#include <time.h>
using namespace std;

int ceilLog2_atcgmap_Count; //全域變數字母表數量

#define BITS_PER_LETTER 2  // 每個字母的位元數（2位）
#define ASCII_SIZE 128  // ASCII 查找表大小

// 查找表，用於將字母對應到位元編碼
unsigned long long lookupTable[ASCII_SIZE];

// 初始化查找表
void initLookupTable() {
    for (int i = 0; i < ASCII_SIZE; i++) {
        lookupTable[i] = -1;  // 初始化查找表中所有位置為 -1，表示尚未分配
    }
}

// 編碼 DNA 字串，並自動分配新的字母位元
void encodeDNA(const char* str, unsigned long long* nextBits) {
    unsigned long long result = 0;  // 用來儲存最終的編碼結果

    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char letter = (unsigned char)str[i];  // 讀取當前字母
        unsigned long long bits = lookupTable[letter];  // 查找該字母的位元編碼

        if (bits == -1) {
            // 如果該字母尚未編碼，為它分配新位元
            bits = (*nextBits)++;  // 分配下一個可用的位元編碼
            lookupTable[letter] = bits;  // 更新查找表，記錄新字母的位元編碼
        }

        //// 將結果左移 BITS_PER_LETTER 位，然後加入該字母的位元編碼
        //result = (result << BITS_PER_LETTER) | bits;
    }

}
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
    for (int num = 0; num < 2; num++) {
        if(num == 0)
            printf(" num=0 , switch");
        if (num == 1)
            printf(" num=1 , 查表");
        for (int l = 4; l < 8; l++) {
            //int l = 5;
            clock_t start1_time = clock();


            size_t atcgmap_Count = 4;

            double log2_atcgmap_Count = log2(static_cast<double>(atcgmap_Count));
            ceilLog2_atcgmap_Count = static_cast<int>(ceil(log2_atcgmap_Count));

            //int ceillog2_atcgmap_Count = ceil(log2(static_cast<int>(atcgmap_Count)));

            //string T = "GAGTCAGAGTA";//11 
            //cout << "T :" << T<<endl;

            // 檔案名稱
            const char* filename1 = "dna_1G.txt";
            FILE* infile1;
            errno_t err = fopen_s(&infile1, filename1, "r");  // 使用 fopen_s

            if (err != 0) {
                fprintf(stderr, "無法打開檔案 %s\n", filename1);
                return 1;
            }

            // 移動到檔案的結尾，獲取檔案大小
            fseek(infile1, 0, SEEK_END);
            long filesize = ftell(infile1);  // 獲取檔案大小
            fseek(infile1, 0, SEEK_SET);     // 返回到檔案的開頭

            // 分配內存來保存檔案內容
            char* T = (char*)malloc(filesize + 1);  // +1 是為了放置字串結尾符 '\0'
            if (T == NULL) {
                fprintf(stderr, "記憶體分配失敗\n");
                fclose(infile1);
                return 1;
            }

            // 讀取檔案內容到 T 陣列中
            fread(T, 1, filesize, infile1);
            T[filesize] = '\0';  // 確保以空字元結尾

            // 關閉檔案
            fclose(infile1);

            // 檢查內容是否正確讀取
            //printf("檔案內容已成功讀入，檔案大小為: %ld\n", filesize);

            // 請在這裡處理變數 T 中的檔案內容

            clock_t end1_time = clock();

            clock_t start2_time = clock();
            int Tsize = strlen(T);  // 計算字串 T 的長度

            //read str to ULL
            //int buffer1Sizes = (Tsize - l + 2)*(Tsize - l + 1) / 2;  // 計算需要的 buffer 數量
            int buffer1Sizes = (Tsize - l + 1);  // 計算需要的 buffer 數量

            clock_t start3_time, end3_time=0;
            if (num == 1) {
                start3_time = clock();
                initLookupTable();  // 初始化查找表
                unsigned long long nextBits = 0;  // 下一個可用的位元編碼從 0 開始

                encodeDNA(T, &nextBits);  // 編碼 DNA 字串
                end3_time = clock();
            }

            unsigned long long* encodedBuffer1 = (unsigned long long*)calloc(buffer1Sizes, sizeof(unsigned long long));
            if (num == 0)
                for (int i = 0; i < l; i++) {
                    encodedBuffer1[0] = (encodedBuffer1[0] << 3 | base_lookup(T[i]));//old
                }
            if (num == 1)
                for (int i = 0; i < l; i++) {
                    encodedBuffer1[0] = (encodedBuffer1[0] << 3 | lookupTable[T[i]]);//new
                }

            int temp = l - 1;
            // 對字串進行編碼
            if (num == 0)
                for (int i = 1; i < buffer1Sizes; ++i) {
                    unsigned long long baseBit = base_lookup(T[i + temp]) & 0b111;  // 獲取字符的 3 位編碼
                    encodedBuffer1[i] = (encodedBuffer1[i - 1]) << 3 | baseBit;  // 將 encodedBuffer1 左移
                }
            if (num == 1)
                for (int i = 1; i < buffer1Sizes; ++i) {
                    unsigned long long baseBit = lookupTable[T[i + temp]] & 0b111;  // 獲取字符的 3 位編碼
                    encodedBuffer1[i] = (encodedBuffer1[i - 1]) << 3 | baseBit;  // 將 encodedBuffer1 左移
                }

            clock_t end2_time = clock();

            double readtime = (double)(end1_time - start1_time) / CLOCKS_PER_SEC;
            double converttime = (double)(end2_time - start2_time) / CLOCKS_PER_SEC;

            int j = 0;

            int* result = (int*)malloc(buffer1Sizes * sizeof(int));
            \

                clock_t start_time, end_time;
            for (j = 0; j < 1; j++) {
                start_time = clock();
                SlidingWindow(encodedBuffer1, result, l, buffer1Sizes);
                end_time = clock();
                //輸出結果
                //cout << T << " 的二進制表示: "<<endl;
                //for (size_t i = 0; i < buffer1Sizes; ++i) {
                //    bitset<15> baseBitbinary(encodedBuffer1[i]);//l*3
                //    cout << baseBitbinary << endl;
                //}

                //for (int i = 0; i < buffer1Sizes; ++i) {// one substring
                //    cout << result[i] << " ";
                //}


                // //寫入答案 確認用
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

                printf("\n");
            }
            double distancetime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            double encodetime = (double)(end3_time - start3_time) / CLOCKS_PER_SEC;//初始化+字母編碼
            /*printf("l= %d read file time : %f\n", l, readtime);
            printf("l= %d convert time : %f\n", l, converttime);
            printf("l= %d distance time : %f\n", l, distancetime);
            printf("l= %d 分配時間 : %f\n", l, ttest);*/
            printf("%f %f %f %f\n", readtime, converttime, distancetime, encodetime);


            free(encodedBuffer1);
            encodedBuffer1 = NULL;  // C 中指針釋放後設為 NULL
            free(result);
            result = NULL;  // C 中指針釋放後設為 NULL
            free(T);

        }
    }
        return 0;
    
}