#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

struct Item {
    string name;
    int qty;
    double price;
};

int main() {
    vector<Item> items = {{"Widget", 3, 4.5}, {"Gadget", 12, 19.99}, {"Doohickey", 1, 0.333}};
    cout << left << setw(12) << "Name" << right << setw(5) << "Qty" << setw(10) << "Price" << endl;
    cout << setfill('-') << setw(27) << "" << setfill(' ') << endl;
    double total = 0;
    for (const auto& it : items) {
        cout << left << setw(12) << it.name << right << setw(5) << it.qty << setw(10) << fixed
             << setprecision(2) << it.price << endl;
        total += it.qty * it.price;
    }
    cout << "Total: " << total << endl;
    cout << setw(8) << setfill('0') << 42 << setfill(' ') << endl;
    cout << hex << 255 << " " << uppercase << 255 << dec << " " << 255 << endl;
    cout << oct << 8 << dec << endl;
    cout << showpos << 5 << " " << -5 << noshowpos << endl;
    cout << setprecision(1) << 0.05 << " " << setprecision(4) << 3.14159 << endl;
    return 0;
}
