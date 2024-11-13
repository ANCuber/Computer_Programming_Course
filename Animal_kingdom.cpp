#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

int dx[5] = {0,0,0,1,-1};
int dy[5] = {0,1,-1,0,0};

vector<vector<set<int> > > MapR(105, vector<set<int> > (405));
priority_queue<int, vector<int> , greater<int> > pqr, pqt;

class Animal;
class Rabbit;
class Tiger;

Rabbit* ListOfR[1000000 + 100];
Tiger* ListOfT[1000000 + 100];
bitset<1000000 + 100> usingr, usingt;
int nRabbit = 0;
int nTiger = 0;

class Animal {
public:
    // member variables
    // age, health, and coordinate
    int age = 0, health;
    int x, y, id;
    // constructor and destructor
    /*
     * Define several constructors and a destructor.
     * For if coordinates are not given, set them to (rand()%50, rand()%50)
     */
    Animal(int h, int _id) {
        health = h;
        x = rand()%50;
        y = rand()%50;
        id = _id;
    }
    Animal(int h, int cordx, int cordy, int _id) {
        health = h;
        x = cordx;
        y = cordy;
        id = _id;
    }
    ~Animal() {}
    // other member functions
    /*
     * Each day, each animal moves in a random direction by 1 unit,
     * or is doesn't move; use rand() % 5.
     * Its age and health change, too
     */
    void UpdateEachDay() {
        int rd = rand()%5;
        x += dx[rd]+100, y += dy[rd]+400;
        x %= 100, y %= 400;
        age++, health--;
    }
};

class Rabbit : public Animal {
public:
    // constructors and destructor
    Rabbit(int h, int _id) : Animal(h, _id) {
        MapR[x][y].insert(id);
    }
    Rabbit(int h, int cordx, int cordy, int _id) : Animal(h, cordx, cordy, _id) {
        MapR[x][y].insert(id);
    }
    // other member functions
    bool IsReady() {
        // A rabbit is ready to reproduce every 10 days
        return !(age%10) && age;
    }
    bool Reproduce(int pos) {
        // Rabbit baby has health 100
        if (!IsReady()) return 0;
        int tmp = pqr.top();
        nRabbit++;
        ListOfR[pqr.top()] = new Rabbit(100, x, y, pqr.top());
        usingr[pqr.top()] = 1;
        pqr.pop();
        return tmp < pos;
    }
    void UpdateRabbit() {
        MapR[x][y].erase(id);
        UpdateEachDay();
        MapR[x][y].insert(id);
    }
};

class Tiger : public Animal {
public:
    // constructors and destructor
    Tiger(int h, int _id) : Animal(h, _id) {}
    Tiger(int h, int cordx, int cordy, int _id) : Animal(h, cordx, cordy, _id) {}
    // other member functions
    bool IsReady() {
        // A tiger is ready to reproduce if health > 100
        return health > 100;
    }
    bool Reproduce(int pos) {
        // Mother tiger gives half of her health to the baby
        if (!IsReady()) return 0;
        int tmp = pqt.top();
        nTiger++;
        ListOfT[pqt.top()] = new Tiger(health/2, x, y, pqt.top());
        usingt[pqt.top()] = 1;
        health -= ListOfT[pqt.top()]->health;
        pqt.pop();
        return tmp < pos;
    }
    void eat(int cx, int cy) {
        // Eating 1 rabbit regenerates 5 health
        for (auto i : MapR[cx][cy]) {
            health += 5*(ListOfR[i]->health != 0);
            ListOfR[i]->health = 0;
        }
    }
};

void InitializeAnimals() {
    // Start with 1000 rabbits and 200 tigers.
    for (int i = 1000; i < 1e6; ++i) pqr.push(i);
    for (int i = 200; i < 1e6; ++i) pqt.push(i);
    nRabbit = 1000, nTiger = 200;
    for (int i = 0; i < 200; ++i) {
        ListOfT[i] = new Tiger(100-(rand()%2)*50,i);
        usingt[i] = 1;
    }
    for (int i = 0; i < 1000; ++i) {
        ListOfR[i] = new Rabbit(100,i);
        usingr[i] = 1;
    }
}

