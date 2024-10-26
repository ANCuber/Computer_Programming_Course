#include <bits/stdc++.h>
using namespace std;

struct Qubit {
    double p, q, r, s;
};

Qubit initialize() {
    Qubit ret = {1,0,0,0};
    return ret;
}
void apply_measurement(Qubit &Q) {
    Q.r = Q.s = 0;
    double cp = Q.p, cq = Q.q;
    Q.p = (cp)/(cp+cq);
    Q.q = (cq)/(cp+cq);
}
void apply_not(Qubit &Q) {
    Q.s *= -1.0;
    swap(Q.p,Q.q);
}
void apply_hadamard(Qubit &Q) {
    double cp = Q.p, cq = Q.q, cr = Q.r, cs = Q.s;
    Q = {(2*cr+cp+cq),(cp+cq-2*cr),(cp-cq),(-2*cs)};
}
void apply_fujisan(Qubit &Q) {
    double cp = Q.p, cq = Q.q, cr = Q.r, cs = Q.s;
    Q = {(9*cp+16*cq+24*cr),(16*cp+9*cq-24*cr),(-12*cp+12*cq-7*cr),(25*cs)};
}
void process_operations(Qubit &Q, string &operations) {
    for (auto i : operations) {
        if (i == 'M') apply_measurement(Q);
        if (i == 'N') apply_not(Q);
        if (i == 'H') apply_hadamard(Q);
        if (i == 'F') apply_fujisan(Q);
    }
}

int main() {
    string TestingCases[12] = {
        "HM", "NM", "FM",
        "HHM", "HNM", "HFM",
        "NHM", "NNM", "NFM",
        "FHM", "FNM", "FFM"
    };
    for (int ndx = 0; ndx < 12; ndx ++) {
        string operations = TestingCases[ndx];
        Qubit Q = initialize();
        process_operations(Q, operations);
        cout << operations << " --> " << Q.p - Q.q << endl;
    }
}
