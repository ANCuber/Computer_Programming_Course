#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
using namespace std;

class Animal {
public:
    int age, health, x, y;    // member variables
    int alive = 0;      // alive or not
    /*
     * Define several constructors and a destructor.
     * For if coordinates are not given, set them to rand() % 50
     */

    void move() {   //animal move
        int direction = rand() % 5;
        if( direction == 0){ 
            if( x == 399 ){ x = 0; }
            else{ x += 1; }
        }
        if( direction == 1){ 
            if( x == 0 ){ x = 399; }
            else{ x -= 1; }
        }
        if( direction == 2){ 
            if( y == 99 ){ y = 0; }
            else{ y += 1; }
        }
        if( direction == 3){ 
            if( y == 0 ){ y = 99; }
            else{ y -= 1; }
        }
    }
    void aging() {  //animal aging
        age += 1; 
        health -= 1;
    }
};

class Rabbit : public Animal {
public:
    bool IsReady() {
        return age > 0 && age % 10 == 0;    // A rabbit is ready to reproduce every 10 days
    }
    Rabbit Reproduce(int x, int y) {
        Rabbit babyrabbit;
        babyrabbit.age = 0;
        babyrabbit.health = 100;
        babyrabbit.alive = 1;
        babyrabbit.x = x;
        babyrabbit.y = y;
        return babyrabbit;
    }
};

class Tiger : public Animal {
public:
    bool IsReady() {
        return health > 100;    // A tiger is ready to reproduce if health > 100
    }
    Tiger Reproduce(int x, int y, int health) {
        Tiger babytiger;
        babytiger.age = 0;
        babytiger.health = health/2;
        babytiger.alive = 1;
        babytiger.x = x;
        babytiger.y = y;
        return babytiger;
    }
};

Rabbit ListOfR[1000000 + 100];
Tiger ListOfT[1000000 + 100];
int RabbitNumber[100][400] ;
int RabbitOrTiger[100][400] ;
int nRabbit = 0;
int nTiger = 0;

void InitializeAnimals() {
    // Start with 1000 rabbits and 200 tigers.
    for (int i=0; i < 1000; i++){
        ListOfR[i].age = 0;
        ListOfR[i].health = 100;
        ListOfR[i].x = rand() % 400;
        ListOfR[i].y = rand() % 100;
        ListOfR[i].alive = 1;
    }
    for (int i=0; i < 200; i++){
        ListOfT[i].age = 0;
        ListOfT[i].health = 50;
        ListOfT[i].x = rand() % 400;
        ListOfT[i].y = rand() % 100;
        ListOfT[i].alive = 1;
    }
}

void AnimalMove() {
    for (int i=0; i < 1000000 + 100; i++ ){
        if( ListOfR[i].alive == 1 ){
            ListOfR[i].move();
            ListOfR[i].aging();
        }
    }
    for (int i=0; i < 1000000 + 100; i++ ){
        if( ListOfT[i].alive == 1 ){
            ListOfT[i].move();
            ListOfT[i].aging();
        }
    }
}

void RabbitReproduce() {
    for (int i=0; i < 1000000 + 100; i++ ){
        if( ListOfR[i].alive == 1 && ListOfR[i].IsReady() ){
            for (int j=0; j <1000000 + 100; j++){
                if (ListOfR[j].alive == 0){
                    ListOfR[j] = ListOfR[i].Reproduce( ListOfR[i].x, ListOfR[i].y);
                    break;
                }
            }
        }
    }
}
void TigerReproduce() {
    for (int i=0; i < 1000000 + 100; i++ ){
        if( ListOfT[i].alive == 1 && ListOfT[i].IsReady()){
            for (int j=0; j <1000000 + 100; j++){
                if (ListOfT[j].alive == 0){
                    ListOfT[j] = ListOfT[i].Reproduce( ListOfT[i].x, ListOfT[i].y, ListOfT[i].health);
                    ListOfT[i].health = ListOfT[i].health/2;
                    break;
                }
            }
        }
    }

}

void TigerEatRabbit() {
    for ( int i=0; i < 1000000 + 100; i++){
        if (ListOfR[i].alive == 0){
                break;
            }
        for ( int j=0; j < 1000000 + 100; j++){
            if (ListOfT[j].alive == 0){
                break;
            }
            if (ListOfR[i].x == ListOfT[j].x && ListOfR[i].y == ListOfT[j].y){
                if ( ListOfT[j].health < 100) {
                    for (int k = 0; k < 1000000 + 100; k++ ){
                        if (ListOfR[k].alive == 0){
                            ListOfR[i] = ListOfR[k-1];
                            ListOfR[k-1].alive = 0;
                            break;
                        }                        
                    }
                    ListOfT[j].health += 5;
                    break;
                }
            }        
        }
    }
}

