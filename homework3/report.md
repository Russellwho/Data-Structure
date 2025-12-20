# 41343111

作業三

## 1. 問題 多項式資料類別

### 1.1 解題說明

實作一個 Polynomial 類別，使用具 header node 的循環鏈結串列（circular linked list）與 available-space list 來表示單變數整數係數多項式，並支援多項式的輸入、輸出、複製、指定、解構、加減乘與帶值計算（Evaluate）。

策略:

1.將輸入字串解析成每項的係數與次方。

2.加法時合併同次方項目，保持降冪順序。

3.乘法時每一項交叉相乘，再合併同次方。

4.Eval 直接把數值代入多項式計算結果。

1.使用template<class T> ChainNode定義節點結構，節點中包含係數、次方與鏈結指標，用來表示多項式的單一項目。

2.以template<class T> Chain實作具header node的循環鏈結串列，作為多項式的內部表示方式，並確保各項依次方遞減順序儲存。

3.實作template<class T> ChainIterator，支援Begin()與End()操作，使鏈結串列可被逐項走訪，方便多項式運算的實作。

4.多項式加法與減法採同步走訪兩個循環串列的方式，當次方相同時合併係數，不同時依次方大小插入結果多項式。

5.多項式乘法使用雙層迴圈，將每一項交叉相乘後加入結果多項式，並即時合併相同次方的項目。

6.Evaluate 函式將指定數值代入多項式中各項，計算coef × x^exp並累加得到結果。

7.透過available-space list管理節點的配置與回收，在多項式清除或解構時將節點回收以供再次使用。

### 1.2 程式實作

```cpp
#include <iostream>
#include <cmath>
using namespace std;

// =============================
//     ChainNode
// =============================
template <class T>
class ChainNode {
public:
    T data;
    ChainNode<T>* link;

    ChainNode(const T& d = T(), ChainNode<T>* l = nullptr)
        : data(d), link(l) {}
};


// =============================
//     Available Space List
// =============================
template <class T>
class AvailList {
public:
    static ChainNode<T>* avail;

    static ChainNode<T>* GetNode() {
        if (avail) {
            ChainNode<T>* p = avail;
            avail = avail->link;
            return p;
        }
        return new ChainNode<T>();
    }

    static void RetNode(ChainNode<T>* x) {
        x->link = avail;
        avail = x;
    }
};

template <class T>
ChainNode<T>* AvailList<T>::avail = nullptr;


// =============================
//     Chain (Circular List)
// =============================
template <class T>
class Chain {
    typedef ChainNode<T> Node;

public:
    Node* head;

    Chain() {
        head = AvailList<T>::GetNode();
        head->link = head; // circular
    }

    bool empty() const {
        return head->link == head;
    }

    Node* Begin() const { return head->link; }
    Node* End() const { return head; }

    // Insert after node p
    void Insert(Node* p, const T& x) {
        Node* q = AvailList<T>::GetNode();
        q->data = x;
        q->link = p->link;
        p->link = q;
    }

    void Clear() {
        Node* p = head->link;
        while (p != head) {
            Node* next = p->link;
            AvailList<T>::RetNode(p);
            p = next;
        }
        head->link = head;
    }

    ~Chain() { Clear(); AvailList<T>::RetNode(head); }
};


// =============================
//     Chain Iterator
// =============================
template <class T>
class ChainIterator {
    typedef ChainNode<T> Node;

public:
    Node* current;
    Node* head;

    ChainIterator(Node* p, Node* h) : current(p), head(h) {}

    bool operator!=(const ChainIterator& it) const {
        return current != it.current;
    }

    T& operator*() { return current->data; }

    ChainIterator& operator++() {
        current = current->link;
        return *this;
    }
};


// =============================
//     Polynomial Term
// =============================
struct Term {
    int coef;
    int exp;
};


// =============================
//     Polynomial Class
// =============================
class Polynomial {
private:
    Chain<Term> poly;

    void Attach(int c, int e) {
        ChainNode<Term>* last = poly.head;
        while (last->link != poly.head) last = last->link;

        Term t = { c, e };
        poly.Insert(last, t);
    }

public:
    Polynomial() {}

    // Copy Constructor
    Polynomial(const Polynomial& a) {
        ChainNode<Term>* p = a.poly.Begin();
        while (p != a.poly.End()) {
            Attach(p->data.coef, p->data.exp);
            p = p->link;
        }
    }

    // Assignment
    const Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            poly.Clear();
            ChainNode<Term>* p = a.poly.Begin();
            while (p != a.poly.End()) {
                Attach(p->data.coef, p->data.exp);
                p = p->link;
            }
        }
        return *this;
    }

    // =============================
    //        Input
    // =============================
    friend istream& operator>>(istream& is, Polynomial& x) {
        x.poly.Clear();

        int n;
        is >> n;

        for (int i = 0; i < n; i++) {
            int c, e;
            is >> c >> e;
            x.Attach(c, e);
        }
        return is;
    }

    // =============================
    //        Output
    // =============================
    friend ostream& operator<<(ostream& os,const Polynomial& x) {
        ChainNode<Term>* p = x.poly.Begin();
        bool first = true;

        while (p != x.poly.End()) {
            if (!first) os << " + ";
            if(p->data.exp!=0)
                os << p->data.coef << "x^" << p->data.exp;
            else
                os << p->data.coef;
            first = false;
            p = p->link;
        }
        return os;
    }

    // =============================
    //        Addition
    // =============================
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        ChainNode<Term>* p = poly.Begin();
        ChainNode<Term>* q = b.poly.Begin();

        while (p != poly.End() && q != b.poly.End()) {
            if (p->data.exp == q->data.exp) {
                int c = p->data.coef + q->data.coef;
                if (c != 0) result.Attach(c, p->data.exp);
                p = p->link;
                q = q->link;
            } else if (p->data.exp > q->data.exp) {
                result.Attach(p->data.coef, p->data.exp);
                p = p->link;
            } else {
                result.Attach(q->data.coef, q->data.exp);
                q = q->link;
            }
        }

        while (p != poly.End()) {
            result.Attach(p->data.coef, p->data.exp);
            p = p->link;
        }
        while (q != b.poly.End()) {
            result.Attach(q->data.coef, q->data.exp);
            q = q->link;
        }

        return result;
    }

    // =============================
    //       Subtraction
    // =============================
    Polynomial operator-(const Polynomial& b) const {
        Polynomial negB = b;
        ChainNode<Term>* p = negB.poly.Begin();
        while (p != negB.poly.End()) {
            p->data.coef = -p->data.coef;
            p = p->link;
        }
        return (*this + negB);
    }

    // =============================
    //       Multiplication
    // =============================
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;

        for (ChainNode<Term>* p = poly.Begin(); p != poly.End(); p = p->link) {
            for (ChainNode<Term>* q = b.poly.Begin(); q != b.poly.End(); q = q->link) {
                result.AddTerm(p->data.coef * q->data.coef, p->data.exp + q->data.exp);
            }
        }

        return result;
    }

    void AddTerm(int c, int e) {
        if (c == 0) return;

        ChainNode<Term>* p = poly.head;

        // find position to insert
        while (p->link != poly.head && p->link->data.exp > e)
            p = p->link;

        if (p->link != poly.head && p->link->data.exp == e) {
            p->link->data.coef += c;
            if (p->link->data.coef == 0) {
                ChainNode<Term>* temp = p->link;
                p->link = temp->link;
                AvailList<Term>::RetNode(temp);
            }
        } else {
            Term t = { c, e };
            poly.Insert(p, t);
        }
    }

    // =============================
    //       Evaluate
    // =============================
    float Evaluate(float x) const {
        float sum = 0.0;
        ChainNode<Term>* p = poly.Begin();
        while (p != poly.End()) {
            sum += p->data.coef * pow(x, p->data.exp);
            p = p->link;
        }
        return sum;
    }
};

int main() {
    Polynomial A, B;
    cout << "請輸入多項式 A (格式：n c1 e1 c2 e2 ...): ";
    cin >> A;
    cout << "請輸入多項式 B (格式：n c1 e1 c2 e2 ...): ";
    cin >> B;
    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    cout << "A + B = " << (A + B) << endl;
    cout << "A - B = " << (A - B) << endl;
    cout << "A * B = " << (A * B) << endl;
    cout << "A(2) = " << A.Evaluate(2) << endl;
}
```

