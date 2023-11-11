// Due: 11:59pm, Wednesday, Nov. 8
/*

For the class triangle, implement constructor, copy constructor,
move constructor, copy assignment,
move assignment, destructor, FirtThreeRow.
Also, implement operator<< to allow cout of triangle object.
See the sample screenshot for output requirement

Only smart pointers are allowed.  

No raw pointers allowed.
No extra helper functions allowed.
No external structures allowed.

*/

#include <iostream>
#include <memory>

using namespace std;

class node {
public:
    int value;
    shared_ptr<node> Lchild;
    shared_ptr<node> Rchild;
    weak_ptr<node> next;

    node(int i) : value(i) {}

    node() {}

};

class triangle {
public:
    shared_ptr<node> top;

    triangle(int n, int m);//constructor
    triangle() { cout << "Default Constructor\n\n"; }

    triangle(const triangle &t);//copy constructor
    void operator=(const triangle &t); //copy assignment
    ~triangle();//destructor
    triangle(triangle &&t);//move constructor
    void operator=(triangle &&t);//move assignment
    triangle FirstThreeRow();//return a triangle which is the first three rows of *this.
    //Assume the triangle *this have three or more than three rows.
};

ostream &operator<<(ostream &os, const triangle &t) {
    auto current = t.top;

    //Single node condition
    if (current->Lchild == nullptr) {
        os << current->value;
    } else {

        os << current->value << " ";
        os << endl;

        //To keep track of node index
        int i = 1;

        //To keep track of nodes in level
        int j = 0;

        //To keep track of levels
        int level = 2;

        current = current->next.lock();
        while (current != t.top) {
            if (j == level) {
                j = 0;
                level++;
                cout << endl;
            }

            os << current->value << " ";
            current = current->next.lock();

            i++;
            j++;
        }
    }
    return os;

}


int main() {

    triangle T1(5, 10);
    cout << T1 << "\n\n";

    triangle T2{T1};
    cout << T2 << "\n\n";
    triangle T3;
    T3 = T2;
    cout << T3 << "\n\n";

    T3 = T2.FirstThreeRow();
    cout << T3 << "\n\n";


    return 0;
}


// Copy Constructor
triangle::triangle(const triangle &t) {
    if (t.top) {
        auto curr = t.top;
        int nodes = 1;

        while (curr->next.lock() != t.top) {
            nodes++;
            curr = curr->next.lock();
        }

        //Solving quadratic equation to get num of levels
        int numLevels = (-1 + (sqrt(1 + 8 * nodes))) / 2;

        //Constructor call
        triangle t1(numLevels, 10);
        top = t1.top;

        curr = t.top;
        auto p = top;

        //Copy the values, deep copy
        while (curr->next.lock() != t.top) {
            p->value = curr->value;
            curr = curr->next.lock();
            p = p->next.lock();
        }

        //Set last node value
        p->value = curr->value;
    }

    cout << "Copy Constructor\n" << endl;
}

// Copy Assignment
void triangle::operator=(const triangle &t) {

    if (this != &t) {

        //Delete top
        top.reset();


        auto curr = t.top;
        int nodes = 1;

        while (curr->next.lock() != t.top) {
            nodes++;
            curr = curr->next.lock();
        }

        //Solving quadratic equation to get num of levels
        int numLevels = (-1 + (sqrt(1 + 8 * nodes))) / 2;

        triangle t1(numLevels, 10);
        top = t1.top;

        curr = t.top;
        auto p = top;

        //Copy the values
        while (curr->next.lock() != t.top) {
            p->value = curr->value;
            curr = curr->next.lock();
            p = p->next.lock();
        }

        //Set last node value
        p->value = curr->value;

    }
    cout << "Copy Assignment\n" << endl;
}


// Move Constructor
triangle::triangle(triangle &&t) {
    cout << "Move Constructor\n" << endl;
    top = move(t.top);
    t.top = nullptr;
}


// Move Assignment
void triangle::operator=(triangle &&t) {
    cout << "Move Assignment\n" << endl;
    if (this != &t) {
        top = move(t.top);
        t.top = nullptr;
    }
}

//Destructor
triangle::~triangle() {
    cout << "Destructor\n" << endl;
    top.reset();
}

//Constructor
triangle::triangle(int n, int m) {

    cout << "Constructor\n" << endl;

    if (n <= 0) {
        return;
    }

    if (n == 1) {
        top = make_shared<node>(rand() % m);
        return;
    }

    int numTotalNodes = n * (n + 1) / 2;
    int numNodesExcludingLastLevel = (n * (n + 1) / 2) - n;

    // ----- SETTING NEXT AND LEFT CHILD CONNECTIONS -----

    //Create top
    top = make_shared<node>(rand() % m);
    shared_ptr<node> p1 = top;
    shared_ptr<node> curr = p1;

    for (int i = 1; i < numTotalNodes; i++) {
        curr = make_shared<node>(rand() % m);
        p1->Lchild = curr;
        p1->next = curr;
        p1 = curr;
    }
    p1->next = top;

    curr = top;
    p1 = top;


    // ----- SETTING RIGHT CHILD CONNECTIONS -----

    //To keep track of node index
    int i = 0;

    //To keep track of nodes in level
    int j = 0;

    //To keep track if levels
    int level = 1;

    while (i < numNodesExcludingLastLevel) {

        if (j == level) {
            j = 0;
            level++;
        }

        for (int k = 0; k < (level + 1); k++) {
            p1 = p1->next.lock();
        }

        //Set right child
        curr->Rchild = p1;


        curr = curr->next.lock();
        p1 = curr;


        i++;
        j++;

    }


    // ----- RESETTING LEFT CHILD CONNECTIONS -----

    curr = top;
    p1 = top;

    //To keep track of node index
    i = 0;

    //To keep track of nodes in level
    j = 0;

    //To keep track if levels
    level = 1;

    while (i < numTotalNodes) {

        if (j == level) {
            j = 0;
            level++;
        }

        //For last level nodes, just set Left child as Nullptr
        if (i > numNodesExcludingLastLevel - 1) {
            curr->Lchild = nullptr;
        } else {

            for (int k = 0; k < (level); k++) {
                p1 = p1->next.lock();

            }

            //Set right child
            curr->Lchild = p1;
        }


        curr = curr->next.lock();
        p1 = curr;


        i++;
        j++;

    }

}

// FirstThreeRow
triangle triangle::FirstThreeRow() {

    triangle result(3, 10);
    if (top) {
        auto p1 = top;
        auto p2 = result.top;

        for (int i = 0; i < 6; i++) {
            p2->value = p1->value;
            p2 = p2->next.lock();
            p1 = p1->next.lock();
        }
    }
    cout << "FirstThreeRows\n" << endl;

    return result;
}

