
//  Page_replacement_algorithm
//
//  Created by Tanusree Sharma on 11/25/18.
//  Copyright © 2018 Tanusree Sharma. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>

using namespace std;

struct request {
    int page;
    bool airtime;
}
temprequest;

int parseinput();
void printRequest();
void policyFIFO();
void policyLRU();
void policyOPT();

/*Global Variables*/
int memsize;

vector<request> pagereqs;

int main(int argc, char * argv[]) {
    string temp, temp2;
    istringstream ss1, ss2;
    
    if(argc !=3){
        cout << "Error: incorrect argument count." << endl;
               return -1;
    }
    ss1.str(argv[2]);
    if(!(ss1 >> memsize)) {
        cout << "Invalid number: " << argv[2] << endl;
             return -2;
    }
    ss1.clear();
    
     while (cin >> temp) {
        ss1.str(temp);
          getline(ss1,temp2, ':');
        
        ss2.str(temp2);
        if(!(ss2 >> temprequest.page)) {
            cout << "Invalid page number: " << temp2 << endl;
                   return -3;
        }
        ss1 >> temp;
        if (temp == "a") {
            temprequest.airtime = true;
        } else if (temp == "n"){
            temprequest.airtime = false;
        }else{
            cout << "Invalid airtime: " << temp << endl;
                  return -4;
        }
         pagereqs.push_back(temprequest);
         temp = "";
         temp2 = "";
         ss1.clear();
         ss2.clear();
     }
    if(string("FIFO") == argv[1]) {
        policyFIFO();
    }else if (string("LRU") == argv[1]){
        policyLRU();
    }else if (string("OPT") == argv[1]){
        policyOPT();
    }else {
        cout << "Invalid page replacement policy: " << argv[1] << endl;
        return -5;
    }
    return 0;
}

void policyFIFO() {
    vector<int> memory;
    queue<int> fifo;
    queue<int> workqueue;
    
    int references =0;
    int faults = 0;
    int delayed = 0;
    
    cout << "FIFO" << endl;
    
    for (request p: pagereqs) {
        cout << p.page << ":" << p.airtime <<" ";
        references++;
        
        /* Recent Request to the workqueue for consederation*/
        workqueue.push(p.page);
        while(!workqueue.empty()){
            int r = workqueue.front();
            vector<int>::iterator m = find(memory.begin(), memory.end(), r);
            
            if (m !=memory.end()){
                /* page in memory already, swapping not needed */
                cout << "Page " << r << " is already in memory. ";
                workqueue.pop();
            } else {
                // page NOT in memory
                
                if (p.airtime == false) {
                    /*air bandwidth not available, adding page to workqueue*/
                    cout << "Tried to put Page " << r << " into memory, but air bandwidth is unavailable.";
                    delayed++;
                    break;
                } else {
                    if (memory.size() < memsize) {
                        /*memory not full yet, just push page
                           1:a: Put Page 1 into the memory.*/
                        cout << "Put Page " << r << " into memory. ";
                        faults++;
                        fifo.push(r);
                        memory.push_back(r);
                        workqueue.pop();
                        // go to next request
                        continue;
                    } else {
                        /* air bandwidth available, page swapping needed
                        3:a: Putting Page 3 into the memory. P2 is removed from the memory. */
                        
                        cout << "Put Page " << r << " into memory. Page " << fifo.front() << " removed from memory. ";
                        faults++;
                        
                        vector<int>::iterator i =  find(memory.begin(), memory.end(), fifo.front());
                        memory.erase(i);
                        memory.push_back(r);
                        fifo.push(r);
                        fifo.pop();
                        workqueue.pop();
                    }
                }
            }
        }
        cout << endl;
        
        
    }
    
    /*Report Generation:
       The total number of page references: 13;
       number of page faults: 4;
       number of delayed page replacements due to unavailable air bandwidth: 3. */
    cout << endl;
    cout << "Report:" << endl;
    cout << "Total number of page references: " << references << endl;
    cout << "The number of page faults: " << faults << endl;
    cout << "The number of delayed pages: " << delayed << endl;
}

