# 41343111

作業一

## 1. 問題 1 Ackermann's Function

### 1.1 解題說明

本題要求實現Ackermann函數，並且分別透過遞迴及非遞迴方式實現。

#### 1.1.1 解題策略

1. 遞迴:
    - 依照Ackermann定義直接寫出遞迴函式。

2. 非遞迴:
    - 觀察到遞迴函式中Ackermann函式會不斷呼叫自己並且呼叫時僅會在第二個參數位置額外呼叫Ackermann函式，因此我藉此設計一個 Vector(`processing[2]`) 以及一個 `is_constant` 常數標記(用於定義目前是否將 `processing[1]` 視為常數並忽略 `processing[0]` )作為計算 $A(m,n)$ 迭代所用暫存器。
    - 當條件一成立時，將 `processing` 中待處裡堆疊中取出一個資料繼續執行。
    - 當條件二成立時，只需照定義修改 `processing` 的值。
    - 當觸發條件三時，將原先的表達式 $A(m-1,A(m,n-1))$ 第一個參數排入待處裡堆疊(FILO)。

### 1.2 程式實作

```cpp
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
```

### 1.3 效能分析

1. 遞迴版本:
   1. 時間複雜度:
    - $O(1)$ ，m = 0
    - $O(n)$ ，1 <= m <= 2
    - $O({\displaystyle (2\uparrow ^{m-2}(n+3))-3})$ ，m >= 3
   2. 空間複雜度:
    - $O(1)$ ，m = 0
    - $O(n)$ ，1 <= m <= 2
    - $O({\displaystyle (2\uparrow ^{m-2}(n+3))-3})$ ，m >= 3
2. 非遞迴版本:
   1. 時間複雜度:
    - 與遞迴版本相同，因為每一步計算量與遞迴版一致。
   2. 空間複雜度:
    - $O(1)$ ，使用固定陣列模擬堆疊（僅適用於小範圍 m,n）
    - $O({\displaystyle (2\uparrow ^{m-2}(n+3))-3})$ ，使用動態向量/堆疊儲存呼叫狀態

### 1.4 測試與驗證

#### 1.4.1 測試案例

| 案例編號 | 輸入參數 $m$ | 輸入參數 $n$ | 預期輸出 | 實際輸出(遞迴版本) | 實際輸出(非遞迴版本) |
| - | - | - | - | - | - |
| 1 | $m = 0$ | $n = 0$ | 1 | 1 | 1 |
| 2 | $m = 0$ | $n = 3$ | 4 | 4 | 4 |
| 3 | $m = 1$ | $n = 0$ | 2 | 2 | 2 |
| 4 | $m = 1$ | $n = 3$ | 5 | 5 | 5 |
| 5 | $m = 2$ | $n = 4$ | 11 | 11 | 11 |
| 6 | $m = 3$ | $n = 0$ | 5 | 5 | 5 |
| 7 | $m = 3$ | $n = 3$ | 61 | 61 | 61 |
| 8 | $m = 3$ | $n = 4$ | 125 | 125 | 125 |
| 9 | $m = 4$ | $n = 0$ | 13 | 13 | 13 |
| 10 | $m = 4$ | $n = 1$ | 65533 | 65533 | 65533 |
| 11 | $m = 4$ | $n = 2$ | ${2\uparrow \uparrow 5-3}$ | error | error |

#### 1.4.2 執行指令

```shell
$ 0 0
Recursion:1
Non-recursion:1
$ 4 1
Recursion:65533
Non-recursion:65533
```

#### 1.4.3 結論

1. 在 $(m <= 2) \vee (m <= 3 \land n <= 14) \vee (m <= 4 \land n <= 1) \vee (m = 5 \land n = 0)$ 時正確運行。
2. 1.限定範圍外將出現錯誤(超過C/C++內建類型值上限、記憶體堆疊溢位)。

### 1.5 申論與開發報告

遞迴的實現簡單直接，非遞迴則依靠了類堆疊的手段進行操作。

據觀察，實際需要計算時只會在最末端進行計算，因此使用類堆疊
的方式對末端進行運算，直到總長度為1的時候，則為答案。

