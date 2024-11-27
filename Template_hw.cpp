#pragma GCC optimize("Ofast","unroll-loops")
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define pb push_back

const int sz = 2;

// Polynomial
template <typename T, int Deg>
class Polynomial{
public:
    T coef[Deg+1];

    Polynomial() {}
    Polynomial(int val) {
        for (int i = 0; i <= Deg; ++i) coef[i] = val;
    }
    
    T& operator[](int i) {
        return coef[i];
    }

    string operator()(char x) {
        cout << "P:";
        for (int i = Deg; i >= 0; --i) cout << ' ' << coef[i];
        cout << endl;
        
        string ret = "";
        for (int i = Deg; i >= 0; --i) {
            if (!coef[i]) continue;
            if (!ret.empty() && coef[i] > 0) ret += "+";
            else if (coef[i] < 0) ret += "-";
            
            if (abs(coef[i]) == 1) {
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
    Matrix(T val) {
        for (int i = 0; i < Row; ++i)
            for (int j = 0; j < Column; ++j)
                arr[i*Row+j] = (i == j)?val:T(0);
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
        if constexpr (Row == 1) return arr[0];
        else if constexpr (Row == 2) return arr[0]*arr[3]-arr[1]*arr[2];
        else {
            T ret = T(0);
            for (int col = 0; col < Row; ++col) {
                // Create submatrix
                Matrix<T, Row-1, Column-1>  submat;
                for (int i = 1; i < Row; ++i) {
                    int subCol = 0;
                    for (int j = 0; j < Row; ++j) {
                        if (j == col) continue;
                        submat[i - 1][subCol++] = arr[i*Row+j];
                    }
                }
                // Recursive call
                ret = ret+(col % 2 == 0 ? 1 : -1) * arr[col] * submat.det();
            }
            return ret;
        }
    }
    
    Polynomial<T, Row> char_poly() {
        Polynomial<T, Row> ret(0);
        Matrix<Polynomial<T, Row>, Row, Column> mat(Polynomial<T, Row>(0));
        for (int i = 0; i < Row; ++i) {
            for (int j = 0; j < Row; ++j) {
                if (i == j) {
                    mat[i][j][0] = -1*arr[i*Row+j];
                    mat[i][j][1] = T(1);
                } else {
                    mat[i][j][0] = -1*arr[i*Row+j];
                }
            }
        }
        ret = mat.det();
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

// Polynomial operators
template <typename T, int Deg>
Polynomial<T, Deg> operator+(Polynomial<T, Deg> A, Polynomial<T, Deg> B) {
    Polynomial<T, Deg> ret(0);
    for (int i = 0; i <= Deg; ++i) ret[i] = A[i]+B[i];
    return ret;
}

template <typename T, int Deg>
Polynomial<T, Deg> operator-(Polynomial<T, Deg> A, Polynomial<T, Deg> B) {
    Polynomial<T, Deg> ret(0);
    for (int i = 0; i <= Deg; ++i) ret[i] = A[i]-B[i];
    return ret;
}

template <typename T, int Deg>
Polynomial<T, Deg> operator*(Polynomial<T, Deg> A, T c) {
    Polynomial<T, Deg> ret(0);
    for (int i = 0; i <= Deg; ++i) ret[i] = c*A[i];
    return ret;
}

template <typename T, int Deg>
Polynomial<T, Deg> operator*(T c, Polynomial<T, Deg> A) {
    return A*c;
}

template <typename T, int Deg>
Polynomial<T, Deg> operator*(Polynomial<T, Deg> A, Polynomial<T, Deg> B) {
    Polynomial<T, Deg> ret(0);
    for (int i = 0; i <= Deg; ++i) {
        for (int j = 0; j <= Deg; ++j) {
            if (i+j <= Deg) {
                ret[i+j] += A[i]*B[j];
            }
        }
    }
    return ret;
}

int main() {
    srand(time(NULL));
    Matrix<int, sz, sz> A;
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            A[i][j] = rand()%100;
        }
    }
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