void AnimalMove() {
    for (int i = 0, cnt = 0; cnt < nRabbit; ++i) {
        if (usingr[i]) cnt++;
        else continue;
        ListOfR[i]->UpdateRabbit();
    }
    for (int i = 0, cnt = 0; cnt < nTiger; ++i) {
        if (usingt[i]) cnt++;
        else continue;
        ListOfT[i]->UpdateEachDay();
    }
}

void RabbitReproduce() {
    for (int i = 0, cnt = 0; cnt < nRabbit; ++i) {
        if (usingr[i]) cnt++;
        else continue;
        cnt += ListOfR[i]->Reproduce(i);
    }
}

void TigerReproduce() {
    for (int i = 0, cnt = 0; cnt < nTiger; ++i) {
        if (usingt[i]) cnt++;
        else continue;
        cnt += ListOfT[i]->Reproduce(i);
    }
}

void TigerEatRabbit() {
    for (int i = 0, cnt = 0; cnt < nTiger; ++i) {
        if (usingt[i]) cnt++;
        else continue;
        int cx = ListOfT[i]->x;
        int cy = ListOfT[i]->y;
        ListOfT[i]->eat(cx, cy);
    }
}

void RabbitDie() {
    /*
    * Rabbit dies if
    * 1. it is older than 50 days
    * 2. it is eaten by a tiger
    * 3. There are five rabbits in the same cell
    */
    for (int i = 0, cnt = 0; cnt < nRabbit; ++i) {
        if (usingr[i]) cnt++;
        else continue;
        int cx = ListOfR[i]->x;
        int cy = ListOfR[i]->y;
        if ((MapR[cx][cy].size() >= 5) || (ListOfR[i]->age > 50)) ListOfR[i]->health = 0;
    }
    int tmpnR = nRabbit;
    for (int i = 0, cnt = 0; cnt < tmpnR; ++i) {
        if (usingr[i]) cnt++;
        else continue;
        if (!ListOfR[i]->health) {
            int cx = ListOfR[i]->x;
            int cy = ListOfR[i]->y;
            MapR[cx][cy].erase(i);
            usingr[i] = 0;
            pqr.push(i);
            nRabbit--;
        }
    }
}

void TigerDie() {
    // Tiger dies if health = 0
    int tmpnT = nTiger;
    for (int i = 0, cnt = 0; cnt < tmpnT; ++i) {
        if (usingt[i]) cnt++;
        if (!usingt[i] || ListOfT[i]->health > 0) continue;
        usingt[i] = 0;
        pqt.push(i);
        nTiger--;
    }
}

void VisualizeField(int day) {
    // create field
    vector<vector<char> > Field(100,vector<char> (400,' '));
    // Put a '#' for each Tiger and a '.' for each Rabbit.
    for (int i = 0, cnt = 0; cnt < nTiger; ++i) {
        if (usingt[i]) cnt++;
        Field[ListOfT[i]->x][ListOfT[i]->y] = '#';
    }
    for (int i = 0, cnt = 0; cnt < nRabbit; ++i) {
        if (usingr[i]) cnt++;
        Field[ListOfR[i]->x][ListOfR[i]->y] = '.';
    }
    int sum = 0;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 400; ++j) {
            sum += MapR[i][j].size();
        }
    }

    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 400; ++j) {
            cout<<Field[i][j];
        }
        cout<<endl;
    }
    cout << day << " days  " << nRabbit << " Rabbits  " << nTiger << " Tigers  " << endl;
    for (int i = 0; i < sqrt(nRabbit) && i < 400; i++) cout << "."; cout << endl;
    for (int j = 0; j < sqrt(nTiger) && j < 400; j++) cout << "#"; cout << endl;

    
    
    usleep(40*1000);
    // sleep for 500 milliseconds, 50 if you have a decent laptop
    // this_thread::sleep_for(chrono::milliseconds(500));
}

int main() {
    srand(time(NULL));
    usingr.reset(), usingt.reset();
    InitializeAnimals();
    
    // Main loop
    int day = 0;
    for (; day < 10000000; day++) {
        // break if no rabbit or tiger left
        if (!nRabbit || !nTiger) break;

        AnimalMove();
        RabbitReproduce();
        TigerReproduce();
        TigerEatRabbit();
        RabbitDie();
        TigerDie();
                
        if (day % 1 == 0) VisualizeField(day);
    }
    cout << "On Day " << day << ": " << nRabbit << " Rabbits and " << nTiger << " Tigers" << endl;
}
