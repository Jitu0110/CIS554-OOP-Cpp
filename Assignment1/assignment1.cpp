/*
Implement member functions and  Merge.  Your implementation has to be in-place.
You are required to implement the Merge algorithm to be described in class.
You are not allowed to use any external structures (such as array)
You are not allowed to use any helper function
You are not allowed to create new nodes or any extra structures (such as array, list, etc.).
*/
#include <iostream>
using namespace std;

class Dnode {
public:
    int value;
    Dnode* next;
    Dnode* previous;
    Dnode(int i) { value = i; next = previous = nullptr; }
    Dnode() { next = previous = nullptr; }
};

class DLL {//Doubly Linked List
public:
    Dnode* head;
    Dnode* tail;
    DLL(int n, int m);//Constructor; Construct an n-node DLL with random
    //values in 0 ... m-1
    DLL() { head = tail = nullptr; }
    void PrintF();//Print forward from head
    void PrintB();//Print backward from tail
    void Sort(Dnode * p1, Dnode *p2);//Sort the portion of a Link List from
    //the Dnode pointed by p1 until the Dnode pointed by p2
    void Merge(Dnode* p1, Dnode* p2, Dnode* p3, Dnode* p4);
    //Nerge the two portions of Linked List, as described in class
    //Note that each of the two portion is already sorted before the merge.

};

void DLL::Merge(Dnode *p1, Dnode *p2, Dnode *p3, Dnode *p4) {
    if(!p1 || !p2 || !p3 || !p4) return;

    //Store P1 previous, P2 next, P3 Previous and P4 next
    Dnode* p1Prev =  p1->previous;
    Dnode* p2Next =  p2->next;
    Dnode* p3Prev =  p3->previous;
    Dnode* p4Next =  p4->next;

    //Pointer to traverse L1
    Dnode* firstLLTracer{p1};

    //Pointer to traverse L2
    Dnode* secondLLTracer{p3};

    //Pointer to mark the head of merged LL
    Dnode* headMerged;

    if(firstLLTracer->value <= secondLLTracer->value){
        headMerged = firstLLTracer;

        //If p1 prev exists,connect it to headMerged
        if(p1Prev!= nullptr) {
            headMerged->previous = p1Prev;
            p1Prev->next = headMerged;
        }
        else{
            headMerged->previous = nullptr;

            //Point 'head' to headMerged since the merged LL starts from here
            this->head = headMerged;
        }

        firstLLTracer = firstLLTracer->next;
    }
    else{
        headMerged = secondLLTracer;

        //If p1 prev exists, connect it to headMerged
        if(p1Prev!= nullptr) {
            headMerged->previous = p1Prev;
            p1Prev->next = headMerged;
        }
        else{
            headMerged->previous = nullptr;

            //Point 'head' to headMerged since the merged LL starts from here
            this->head = headMerged;
        }

        secondLLTracer = secondLLTracer->next;
    }

    Dnode* thirdLLTracer = headMerged;


    while(firstLLTracer && secondLLTracer && firstLLTracer!=p2Next && secondLLTracer!=p4Next){
        if(firstLLTracer->value <= secondLLTracer->value){
            thirdLLTracer->next = firstLLTracer;
            firstLLTracer->previous = thirdLLTracer;

            //Move the pointer to next node of L1
            firstLLTracer = firstLLTracer->next;
        }
        else{
            thirdLLTracer->next = secondLLTracer;
            secondLLTracer->previous = thirdLLTracer;

            //Move the pointer to next node of L2
            secondLLTracer = secondLLTracer->next;
        }

        thirdLLTracer = thirdLLTracer->next;
    }

    //L1 has ended, just point the remaining of L2 to current sorted
    if(!firstLLTracer || firstLLTracer == p2->next){
        thirdLLTracer->next = secondLLTracer;
        secondLLTracer->previous = thirdLLTracer;

        //Move thirdLLTracer to last node of the merged linked list
        thirdLLTracer = p4;
    }

    //L2 has ended, just point the remaining of L1 to current sorted
    if(!secondLLTracer || secondLLTracer == p4->next){
        thirdLLTracer->next = firstLLTracer;
        firstLLTracer->previous = thirdLLTracer;


        //Move thirdLLTracer to last node of the merged linked list
        thirdLLTracer = p2;
    }


    // Check if there are other nodes between L1 and L2
    if(p2Next!=p3) {

        //Point last node of the merged LL to p2 next
        thirdLLTracer->next= p2Next;
        p2Next->previous = thirdLLTracer;

        //Move thirdLLTracer to p3 prev
        thirdLLTracer = p3Prev;

        //if p4next doesn't exist, make the p3prev null
        if(!p4Next){
            thirdLLTracer->next = nullptr;
        }
    }

    //If there exists nodes after P4, connect thirdLLTracer to P4next
    if(p4Next) {
        thirdLLTracer->next = p4Next;
        p4Next->previous = thirdLLTracer;
    }
    else{
        thirdLLTracer->next = nullptr;
    }


    //Move tail to last node on the right
    while(this->tail->next!= nullptr){
        this->tail = this->tail->next;
    }


}
void DLL::Sort(Dnode * p1, Dnode *p2){

    if (!p1 || !p1->next ||!p2 ) return;

    Dnode* p3{ p1 };
    while (p3->next && p3!=p2) {
        Dnode* p4 = p3->next;
        Dnode* pmin = p3;
        int min{ p3->value };
        while (p4) {//while p2 is not nullptr
            if (p4->value < min) {
                min = p4->value;
                pmin = p4;
            }
            p4 = p4->next;
        }
        if (pmin != p3) {//swap pmin->value with p3->value
            pmin->value = p3->value;
            p3->value = min;
        }
        p3 = p3->next;
    }
}

DLL::DLL(int n, int m) {
    head = tail = nullptr;
    for (int i = 0; i < n; ++i) {
        Dnode* p1{ new Dnode{rand() % m} };
        if (!head) {//empty
            head = tail = p1;
        }
        else {
            tail->next = p1;
            p1->previous = tail;
            tail = p1;
        }
    }
}

void DLL::PrintF() {
    cout << endl;
    Dnode* p1{ head };
    while (p1) {
        cout << p1->value << " ";
        p1 = p1->next;
    }
}

void DLL::PrintB() {
    cout << endl;
    Dnode* p1{ tail };
    while (p1) {
        cout << p1->value << " ";
        p1 = p1->previous;
    }
}



int main() { //During grading, TA might use different examples to test your code.
    DLL  L1{ 30, 20 };
    L1.PrintF();
    L1.PrintB();

    L1.Sort(L1.head, L1.tail );
    L1.PrintF();
    L1.PrintB();
    cout << endl;
    DLL L2{ 29, 15 };
    L2.Sort(L2.head, L2.tail);
    L1.tail->next = L2.head;
    L2.head->previous = L1.tail;
    L1.tail = L2.tail;

    L1.Merge(L1.head, L2.head->previous->previous, L2.head, L1.tail);

    L1.PrintF();
    L1.PrintB();


    return 0;
}
