#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>  
#include <map>  
#include <time.h>
#include <math.h>
#include <fstream>

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
map<int,int> totalSlotsLowerBound;
map<int,int> totalSlotsEomLee;

int totalCollisions;
map<int,int> totalCollisionsLowerBound;
map<int,int> totalCollisionsEomLee;

int totalIdles;
map<int,int> totalIdlesLowerBound;
map<int,int> totalIdlesEomLee;

double totalTime;
map<double,double> totalTimeLowerBound;
map<double,double> totalTimeEomLee;


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
    cout << "Initial Tag Ammount" << endl;
    cin >> initialTag;
    
    cout << "Final Tag Ammount" << endl;
    cin >> finalTag;
    
    cout << "Delta Tag Ammount" << endl;
    cin >> deltaTag;

    cout << "Initial Frame Size" << endl;
    cin >> initialFrameSize;
    
    int maxRepetition;
    cout << "Number of Repetitions per Iteration" << endl;
    cin >> maxRepetition;

    srand (time(NULL));
    
    for(int chosen_algorithm = 0; chosen_algorithm < 2; chosen_algorithm ++){
    
    
        for(int index_tag = initialTag; index_tag <= finalTag; index_tag += deltaTag){
        
            int avarageSlots = 0;
            int avarageCollision = 0;
            int avarageIdle = 0;
            double avarageTime = 0;
            for(int repetition = 0; repetition < maxRepetition; repetition++){
            
                tag_ammount = index_tag;
                frameSize = initialFrameSize;
                
                if(chosen_algorithm == 0){
                    runLowerBound();
                }
                else{
                    runEomLee();
                }
                
                avarageSlots += totalSlots;
                avarageCollision += totalCollisions;
                avarageIdle += totalIdles;
                avarageTime += totalTime;            
            }
            if(chosen_algorithm == 0){
            
                totalSlotsLowerBound        [index_tag] = avarageSlots      /maxRepetition;
                totalCollisionsLowerBound   [index_tag] = avarageCollision   /maxRepetition;
                totalIdlesLowerBound        [index_tag] = avarageIdle       /maxRepetition;
                totalTimeLowerBound         [index_tag] = avarageTime       /maxRepetition;
         
            }
            else{
                
                totalSlotsEomLee            [index_tag] = avarageSlots      /maxRepetition;
                totalCollisionsEomLee       [index_tag] = avarageCollision   /maxRepetition;
                totalIdlesEomLee            [index_tag] = avarageIdle       /maxRepetition;
                totalTimeEomLee             [index_tag] = avarageTime       /maxRepetition;
            }
        }
    }

    ofstream totalSlotsFile;
    totalSlotsFile.open ("../plots/data/slotsTotal.txt");
    
    ofstream totalCollisionsFile;
    totalCollisionsFile.open ("../plots/data/slotsColisao.txt");
    
    ofstream totalIdleFile;
    totalIdleFile.open ("../plots/data/slotsVazios.txt");
    
    ofstream totalTimeFile;
    totalTimeFile.open ("../plots/data/tempo.txt");
    for(int index_tag = initialTag; index_tag <= finalTag; index_tag += deltaTag){
    
        totalSlotsFile      << index_tag << " " << totalSlotsLowerBound[index_tag]         << " " << totalSlotsEomLee[index_tag]      << endl;
        
        totalCollisionsFile << index_tag << " " << totalCollisionsLowerBound[index_tag]    << " " << totalCollisionsEomLee[index_tag] << endl;
        
        totalIdleFile       << index_tag << " " << totalIdlesLowerBound[index_tag]         << " " << totalIdlesEomLee[index_tag]      << endl;
        
        totalTimeFile       << index_tag << " " << totalTimeLowerBound[index_tag]          << " " << totalTimeEomLee[index_tag]       << endl;
    
    }
    
    totalSlotsFile.close();
    
    totalCollisionsFile.close();
    
    totalIdleFile.close();
    
    totalTimeFile.close();

    
    system("cd ../plots\ngnuplot < 'script/plotter.txt'");

    return 0;
}


