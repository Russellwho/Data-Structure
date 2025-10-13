#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
using namespace std;
template<typename T>
class MyVector {
    private:
        T* data;          // 指向儲存資料的動態陣列
        size_t sz;        // 元素個數
        size_t capacity;  // 容量大小（目前可存放元素數）

        // 私有函式：擴充容量，並把舊資料搬移過去
        void resize(size_t new_capacity) {
            T* new_data = new T[new_capacity];      // 配置新陣列
            for (size_t i = 0; i < sz; ++i)        // 拷貝舊元素
                new_data[i] = data[i];
            delete[] data;                          // 釋放舊陣列
            data = new_data;
            capacity = new_capacity;
        }

    public:
        // 建構子，初始無資料
        MyVector() : data(nullptr), sz(0), capacity(0) {}

        // 解構子，釋放動態記憶體
        ~MyVector() {
            delete[] data;
        }

        // 回傳目前元素數量
        size_t size() const {
            return sz;
        }

        // 尾端新增元素
        void push_back(const T& value) {
            if (sz == capacity) {                   // 容量不足時擴容
                size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
                resize(new_capacity);
            }
            data[sz++] = value;                     // 新增元素並更新大小
        }

        // 取得指定位置元素，不做邊界檢查
        T& operator[](size_t index) {
            return data[index];
        }

        // 移除最後一個元素
        void pop_back() {
            if (sz > 0)
                --sz;
        }
};
int main(){
    int x;
    string IN,word;
    MyVector<string> D;
    getline(cin,IN);
    stringstream ss(IN);
    while(ss>>word)
        D.push_back(word);
    x=D.size();
    int C=pow(2,x);
    bool d[C][x];
    for(int i=0;i<C;i++){
        int r=i;
        for(int j=0;j<x;j++){
            d[i][x-j-1]=r%2;
            r/=2;
        }
    }
    cout<<"\n[∅]\n";
    for(int i=1;i<C;i++){
        bool f=0;
        cout<<"[";
        for(int j=0;j<x;j++){
            if(d[i][j]){
                if(f>0)
                    cout<<",";
                f=1;
                cout<<D[j];
            }
        }
        cout<<"]\n";
    }
}