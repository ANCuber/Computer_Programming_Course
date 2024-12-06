#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <queue>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

// 3-dimensional coordinate
class Coor3d {
public:
    double x, y, z;
};

// 4-dimensional coordinate
class Coor4d : public Coor3d {
public:
    double t;
};

// How to cout time
ostream& operator<<(ostream& seaotter, const std::chrono::high_resolution_clock::time_point& time_2b_printed) {
    static auto first_call_time = std::chrono::high_resolution_clock::now();
    auto since_start = time_2b_printed - first_call_time;
    long long since_start_mus = std::chrono::duration_cast<std::chrono::microseconds>(since_start).count();
    seaotter << since_start_mus / 1000. << " ms";
    return seaotter;
}

//* How to cout 3d coordinate
ostream& operator<<(ostream& seaotter, const Coor3d& xyz) {
    seaotter << fixed << setprecision(1);
    seaotter << "(" << xyz.x << ", " << xyz.y << ", " << xyz.z << ")" << endl;
    return seaotter;
}
//* How to cout 4d coordinate
ostream& operator<<(ostream& seaotter, const Coor4d& xyzt) {
    seaotter << fixed << setprecision(1);
    seaotter << "(" << xyzt.x << ", " << xyzt.y << ", " << xyzt.z << ", " << xyzt.t << ")" << endl;
    return seaotter;
}

//* The distance between P and Q
double EuclideanDistance(Coor3d P, Coor3d Q) {
    return sqrt((P.x-Q.x)*(P.x-Q.x)+(P.y-Q.y)*(P.y-Q.y)+(P.z-Q.z)*(P.z-Q.z));
}

// Random number infrastructure
std::random_device HardwareNoise;
std::mt19937 CoinsAndDices(HardwareNoise());
std::uniform_real_distribution<> Uniform11(-1.0, 1.0);

//* How to sample points at radius r km
Coor3d UniformSphere(double r) {
    Coor3d random_point;
    double theta = rand(), phi = rand();
    random_point.x = r*sin(theta)*cos(phi);
    random_point.y = r*sin(theta)*sin(phi);
    random_point.z = r*cos(theta);
    return random_point;
}

// Multi-threading infrastructure
std::mutex CoutMutex;
std::mutex SatelliteMutex;
std::mutex CellPhoneMutex;
std::queue<Coor4d> SatelliteBroadcast;
std::queue<Coor4d> CellphoneReception;
std::queue<Coor3d> CellphoneEstimate;

void SatelliteFunction() {
    while (true) {
        usleep(1000000);
        auto now = std::chrono::system_clock::now();
        time_t current_time = std::chrono::system_clock::to_time_t(now);
        //* Every second, a random satellite pops up at a random location in the sky
        Coor3d tmp = UniformSphere(20180);
        Coor4d S4 = {tmp.x, tmp.y, tmp.z, current_time};

        // The satellite broadcasts its location and timestamp
        SatelliteMutex.lock();
        SatelliteBroadcast.push(S4);
        SatelliteMutex.unlock();

        CoutMutex.lock();
        cout << "A satellite broadcasted at " << endl
             << S4 << endl;
        CoutMutex.unlock();
    }
}

void SignalFunction(Coor3d C3, Coor4d S4) {
    // Calculate the distance between satellite and cellphone
    Coor3d tmp = {S4.x, S4.y, S4.z};
    double distance_km = EuclideanDistance(C3, tmp);
    double travel_time_mus;
    CoutMutex.lock();
    cout << string(50, ' ') << "This signal is put to sleep for " << endl
         << string(50, ' ') << travel_time_mus / 1000. << " ms to travel " << distance_km << " km." << endl;
    CoutMutex.unlock();

    // Now sleep to simulate the travel time
    auto expected_arrival_time = S4.t + std::chrono::microseconds(travel_time_mus);
    std::this_thread::sleep_until(expected_arrival_time);

    CellPhoneMutex.lock();
    CellphoneReception.push(S4);
    CellPhoneMutex.unlock();

    CoutMutex.lock();
    cout << string(80, ' ') << "A signal has arrived" << endl;
    CoutMutex.unlock();
}

void CellphoneFunction() {
    Coor3d C3 = {0, 0, 0}; // let's guess the cellphone's position
    int nSignals = 0;
    Coor4d S5;
    double r5 = 0;
    while (true) {
        // Check if there is any reception every now and then
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        CellPhoneMutex.lock();
        if (!CellphoneReception.empty()) {
            Coor4d S4 = CellphoneReception.front();
            CellphoneReception.pop();
            CellPhoneMutex.unlock();
            // Record when the signal was received
            double r4 = std::chrono::high_resolution_clock::now();
            nSignals++;
            CoutMutex.lock();
            cout << string(100, ' ') << "The cellphone received the " << nSignals << "th signal" << endl
                 << string(100, ' ') << S4 << endl
                 << string(100, ' ') << "at time " << r4 << endl;
            CoutMutex.unlock();
            // Calculate time difference
            
            // Calculate distance discrepancy

            // Compute the correction vector

            CoutMutex.lock();
            cout << string(150, ' ') << "Updated Cellphone Position:" << endl
                 << string(150, ' ') << C3 << " +- " << EuclideanDistance(C3, Cold) << "km" << endl;
            CoutMutex.unlock();

            S5 = S4;
            r5 = r4;
        }
        else {
            CellPhoneMutex.unlock();
        }
    }
}

int main() {
    srand(time(NULL));
    Coor3d CellphonePosition = UniformSphere(6371);
    CoutMutex.lock();
    cout << "Cellphone is fixed at " << endl
         << CellphonePosition << endl;
    CoutMutex.unlock();

    std::thread SatelliteThread(SatelliteFunction);
    std::thread CellphoneThread(CellphoneFunction);

    while (true) {
        // Every microsecond, check if there is any broadcast
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        SatelliteMutex.lock();
        if (!SatelliteBroadcast.empty()) {
            Coor4d S4 = SatelliteBroadcast.front();
            SatelliteBroadcast.pop();
            SatelliteMutex.unlock();

            // One thread per signal
            std::thread SignalThread(SignalFunction, CellphonePosition, S4);
            SignalThread.detach();
        }
        else {
            SatelliteMutex.unlock();
        }
    }

    SatelliteThread.join();
    CellphoneThread.join();
    cout << "All threads join the main thread.  Program ends." << endl;
}
