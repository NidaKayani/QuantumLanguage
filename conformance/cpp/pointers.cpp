#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int d) : data(d), next(nullptr) {}
};

class LinkedList {
    Node* head = nullptr;
    int count = 0;
public:
    ~LinkedList() {
        while (head) { Node* t = head; head = head->next; delete t; }
    }
    void pushFront(int v) { Node* n = new Node(v); n->next = head; head = n; count++; }
    void pushBack(int v) {
        Node* n = new Node(v);
        count++;
        if (!head) { head = n; return; }
        Node* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
    bool remove(int v) {
        Node** p = &head;
        while (*p) {
            if ((*p)->data == v) { Node* dead = *p; *p = dead->next; delete dead; count--; return true; }
            p = &(*p)->next;
        }
        return false;
    }
    void reverse() {
        Node* prev = nullptr;
        Node* cur = head;
        while (cur) { Node* nx = cur->next; cur->next = prev; prev = cur; cur = nx; }
        head = prev;
    }
    void print() const {
        for (Node* c = head; c; c = c->next) cout << c->data << " -> ";
        cout << "null (" << count << ")" << endl;
    }
};

void swapPtr(int* a, int* b) { int t = *a; *a = *b; *b = t; }
void swapRef(int& a, int& b) { int t = a; a = b; b = t; }

int main() {
    int x = 10, y = 20;
    int* px = &x;
    *px += 5;
    cout << x << " " << *px << endl;
    swapPtr(&x, &y);
    cout << x << " " << y << endl;
    swapRef(x, y);
    cout << x << " " << y << endl;
    int arr[5] = {1, 2, 3, 4, 5};
    int* p = arr;
    int sum = 0;
    for (int i = 0; i < 5; i++) sum += *(p + i);
    cout << sum << " " << p[2] << " " << *(arr + 4) << endl;
    int& ref = arr[1];
    ref = 99;
    cout << arr[1] << endl;
    int* dyn = new int[4];
    for (int i = 0; i < 4; i++) dyn[i] = i * 3;
    cout << dyn[3] << endl;
    delete[] dyn;
    LinkedList list;
    for (int i = 1; i <= 4; i++) list.pushBack(i);
    list.pushFront(0);
    list.print();
    list.remove(2);
    list.reverse();
    list.print();
    int* np = nullptr;
    cout << (np == nullptr ? "null" : "not null") << endl;
    return 0;
}
