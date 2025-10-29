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
class MyVector{
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

        // 拷貝建構子，用於Polynommial C=A;）
        MyVector(const MyVector& other) {
            sz = other.sz;
            capacity = other.capacity;
            if (capacity > 0)
                data = new T[capacity];
            else
                data = nullptr;
            for (size_t i = 0; i < sz; ++i)
                data[i] = other.data[i];
        }
        MyVector& operator=(const MyVector& other) {
            if (this == &other) return *this; // 自我賦值
            delete[] data;                     // 先釋放舊資源
            sz = other.sz;
            capacity = other.capacity;
            if (capacity > 0) {
                data = new T[capacity];
                for (size_t i = 0; i < sz; i++)
                    data[i] = other.data[i];   // 這裡會用 T 的拷貝建構子
            } else {
                data = nullptr;
            }
            return *this;
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
        const T& operator[](size_t index)const{
        return data[index];
        }

        // 移除最後一個元素
        void pop_back() {
            if (sz > 0)
                --sz;
        }

        void clear(){
            sz=0;
        }

        void erase(int x){
            while(x<sz-1){
                data[x]=data[x+1];
                x++;
            }
            sz--;
        }

        T* begin(){
            return data;
        }

        void insert(int x,T value){
            if (sz == capacity)
                resize(capacity == 0 ? 1 : capacity * 2);
            for(int i=sz;i>x;i--)
                data[i]=data[i-1];
            data[x]=value;
            sz++;
        }
};
class Polynommial{
    private:
        MyVector<MyVector<int>> data;
    public:
        Polynommial(string D){
            stringstream ss(D);
            string text;
            while(ss>>text){
                MyVector<int> R;
                for(int i=text.length()-1;i>=0;i--)
                    if(text[i]==' ')
                        text.erase(i,1);
                if(text[text.size()-1]=='x')
                    text.insert(text.size(),"^1");
                if(text.find('x')==string::npos){
                    if(text=="+")
                        R.push_back(1);
                    else if(text=="-")
                        R.push_back(-1);
                    else
                        R.push_back(stoi(text));
                    R.push_back(0);
                }
                else{
                    int p=text.find('x');
                    if(text.substr(0,p)=="+"||text.substr(0,p)=="")
                        R.push_back(1);
                    else if(text.substr(0,p)=="-")
                        R.push_back(-1);
                    else
                        R.push_back(stoi(text.substr(0,p)));
                    p=text.find('^');
                    R.push_back(stoi(text.substr(p+1)));
                }
                data.push_back(R);
            }
        }

        Polynommial Add(const Polynommial& poly){
            Polynommial R = *this;
            for(int i=0;i<poly.data.size();i++){
                bool F=1; //判斷是否需要新增系數
                for(int j=0;j<data.size();j++){
                    if(data[j][1]==poly.data[i][1]){
                        data[j][0]+=poly.data[i][0];
                        F=0;
                        break;
                    }
                }
                if(F){
                    if(poly.data[i][1]>data[0][1])
                        data.insert(0,poly.data[i]);
                    else if (poly.data[i][1]<data[data.size()-1][1])
                        data.push_back(poly.data[i]);
                    else
                        for(int j=1;j<data.size();j++)
                            if(data[j-1][1]>poly.data[i][1]&&poly.data[i][1]>data[j][1])
                                data.insert(j,poly.data[i]);
                }
            }
            for(int i=data.size()-1;i>=0;i--)
                if(data[i][0]==0)
                    data.erase(i);
            swap(R,*this);
            return R;
        }

        Polynommial Mult(Polynommial poly){
            Polynommial R = *this;
            MyVector<MyVector<int>> R1=data;
            for(int i=0;i<data.size();i++){
                data[i][0]*=poly.data[0][0];
                data[i][1]+=poly.data[0][1];
            }
            for(int i=1;i<poly.data.size();i++){
                MyVector<MyVector<int>> R2=R1;
                for(int j=0;j<R2.size();j++){
                    R2[j][0]*=poly.data[i][0];
                    R2[j][1]+=poly.data[i][1];
                }
                for(int j=0;j<R2.size();j++){
                    bool F=1; //判斷是否需要新增系數
                    for(int k=0;k<data.size();k++){
                        if(data[k][1]==R2[j][1]){
                            data[k][0]+=R2[j][0];
                            F=0;
                            break;
                        }
                    }
                    if(F){
                        if(R2[j][1]>data[0][1])
                            data.insert(0,R2[j]);
                        else if (R2[j][1]<data[data.size()-1][1])
                            data.push_back(R2[j]);
                        else
                            for(int k=1;k<data.size();k++)
                                if(data[k-1][1]>R2[j][1]&&R2[j][1]>data[k][1])
                                    data.insert(k,R2[j]);
                    }
                }
            }
            for(int i=data.size()-1;i>=0;i--)
                if(data[i][0]==0)
                    data.erase(i);
            swap(R,*this);
            return R;
        }

        float Eval(float f){
            float ans=0;
            for(int i=0;i<data.size();i++)
                ans+=data[i][0]*pow(f,data[i][1]);
            return ans;
        }

        void print(){
            for(int i=0;i<data.size();i++){
                if(i&&data[i][0]>0)
                    cout<<"+";
                cout<<data[i][0];
                if(data[i][1]>0)
                    cout<<"x";
                if(data[i][1]>1)
                    cout<<"^"<<data[i][1];
            }
            cout<<"\n";
//            cout<<"項數:"<<data.size()<<"\n"; //檢查資料用
        }
};
int main(){
    string in,a,b;
    for(int i=0;i<2;i++){ //輸入多項式並規格化
        getline(cin,in);
        if(in[0]!='-'&&in[0]!='+')
            in.insert(0,"+");
        for(int j=in.size()-1;j>0;j--)
            if(in[j]=='+'||in[j]=='-')
                in.insert(j," ");
        i==0?a=in:b=in;
    }
    Polynommial A(a);
    Polynommial B(b);
    Polynommial C=A.Add(B);
    Polynommial D=A.Mult(B);
    cout<<"\nInput 1:\n";
    A.print();
    cout<<"Input 2:\n";
    B.print();
    cout<<"Add:\n";
    C.print();
    cout<<"Mult:\n";
    D.print();
    cout<<"Eval: N=";
    float N;
    cin>>N;
    cout<<"N->Eval:"<<D.Mult(N)<<endl;
}
/*
+3x^3+2x^2+1
+2x^2+4x+5
Add: 3x^3+4x^2+4x+6
Mult: 6x^5+16x^4+23x^3+12x^2+4x+5
*/