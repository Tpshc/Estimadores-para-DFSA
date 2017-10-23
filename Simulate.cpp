#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>  
#include <map>  
#include <time.h>
#include <math.h>


using namespace std;


double ERROR = 0.001; 

int initialTag;
int finalTag;
int deltaTag;

int initialFrameSize;
int frameSize;

int tag_ammount;


bitset<3000> slots;
bitset<3000> collisions;

int totalSlots;
map<int,int> totalSlotsMap;

int totalCollisions;
map<int,int> totalCollisionsMap;

int totalIdles;
map<int,int> totalIdlesMap;

double totalTime;
map<double,double> totalTimeMap;


void runLowerBound(){

    totalSlots = 0;
    totalCollisions = 0;
    totalIdles = 0;
    totalTime = 0;
    
    clock_t tStart = clock();
    
    while(frameSize){
        totalSlots+= frameSize;
        
        slots.reset();
        collisions.reset();

        for(int tag=0; tag<tag_ammount; tag++){
            int random =  rand() % frameSize;
            if(slots[random] && !collisions[random])
                collisions.set(random);
            else slots.set(random);
        }   
        
        
        totalCollisions += collisions.count();
        totalIdles += frameSize - slots.count();
        
        tag_ammount -= slots.count() - collisions.count();
        frameSize = collisions.count()*2;
        
    }
    totalTime = (double)(clock() - tStart)/CLOCKS_PER_SEC;
    
    
    
}


int estimate(int collisions, int success, int L){
    double bprox, yprox, temp, backlog, y1=2,expBprox;
    
    do{
        bprox = L/((y1*collisions)+success);
        expBprox = exp(-1/bprox);
        yprox = (1-expBprox) / (bprox*(1-(1 + (1/bprox))*expBprox));
        backlog = yprox*collisions;
        temp = y1;
        y1 = yprox;
    }
    while(y1-temp > ERROR);

    return (int) round(backlog);
}

void runEomLee(){

    totalSlots = 0;
    totalCollisions = 0;
    totalIdles = 0;
    totalTime = 0;
    
    clock_t tStart = clock();
    
    while(frameSize > 0){
        totalSlots+= frameSize;
        
        slots.reset();
        collisions.reset();

        for(int tag=0; tag<tag_ammount; tag++){
            int random =  rand() % frameSize;
            if(slots[random] && !collisions[random])
                collisions.set(random);
            else slots.set(random);
        }   
        
        
        totalCollisions += collisions.count();
        totalIdles += frameSize - slots.count();
        
        int success = slots.count() - collisions.count();
        tag_ammount -= success;
        frameSize = estimate(collisions.count(),success,frameSize);
        //cout << frameSize << endl;
        
    }
    totalTime = (double)(clock() - tStart)/CLOCKS_PER_SEC;
    
    
}








int main(){
    
    int maxRepetition = 2000;
    
    initialTag = 100;
    finalTag = 1000;
    deltaTag = 100;
    initialFrameSize = 64;

    srand (time(NULL));
    
    for(int index_tag = initialTag; index_tag <= finalTag; index_tag += deltaTag){
    
        int avarageSlots = 0;
        int avarageColision = 0;
        int avarageIdle = 0;
        double avarageTime = 0;
        for(int repetition = 0; repetition < maxRepetition; repetition++){
        
            tag_ammount = index_tag;
            frameSize = initialFrameSize;
            runLowerBound();
            
            avarageSlots += totalSlots;
            avarageColision += totalCollisions;
            avarageIdle += totalIdles;
            avarageTime += totalTime;            
        }
        totalSlotsMap       [index_tag] = avarageSlots      /maxRepetition;
        totalCollisionsMap   [index_tag] = avarageColision   /maxRepetition;
        totalIdlesMap       [index_tag] = avarageIdle       /maxRepetition;
        totalTimeMap        [index_tag] = avarageTime       /maxRepetition;
        
        cout << "INDEX " << index_tag << endl;
        cout << "  TOTAL SLOTS " << totalSlotsMap[index_tag] << endl;
        cout << "  TOTAL COLLISIONS " << totalCollisionsMap[index_tag] << endl;
        cout << "  TOTAL IDLES " << totalIdlesMap[index_tag] << endl;
        cout << "  TOTAL TIME " << totalTimeMap[index_tag] << endl;
    
    
    }



}
