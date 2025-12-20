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