void CountRabbit() {
    for (int i=0; i < 100; i++){
        for (int j=0; j < 400; j++){
            RabbitNumber[i][j] = 0;
        }
    }
    for (int i=0; i < 1000000 + 100; i++ ){
        if( ListOfR[i].alive == 1 ){
            RabbitNumber[ListOfR[i].y][ListOfR[i].x] += 1;
        }
        else{ break; }
    }
}

void RabbitDie() {      //Rabbit dies
    for (int i=0; i < 1000000 + 100; i++ ){
        if( ListOfR[i].alive == 1 ){
            if( ListOfR[i].age > 50 ){      //it is older than 50 days
                for (int k = 0; k < 1000000 + 100; k++ ){
                    if (ListOfR[k].alive == 0){
                        ListOfR[i] = ListOfR[k-1];
                        ListOfR[k-1].alive = 0;
                        break;
                    }
                }
            }   
            if( RabbitNumber[ListOfR[i].y][ListOfR[i].x] >= 5 ){        //There are five rabbits in the same cell
                for (int k = 0; k < 1000000 + 100; k++ ){
                    if (ListOfR[k].alive == 0){
                        ListOfR[i] = ListOfR[k-1];
                        ListOfR[k-1].alive = 0;
                        break;
                    }
                }
            }    
        }
        else{ break; }
    }
}
void TigerDie() {       // Tiger dies if health = 0
    for (int i=0; i < 1000000 + 100; i++ ){
        if( ListOfT[i].alive == 1 ){
            if( ListOfT[i].health <= 0 ){ 
                for (int k = 0; k < 1000000 + 100; k++ ){
                    if (ListOfT[k].alive == 0){
                        ListOfT[i] = ListOfT[k-1];
                        ListOfT[k-1].alive = 0;
                        break;
                    }
                }
            }
        }
        else{ break; }
    }
}

void AnimalLocation() {     // Where is the rabbits and tigers at
    for (int i=0; i < 100; i++){
        for (int j=0; j < 400; j++){
            RabbitOrTiger[i][j] = 0;
        }
    }
    for (int i=0; i < 1000000 + 100; i++ ){
        if( ListOfR[i].alive == 1 ){
            RabbitOrTiger[ListOfR[i].y][ListOfR[i].x] = 1;
        }
        else{ break; }
    }
    for (int i=0; i < 1000000 + 100; i++ ){
        if( ListOfT[i].alive == 1 ){
            RabbitOrTiger[ListOfT[i].y][ListOfT[i].x] = 2;
        }
        else{ break; }
    }
}

void VisualizeField(int day) {
    nRabbit = 0;
    nTiger = 0;
    for (int i=0; i < 1000000 + 100; i++ ){         //count number of rabbits
        if( ListOfR[i].alive == 1 ){
            nRabbit += 1;
        }
        else{ break; }
    }
    for (int i=0; i < 1000000 + 100; i++ ){         //count number of tigers
        if( ListOfT[i].alive == 1 ){
            nTiger += 1;
        }
        else{ break; }
    }
    // Put a '#' for each Tiger and a '.' for each Rabbit.
    for (int i=0; i < 100; i++){          
        for (int j=0; j < 400; j++){
            //if (RabbitOrTiger[i][j] == 0 ){ cout << " "; }
            //if (RabbitOrTiger[i][j] == 1 ){ cout << "."; }
            //if (RabbitOrTiger[i][j] == 2 ){ cout << "#"; }
        }
        //cout << endl;
    }
    if (day%10 == 0){
        cout << day << " days  " << nRabbit << " Rabbits  " << nTiger << " Tigers  " << endl;
    }
    
}

int main() {
    srand(time(NULL));
    InitializeAnimals();
    for (int day = 1; day < 10000000; day++) {
        AnimalMove();
        if ( day%10 == 0 ){
            RabbitReproduce();
        }
        TigerReproduce();
        TigerEatRabbit();
        CountRabbit();
        RabbitDie();
        TigerDie();
        AnimalLocation();
        if (day % 1 == 0) VisualizeField(day);
        if (nRabbit <= 0 || nTiger <= 0){          // break if no rabbit or tiger left
            break;
        }
    }
    return 0;
}

