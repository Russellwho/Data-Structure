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
int F(int m,int n){
    if(m==0)
        return n+1;
    else if(n==0)
        return F(m-1,1);
    else
        return F(m-1,F(m,n-1));
}
int main(){
    MyVector<int> D;
    int x;
    for(int i=0;i<2;i++){
        cin>>x;
        D.push_back(x);
    }
    cout<<"Recursion:"<<F(D[0],D[1])<<endl;
    while(D.size()>1){
        int m=D[D.size()-2],n=D[D.size()-1];
        if(m==0){
            for(int i=0;i<2;i++)
                D.pop_back();
            D.push_back(n+1);
        }
        else if(m>0&&n==0){
            for(int i=0;i<2;i++)
                D.pop_back();
            D.push_back(m-1);
            D.push_back(1);
        }
        else{
            for(int i=0;i<2;i++)
                D.pop_back();
            D.push_back(m-1);
            D.push_back(m);
            D.push_back(n-1);
        }
        /*for(int i=0;i<D.size();i++)
            cout<<D[i]<<" ";
        cout<<endl;*/
    }
    cout<<"Non-recursive:"<<D[0]<<endl;
    return 0;
}