#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;
class C1 {
private:
    double re, im;
public:
    C1() : re(3.14159265358979323846), im(0) {}
    C1(double r, double i) : re(r), im(i) {}
    C1 operator+(const C1& other) const {
        return C1(re + other.re, im + other.im);
    }
    C1 operator*(const C1& other) const {
        double r = re * other.re - im * other.im;
        double i = re * other.im + im * other.re;
        return C1(r, i);
    }
    C1 operator/(const C1& other) const {
        double denom = other.re * other.re + other.im * other.im;
        double r = (re * other.re + im * other.im) / denom;
        double i = (im * other.re - re * other.im) / denom;
        return C1(r, i);
    }
    friend ostream& operator<<(ostream& oyster, const C1& other) {
        oyster << other.re << " + " << other.im << "i";
        return oyster;
    }
    C1 Csqrt(int b) {
        if (b >= 0) {
            return C1(sqrt(b * 1.), 0.);
        }
        else {
            return C1(0., sqrt(-b * 1.));
        }
    }
    C1 Cexp(C1 a) {
        return C1(exp(re) * cos(im), exp(re) * sin(im));
    }
};

template<typename CC>
CC Csqrt(CC a, int b) { return a.Csqrt(b);}

template<typename CC>
CC Cexp(CC a) { return a.Cexp(a);}

template<typename CC>
void FourierTransform(int n, CC* A, CC* B, bool inverse=0) {
    CC PI = CC();
    CC ONE = PI/PI;
    CC I = Csqrt(PI, -1);
    CC MI = Cexp(PI*I);
    
    CC num[n+1];
    num[1] = ONE;
    for (int i = 2; i <= n; ++i) {
        num[i] = num[i-1]+ONE;
    }
    
    for (int k = 0; k < n; ++k) {
        CC tmp = CC(0,0);
        for (int j = 0; j < n; ++j) {
            //if (k == 0) tmp = tmp+A[j];
            //else tmp = tmp+A[j]*Cexp(inverse?(num[j]*num[k]*I*PI*num[2]/num[n]):(MI*num[j]*num[k]*I*PI*num[2]/num[n]));
            tmp = tmp+A[j]*Cexp(PI*(inverse?(CC(0,j*k*2/n)):(CC(0,-j*k*2/n))));
        }
        B[k] = tmp/Csqrt(PI,n);
    }
}

// Example usage:
int main() {
    srand(time(NULL));
    int n = 20; // try n = 5, 10, 15, 20, 25;
    C1 A[n];
    C1 B[n];
    C1 C[n];
    for (int i = 0; i < n; i++) {
        A[i] = C1(1 + rand() % 9, 1 + rand() % 9);
        cout << "A" << i << " is " << A[i] << endl;
    }
    cout << endl;
    FourierTransform(n, A, B);
    FourierTransform(n, B, C, 1);
    for (int i = n; i > 0; i--) {
        cout << "C" << i % n << " is " << C[i % n] << endl;
        // this should output the sequence as A
    }
    return 0;
}