void policyLRU() {
    /* east-recently-used */
    vector<int> memory;
    queue<int> workqueue;
    
    int references = 0;
    int faults = 0;
    int delayed = 0;
    
    
    cout << "LRU:" << endl;
    
    
    for (request p: pagereqs) {
        cout << p.page << ":" << p.airtime << " ";
        references++;
        
        
        /* add current request to workqueue */
        workqueue.push(p.page);
        while (!workqueue.empty()) {
            int r = workqueue.front();
            
            vector<int>::iterator m = find(memory.begin(), memory.end(), r);
            if (m != memory.end()) {
                /* page in memory already,  page swapping not needed. Only updating the queue */
                cout << "Page " << r << " is already in memory. ";
                int temp = r;
                memory.erase(m);
                memory.push_back(temp);
                workqueue.pop();
            } else {
                /* page NOT in memory*/
                
                if (p.airtime == false) {
                    /* air bandwidth not available, adding page to workqueue*/
                    cout << "Tried to put Page " << r << " into memory, but air bandwidth is unavailable.";
                    delayed++;
                    break;
                } else {
                    if (memory.size() < memsize) {
                        /* memory not full yet, just push page
                         1:a: Put Page 1 into the memory.*/
                        cout << "Put Page " << r << " into memory. ";
                        faults++;
                        memory.push_back(r);
                        workqueue.pop();
                        /*next request*/
                        continue;
                    } else {
                        /*air bandwidth available, swapping pages
                           3:a: Put Page 3 into the memory. Page 2 is removed from the memory. */
                        
                        cout << "Put Page " << r << " into memory. Page " << memory.front() << " removed from memory. ";
                        faults++;
                        
                        memory.erase(memory.begin());
                        memory.push_back(r);
                        workqueue.pop();
                    }
                }
            }
        }
        cout << endl;
        
        
        
       
    }
    
        /*Report:
    total number of page references: 13;
    number of page faults: 4;
    number of delayed page replacements due to unavailable air bandwidth: 3. */
    cout << endl;
    cout << "Report:" << endl;
    cout << "Total number of page references: " << references << endl;
    cout << "The number of page faults: " << faults << endl;
    cout << "The number of delayed pages: " << delayed << endl;
}
void policyOPT() {
    vector<int> memory;
    queue<int> workqueue;
    
    int references = 0;
    int faults = 0;
    int delayed = 0;
    
    
    cout << "OPT:" << endl;
    
    for(vector<request>::iterator p = pagereqs.begin(); p != pagereqs.end(); ++p) {
      
        cout << p->page << ":" << p->airtime << " ";
        references++;
        
        
        /*current request to workqueue*/
        workqueue.push(p->page);
        while (!workqueue.empty()) {
            int r = workqueue.front();
            
            vector<int>::iterator m = find(memory.begin(), memory.end(), r);
            if (m != memory.end()) {
                /*page in memory already, no page swap needed*/
                cout << "Page " << r << " is already in memory. ";
                workqueue.pop();
            } else {
                /*page NOT in memory*/
                
                if (p->airtime == false) {
                    /*NO air bandwidth available, adding page to workqueue*/
                    cout << "Tried to put Page " << r << " into memory, but air bandwidth is unavailable.";
                    delayed++;
                    break;
                } else {
                    if (memory.size() < memsize) {
                        /*memory not full yet, just push page
                        1:a: Put Page 1 into the memory.*/
                        cout << "Put Page " << r << " into memory. ";
                        faults++;
                        memory.push_back(r);
                        workqueue.pop();
                        // go to next request
                        continue;
                    } else {
                        /*air bandwidth available, pages swapping
                        3:a: Put Page 3 into the memory. Page 2 is removed from the memory.
                        search future for best page to replace
                           begin search at current request (p), try to find a page already in memory and save it as best*/
                        map<int, int> best;
                        
                        int dist = 0;
                        for(vector<request>::iterator p1 = p; p1 != pagereqs.end(); ++p1) {
                            if (best.count(p1->page) == 0) {
                                vector<int>::iterator it = find (memory.begin(), memory.end(), p1->page);
                                if (it != memory.end()) {
                                    best[p1->page] = dist;
                                }
                            }
                            ++dist;
                        }
                       
                        int bestpage = -1;
                        int bestdist = -1;
                        for (auto &m : memory) {
                            if (best.count(m) == 0) {
                                bestpage = m;
                                break;
                            }
                            else if (best[m] > bestdist) {
                                bestdist = best[m];
                                bestpage = m;
                            }
                        }
                        //
                        vector<int>::iterator f = find (memory.begin(), memory.end(), bestpage);
                        if (f == memory.end()) {
                            f = memory.begin();
                        }
                        cout << "Put Page " << r << " into memory. Page " << *f << " removed from memory. ";
                        faults++;
                        
                        memory.erase(f);
                        memory.push_back(r);
                        workqueue.pop();
                    }
                }
            }
        }
        cout << endl;
    }
    
    /*   Report:
       total number of page references: 13;
       number of page faults: 4;
       number of delayed page replacements due to unavailable air bandwidth: 3.*/
    cout << endl;
    cout << "Report:" << endl;
    cout << "Total number of page references: " << references << endl;
    cout << "The number of page faults: " << faults << endl;
    cout << "The number of delayed pages: " << delayed << endl;
}
            

        
        
        
    
