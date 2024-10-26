#include <bits/stdc++.h>
using namespace std;

int Fins[10][15] = {
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
};

int F[15][20];

int dx[4] = {0,0,1,-1};
int dy[4] = {1,-1,0,0};

double SimulateCpuTemperature(double g) {
    vector<vector<double> > tm(15, vector<double>(20,30)), dt(15, vector<double>(20,0));
    double mxd = 100;
    int cnt = 0;
    while(mxd > 1e-6) {
        mxd = 0;
        cnt++;
        for (int i = 1; i <= 15; ++i) tm[10][i] += g;
        
        for (int i = 1; i <= 10; ++i) {
            for (int j = 1; j <= 15; ++j) {
                for (int k = 0; k < 4; ++k) {
                    if (F[i][j] == 2 && F[i+dx[k]][j+dy[k]] == 2) dt[i][j] -= (tm[i][j]-tm[i+dx[k]][j+dy[k]])/50;
                    if ((F[i][j] == 1 && F[i+dx[k]][j+dy[k]] == 2) || (F[i][j] == 2 && F[i+dx[k]][j+dy[k]] == 1)) dt[i][j] -= (tm[i][j]-tm[i+dx[k]][j+dy[k]])/20;
                    if (F[i][j] == 1 && F[i+dx[k]][j+dy[k]] == 1) dt[i][j] -= (tm[i][j]-tm[i+dx[k]][j+dy[k]])/10;
                    if ((F[i][j] == 0 && F[i+dx[k]][j+dy[k]] == 1) || (F[i][j] == 1 && F[i+dx[k]][j+dy[k]] == 0)) dt[i][j] -= (tm[i][j]-tm[i+dx[k]][j+dy[k]])/100;
                }
            }
        }

        for (int i = 1; i <= 10; ++i) {
            for (int j = 1; j <= 15; ++j) {
                tm[i][j] += dt[i][j];
                if (F[i][j] == 2) mxd = max(mxd,abs(g-abs(dt[i][j])));
                else if (F[i][j] == 0) tm[i][j] = 30;
                dt[i][j] = 0;
            }
        }
    }
    double mxt = -1e9;
    for (int i = 1; i <= 10; ++i) for (int j = 1; j <= 15; ++j) mxt = max(mxt,tm[i][j]);
    return mxt;
}

int main() {
    double TestingCases[10] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 100};
    cout<<fixed<<setprecision(3);
    for (int i = 1; i <= 10; ++i) for (int j = 1; j <= 15; ++j) F[i][j] = Fins[i-1][j-1];
    for (int i = 0; i <= 11; ++i) F[i][0] = F[i][16] = 5;
    for (int i = 0; i <= 16; ++i) F[0][i] = F[11][i] = 5;
    
    for (int ndx = 0; ndx < 10; ndx ++) {
        double g = TestingCases[ndx];
        double hottest = SimulateCpuTemperature(g);
        cout << "g: " << g << " --> Hottest: " << hottest << "\n";
    }
    return 0;
}
