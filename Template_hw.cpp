#pragma GCC optimize("Ofast","unroll-loops")
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define pb push_back

// Polynomial
template <typename T, int Deg>
class Polynomial{
public:
    T coef[Deg+1];

    Polynomial() {
        for (int i = 0; i <= Deg; ++i) coef[i] = 0;
    }
    
    T& operator[](int i) {
        return coef[i];
    }

    string operator()(char x) {
        string ret = "";
        for (int i = Deg; i >= 0; --i) {
            if (!coef[i]) continue;
            if (!ret.empty()) {
                if (coef[i] > 0) ret += "+";
                else ret += "-";
            }
            if (coef[i] == 1) {
                string s(1,x);
                if (i > 1) ret += (s+"^"+to_string(i));
                else if (i == 1) ret += (s);
                else ret += to_string(1);
            } else {
                if (i > 1) ret += (to_string(abs(coef[i]))+x+"^"+to_string(i));
                else if (i == 1) ret += (to_string(abs(coef[i]))+x);
                else ret += (to_string(abs(coef[i])));
            }
        }
        if (ret.empty()) return "0";
        return ret;
    }
    
    template <typename XT>
    XT operator()(XT x) {
        XT sum = XT(0), bs = XT(1);
        for (int i = 0; i <= Deg; ++i) {
            sum = sum + coef[i]*bs;
            bs = bs * x;
        }
        return sum;
    }
};

// Matrix
template <typename T, int Row, int Column>
class Matrix{
public:
    T arr[Row*Column];

    Matrix() {}
    Matrix(int val) {
        for (int i = 0; i < Row; ++i)
            for (int j = 0; j < Column; ++j)
                arr[i*Row+j] = (i == j)?val:0;
    }
    
    class SingleRow{
    public:
        T* RowStart;

        SingleRow(T* start) : RowStart(start) {}

        T& operator[](int j) {
            return RowStart[j];
        }
    };
    SingleRow operator[](int i) {
        return SingleRow(arr+i*Row);
    }

    T det() {
        if (Row == 1) return arr[0];
        else if (Row == 2) return arr[0]*arr[3]-arr[1]*arr[2];
    }
    
    T tr() {
        T sum = T(0);
        for (int i = 0; i < Row; ++i) sum = sum+arr[i*Row+i];
        return sum;
    }

    Polynomial<T, Row> char_poly() {
        Polynomial<T, Row> ret;
        ret[2] = 1;
        ret[1] = -tr();
        ret[0] = det();
        return ret;
    }
};

// Matrix operators
template<typename T, int Row, int Column>// cA
Matrix<T,Row,Column> operator*(int c, Matrix<T,Row,Column> A) {
    for (int i = 0; i < Row; ++i) {
        for (int j = 0; j < Column; ++j) {
            A[i][j] *= c;
        }
    }
    return A;
}

template<typename T, int Row, int Column>// Ac
Matrix<T,Row,Column> operator*(Matrix<T,Row,Column> A, int c) {
    for (int i = 0; i < Row; ++i) {
        for (int j = 0; j < Column; ++j) {
            A[i][j] *= c;
        }
    }
    return A;
}

template<typename T, int Row, int Column>// A+B
Matrix<T,Row,Column> operator+(Matrix<T,Row,Column> A, Matrix<T,Row,Column> B) {
    Matrix<T,Row,Column> ret(0);
    for (int i = 0; i < Row; ++i) {
        for (int j = 0; j < Column; ++j) {
            ret[i][j] = A[i][j]+B[i][j];
        }
    }
    return ret;
}

template<typename T, int Row, int Column>// A*B
Matrix<T,Row,Column> operator*(Matrix<T,Row,Column> A, Matrix<T,Row,Column> B) {
    Matrix<T,Row,Column> ret(0);
    for (int i = 0; i < Row; ++i) {
        for (int j = 0; j < Column; ++j) {
            for (int k = 0; k < Row; ++k) {// Using Column should also work.
                ret[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    return ret;
}

template <typename T, int Row, int Column>// Matrix ostream
ostream& operator<<(ostream& mos, Matrix<T, Row, Column> mat) {
    for (int i = 0; i < Row; ++i) {
        mos << '[';
        for (int j = 0; j < Column; ++j) {
            mos << setw(5) << mat[i][j];
            if (j < Column-1) cout<<',';
        }
        mos << "]\n";
    }
    return mos;
}

int main() {
    srand(time(NULL));
    Matrix<int, 2, 2> A;
    A[0][0] = rand() % 100;
    A[0][1] = rand() % 100;
    A[1][0] = rand() % 100;
    A[1][1] = rand() % 100;
    cout << "Matrix A is" << endl;
    cout << A << endl;
    Polynomial poly = A.char_poly();
    cout << "The characteristic polynomial of A is" << endl;
    cout << poly('x') << endl;
    cout << "P(-1) = " << poly(-1) << endl;
    cout << "P(0) = " << poly(0) << endl;
    cout << "P(1) = " << poly(1) << endl;
    cout << "P(A) is " << endl;
    cout << poly(A) << endl;
}
