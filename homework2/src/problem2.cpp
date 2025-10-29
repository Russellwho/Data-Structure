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
class Term {
        friend class Polynomial;
    private:
        float coef; //係數
        int exp; //次方
};
class Polynomial {
    private:
        Term terms[999]; //最多支援999項
        int n; //實際項數
    public:
        Polynomial() : n(0) {}

        friend istream& operator>>(istream& in, Polynomial& poly) {
            string line;
            getline(in, line);
            if (line[0] != '+' && line[0] != '-')
                line.insert(0, "+");
            for (int i = line.size() - 1; i > 0; --i)
                if (line[i] == '+' || line[i] == '-') line.insert(i, " ");
            stringstream ss(line);
            string termStr;
            poly.n = 0;
            while (ss >> termStr) {
                float coef = 0;
                int exp = 0;
                size_t xPos = termStr.find('x');
                if (xPos == string::npos) { //常數
                    coef = stof(termStr);
                    exp = 0;
                } else {
                    string coefStr = termStr.substr(0, xPos);
                    if (coefStr == "+" || coefStr == "") coef = 1;
                    else if (coefStr == "-") coef = -1;
                    else coef = stof(coefStr);
                    size_t expPos = termStr.find('^');
                    if (expPos == string::npos) exp = 1;
                    else exp = stoi(termStr.substr(expPos + 1));
                }
                poly.terms[poly.n].coef = coef;
                poly.terms[poly.n].exp = exp;
                poly.n++;
            }
            return in;
        }

        friend ostream& operator<<(ostream& out, const Polynomial& poly) {
        for (int i = 0; i < poly.n; ++i) {
            if (i > 0 && poly.terms[i].coef >= 0)
                out << "+";
            out << poly.terms[i].coef;
            if (poly.terms[i].exp != 0)
                out << "x";
            if (poly.terms[i].exp > 1)
                out << "^" << poly.terms[i].exp;
        }
        return out;
    }
};
int main() {
    Polynomial p;
    cout << "Input: ";
    cin >> p; 
    cout << "Output: " << p << endl; 
}
/*
+3x^3+2x^2+1
+2x^2+4x+5
*/