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

void plot(){



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
    
    /*
    int algorithm;
    cout << "Algorithm:" << endl << "1- Lower Bound" << endl << "2- Eom-Lee" << endl;
    cin >> algorithm;
    */
    srand (time(NULL));
    
    for(int chosen_algorithm = 0; chosen_algorithm < 2; chosen_algorithm ++){
    
    
        for(int index_tag = initialTag; index_tag <= finalTag; index_tag += deltaTag){
        
            int avarageSlots = 0;
            int avarageColision = 0;
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
                avarageColision += totalCollisions;
                avarageIdle += totalIdles;
                avarageTime += totalTime;            
            }
            if(chosen_algorithm == 0){
                totalSlotsLowerBound        [index_tag] = avarageSlots      /maxRepetition;
                totalCollisionsLowerBound   [index_tag] = avarageColision   /maxRepetition;
                totalIdlesLowerBound        [index_tag] = avarageIdle       /maxRepetition;
                totalTimeLowerBound         [index_tag] = avarageTime       /maxRepetition;
            
            
            }
            else{
                
                totalSlotsEomLee        [index_tag] = avarageSlots      /maxRepetition;
                totalCollisionsEomLee   [index_tag] = avarageColision   /maxRepetition;
                totalIdlesEomLee        [index_tag] = avarageIdle       /maxRepetition;
                totalTimeEomLee         [index_tag] = avarageTime       /maxRepetition;
            
            }
          /*  
            cout << "INDEX " << index_tag << endl;
            cout << "  TOTAL SLOTS " << totalSlotsMap[index_tag] << endl;
            cout << "  TOTAL COLLISIONS " << totalCollisionsMap[index_tag] << endl;
            cout << "  TOTAL IDLES " << totalIdlesMap[index_tag] << endl;
            cout << "  TOTAL TIME " << totalTimeMap[index_tag] << endl;
        */
        
        }
    }
/*
	Gnuplot gp;
	
    vector<pair<double, double> > v_totalSlotsLowerBound;
    vector<pair<double, double> > v_totalSlotsEomLee;
    
    vector<pair<double, double> > v_totalCollisionsLowerBound;
    vector<pair<double, double> > v_totalCollisionsEomLee;
    
    vector<pair<double, double> > v_totalIdlesLowerBound;
    vector<pair<double, double> > v_totalIdlesEomLee;
    
    vector<pair<double, double> > v_totalTimeLowerBound;
    vector<pair<double, double> > v_totalTimeEomLee;
    
    
    for(int index_tag = initialTag; index_tag <= finalTag; index_tag += deltaTag){
        v_totalSlotsLowerBound.push_back(make_pair(index_tag,totalSlotsLowerBound[index_tag]));
        v_totalSlotsEomLee.push_back(make_pair(index_tag,totalSlotsEomLee[index_tag]));
    
        v_totalCollisionsLowerBound.push_back(make_pair(index_tag,totalCollisionsLowerBound[index_tag]));
        v_totalCollisionsEomLee.push_back(make_pair(index_tag,totalCollisionsEomLee[index_tag]));
    
        v_totalIdlesLowerBound.push_back(make_pair(index_tag,totalIdlesLowerBound[index_tag]));
        v_totalIdlesEomLee.push_back(make_pair(index_tag,totalIdlesEomLee[index_tag]));
    
        v_totalTimeLowerBound.push_back(make_pair(index_tag,totalTimeLowerBound[index_tag]));
        v_totalTimeEomLee.push_back(make_pair(index_tag,totalTimeEomLee[index_tag]));
    
    }
    
    gp << "set term png" << endl;
    gp << "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5   # --- blue" << endl;
    gp << "set style line 3 lc rgb '#00ff00' lt 1 lw 2 pt 3 ps 1.5   # --- green" << endl;
    gp << "set style line 4 lc rgb '#000000' lt 1 lw 2 pt 9 ps 1.5   # --- black" << endl;
    gp << "set xlabel 'Número de Etiquetas" << endl;
    gp << "set title ''" << endl;
    
    gp << "set ylabel 'Número de Slots'" << endl;
    gp << "set output 'slotsTotal.png'" << endl;
    gp << "plot" << gp.file1d(v_totalSlotsLowerBound) << "title 'Lower Bound' with linespoints ls 1,"<< gp.file1d(v_totalSlotsEomLee) << "title 'Eom Lee' with linespoints ls 2" << endl;
    
    gp << "set ylabel 'Tempo para Identificação (s)'" << endl;
    gp << "set output 'tempo.png'" << endl;
    gp << "plot" << gp.file1d(v_totalTimeLowerBound) << "title 'Lower Bound' with linespoints ls 1,"<< gp.file1d(v_totalTimeEomLee) << "title 'Eom Lee' with linespoints ls 2" << endl;
    
    gp << "set ylabel 'Número de Slots Vazios'" << endl;
    gp << "set output 'slotsVazios.png'" << endl;
    gp << "plot" << gp.file1d(v_totalIdlesLowerBound) << "title 'Lower Bound' with linespoints ls 1,"<< gp.file1d(v_totalIdlesEomLee) << "title 'Eom Lee' with linespoints ls 2" << endl;
    
    gp << "set ylabel 'Número de Slots em Colisão'" << endl;
    gp << "set output 'slotsColisao.png'" << endl;
    gp << "plot" << gp.file1d(v_totalCollisionsLowerBound) << "title 'Lower Bound' with linespoints ls 1,"<< gp.file1d(v_totalCollisionsEomLee) << "title 'Eom Lee' with linespoints ls 2" << endl;


    */
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

}






/*
int Test(){
    
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
            runEomLee();
            
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
*/