若要計算更大的數則需要能存更大值的自訂類型(或程式庫)。

## 2. 問題 2 Powerset

### 2.1 解題說明

本題要求透過遞迴方式實現冪集合的方法，能夠根據指定集合產生另一個集合，且產生的集合為原集合之冪集合。

冪集合，包含了原集合中所有可能的子集合。

#### 2.1.1 解題策略

透過觀察可以發現，冪集合與原集合有密切的關聯性：
- 最小的子集合為空集合[∅]
- 最大的子集合為原集合本身

若將每個子集合視為原集合中每個元素的「存在與否」組合，就可以與二進位表示對應：
- 假設原集合有 4 個元素，索引分別為 0~3
- 「1111₂」表示所有元素存在（即原集合本身）
- 「0000₂」表示所有元素不存在（空集合）

因此，只要將二進位數值從 0 遞增到 $2^n−1$，即可系統性地生成所有子集合。

### 2.2 程式實作

```cpp
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
    return 0;
}
```

### 2.3 效能分析

時間複雜度: $O(x·2^x)$

空間複雜度: $O(x·2^x)$

### 2.4 測試與驗證

#### 2.4.1 測試案例

| 案例編號 | 輸入參數 $size$ | 輸入參數 $data$ | 輸出 |
| - | - | - | - |
| 1 | 0 | - | [∅] |
| 2 | 1 | a | [∅] [a] |
| 3 | 3 | a b c | [∅] [c] [b] [b,c] [a] [a,c] [a,b] [a,b,c] |
| 4 | 4 | a bb ab ccc | [∅] [ccc] [ab] [ab,ccc] [bb] [bb,ccc] [bb,ab] [bb,ab,ccc] [a] [a,ccc] [a,ab] [a,ab,ccc] [a,bb] [a,bb,ccc] [a,bb,ab] [a,bb,ab,ccc] |

#### 2.4.2 執行指令

```shell
$ a bb ab ccc
[∅] [ccc] [ab] [ab,ccc] [bb] [bb,ccc] [bb,ab] [bb,ab,ccc] [a] [a,ccc] [a,ab] [a,ab,ccc] [a,bb] [a,bb,ccc] [a,bb,ab] [a,bb,ab,ccc]
```

### 2.5 申論與開發報告

為了系統化生成冪集合，我採用了二進位表示法：將每個子集合對應到原集合元素「存在與否」的二進位數值。

每個二進位位元對應原集合中一個元素

1 表示該元素存在，0 表示不存在

遞增遍歷二進位數值，即可生成所有可能的子集合

在程式中，我建立了一個布林矩陣 d[C][x] 來儲存每個二進位組合的元素存在情況，避免在每次輸出時重複計算。

C = 2^x 為子集合總數

x 為原集合元素數量

主程式依序遍歷矩陣，依布林值判斷是否輸出對應元素，從而列出完整的冪集合，包含空集合 [∅]。

程式設計上，實現了以下特性：

系統化生成冪集合：利用二進位表示法與布林矩陣避免冗長的巢狀迴圈

簡單易讀的輸出：每個子集合用中括號表示，元素間以逗號分隔，空集合單獨列出

觀察程式輸出可以發現，冪集合並沒有依子集合大小或元素順序排序。雖然數學上冪集合無需排序，但若需要更清楚的可視化，可進一步新增排序功能，例如對集合元素進行排序或建立比較運算子。

此外，程式中 時間複雜度為 O(x·2^x)，空間複雜度亦為 O(x·2^x)，符合理論上生成所有子集合的最佳效率。

## 程式實作

[問題一](#12-程式實作)

[問題二](#22-程式實作)

## 效能分析

[問題一](#13-效能分析)

[問題二](#23-效能分析)

## 測試與驗證

[問題一](#14-測試與驗證)

[問題二](#24-測試與驗證)

## 解題說明

[問題一](#11-解題說明)

[問題二](#21-解題說明)

## 申論及開發報告

[問題一](#15-申論與開發報告)

[問題二](#25-申論與開發報告)