### 1.3 效能分析

| 函式 | 時間複雜度 | 空間複雜度 |
| ------------------- | ---------- | ---------- |
| Add（operator+） | O(n1 + n2) | O(n1 + n2) |
| Sub（operator-） | O(n1 + n2) | O(n1 + n2) |
| Mult（operator*） | O(n1 * n2) | O(n1 * n2) |
| Eval（Evaluate） | O(n) | O(1) |
| print（operator<<） | O(n) | O(1) |
| 輸入解析（operator>>） | O(n) | O(n) |
| Copy Constructor | O(n) | O(n) |
| Assignment Operator | O(n) | O(1) |

### 1.4 測試與驗證

輸入多項式：
3 3 3 2 2 1 0
3 2 2 4 1 5 0

運算結果：
A = 3x^3 + 2x^2 + 1
B = 2x^2 + 4x^1 + 5
A + B = 3x^3 + 4x^2 + 4x^1 + 6
A - B = 3x^3 + -4x^1 + -4
A * B = 6x^5 + 16x^4 + 23x^3 + 12x^2 + 4x^1 + 5
A(2) = 33

### 1.5 申論與開發報告

多項式的加法運算採用逐項比較次方的方式，對應次方相同的項目會進行係數相加，不同次方則直接保留原項，最終結果維持降冪排列。這種方法確保了運算邏輯清晰且操作簡單。

乘法運算中，每一項會與另一多項式的所有項交叉相乘，產生的新項再依次方合併相同項，若係數為零則移除，以保持結果的正確性與整潔性。由於運算僅針對多項式中現有的非零項進行，計算主要集中在項目的管理與合併上。當多項式項數增加時，運算時間與臨時儲存空間需求也會增加，因此在大規模應用中，可能需要採用更高效的資料結構或演算法來優化性能。

## 程式實作

[問題一](#12-程式實作)

## 效能分析

[問題一](#13-效能分析)

## 測試與驗證

[問題一](#14-測試與驗證)

## 解題說明

[問題一](#11-解題說明)

## 申論及開發報告

[問題一](#15-申論與開發報告)