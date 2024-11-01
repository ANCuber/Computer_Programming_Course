#include <iostream>
#include <cmath>
#include <cassert>
using namespace std;

const double PI = 3.14159265359;

class CCC {
    public:
    double r, i;
    double abs() {
        return sqrt(r*r+i*i);
    }
    CCC conjugate() {
        CCC ret = {r,-i};
        return ret;
    }
};

CCC exp(CCC z) {
    CCC ret = {exp(z.r)*cos(z.i), exp(z.r)*cos(z.i)};
    return ret;
}

CCC log(CCC z) {
    assert(z.i != 0 || z.r != 0);
    CCC ret = {log(z.abs()), 0};
    if (z.r > 0) ret.i = atan2(z.i,z.r);
    else if (z.r == 0) ret.i = (z.i > 0)?(PI/2):(-PI/2);
    else ret.i = (z.i > 0)?(PI-atan2(z.i,-z.r)):(-PI+atan2(-z.i,-z.r));
    return ret;
}

CCC polar(double r, double theta) {
    CCC ret = {r*cos(theta), r*sin(theta)};
    return ret;
}

CCC operator""i(unsigned long long digits) { 
    CCC ret = {0, digits};
    return ret;
}

CCC operator""i(long double digits) { 
    CCC ret = {0, digits};
    return ret;
}

std::ostream& operator<<(std::ostream& seaotter, CCC z) {
    if (z.i >= 0) seaotter << z.r << " + " << z.i << "i";
    else seaotter << z.r << " - " << -z.i <<"i";
    return seaotter;
}

CCC operator+(CCC c) {  
    return c;
} // unary +

CCC operator+(CCC lhs, double rhs) {
    CCC ret = {lhs.r+rhs, lhs.i};
    return ret;
}

CCC operator+(double lhs, CCC rhs) {
    CCC ret = {lhs+rhs.r, rhs.i};
    return ret;
}

CCC operator+(CCC lhs, CCC rhs) {
    CCC ret = {lhs.r+rhs.r, lhs.i+rhs.i};
    return ret;
}

CCC operator-(CCC c) {  
    CCC ret = {-c.r, -c.i};
    return ret;
} // unary -

CCC operator-(CCC lhs, double rhs) {
    CCC ret = {lhs.r-rhs, lhs.i};
    return ret;
}

CCC operator-(double lhs, CCC rhs) {
    CCC ret = {lhs-rhs.r, -rhs.i};
    return ret;
}

CCC operator-(CCC lhs, CCC rhs) {
    CCC ret = {lhs.r-rhs.r, lhs.i-rhs.i};
    return ret;
}

CCC operator*(CCC lhs, double rhs) {
    CCC ret = {lhs.r*rhs, lhs.i*rhs};
    return ret;
}

CCC operator*(double lhs, CCC rhs) {
    CCC ret = {lhs*rhs.r, lhs*rhs.i};
    return ret;
}

CCC operator*(CCC lhs, CCC rhs) {
    CCC ret = {lhs.r*rhs.r-lhs.i*rhs.i, lhs.r*rhs.i+lhs.i*rhs.r};
    return ret;
}

CCC operator/(CCC lhs, double rhs) {
    CCC ret = {lhs.r/rhs, lhs.i/rhs};
    return ret;
}

CCC operator/(double lhs, CCC rhs) {
    CCC ret = {lhs*rhs.r/(rhs.r*rhs.r+rhs.i*rhs.i), -lhs*rhs.i/(rhs.r*rhs.r+rhs.i*rhs.i)};
    return ret;
}

CCC operator/(CCC lhs, CCC rhs) {
    CCC tmp = lhs*rhs.conjugate();
    CCC ret = {tmp.r/(rhs.r*rhs.r+rhs.i*rhs.i), tmp.i/(rhs.r*rhs.r+rhs.i*rhs.i)};
    return ret;
}

int main() {
    for (double y = -2; y <= 2; y += 0.05) {
        for (double x = -2; x <= 2; x += 0.05) {
            CCC c = x + y * 1i;
            CCC z = 0 + 0i;
            for (int i = 0; i < 1000; ++i) z = z*z+c;
            (z.abs() > 0)?(cout << '#'):(cout << ' ');
        }
        cout << endl;
    }
}
