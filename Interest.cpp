#include <bits/stdc++.h>
using namespace std;

bool simulate_payment_1(long long LL, long long PP) {
    __int128 L = LL, P = PP;
    LL *= 1000, PP *= 1000;
    //long long L = LL*1000, P = PP*1000;
    for (int i = 0; i < 132; ++i) {
        if (i < 6) L += (L/1000)*3;
        else L += (L/1000)*2;
        if (L%100 >= 50) L += (100-L%100);
        else L -= L%100;
        L -= P;
        if (L <= 0) return true;
    }
    return false;
}

long long calculate_min_payment_1(long long L) {
    __int128 l = 0, r = 2e16, m;
    while(r-l > 1) {
        m = (r-l)/2+l;
        //cout<<simulate_payment(L,m)<<endl;
        if (simulate_payment_1(L,m)) r = m;
        else l = m;
    }
    return r;
}

bool simulate_payment_2(long long LL, long long PP) {
    __int128 L = LL, P = PP;
    LL *= 1000, PP *= 1000;
    //long long L = LL*1000, P = PP*1000;
    for (int i = 0; i < 132; ++i) {
        if (i < 6) L += (L/1000)*3;
        else L += (L/1000)*2;
        if (L%1000) L += (1000-L%1000);
        L -= P;
        if (L <= 0) return true;
    }
    return false;
}

long long calculate_min_payment_2(long long L) {
    __int128 l = 0, r = 2e16, m;
    while(r-l > 1) {
        m = (r-l)/2+l;
        //cout<<simulate_payment(L,m)<<endl;
        if (simulate_payment_2(L,m)) r = m;
        else l = m;
    }
    return r;
}

int main() {
    long long cnt = 0, bs = 1000;
    srand(time(NULL));
    for (int ndx = 0; ndx <= 13; ndx ++) {
        long long a = rand(), b = rand(), c = rand();
        long long L = bs;
        long long P1 = calculate_min_payment_1(L);
        long long P2 = calculate_min_payment_2(L);
        /*if (P1 != P2) {
            cout<<P1<<' '<<P2<<endl;
            cnt++;
        }*/
        cout<<bs<<' '<<P1<<endl;
        cout<<bs<<' '<<P2<<endl;
        bs *= 10;
    }
    //cout<<cnt<<endl;
}
