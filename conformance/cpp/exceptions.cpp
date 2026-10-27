#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

class InsufficientFunds : public exception {
    string msg;
public:
    InsufficientFunds(double need) : msg("need " + to_string((int)need) + " more") {}
    const char* what() const noexcept override { return msg.c_str(); }
};

double divide(double a, double b) {
    if (b == 0) throw invalid_argument("division by zero");
    return a / b;
}

void withdraw(double balance, double amount) {
    if (amount > balance) throw InsufficientFunds(amount - balance);
    cout << "ok " << balance - amount << endl;
}

int level(int n) {
    if (n == 0) throw runtime_error("bottom reached");
    return level(n - 1) + 1;
}

int main() {
    try {
        cout << divide(10, 4) << endl;
        cout << divide(1, 0) << endl;
    } catch (const invalid_argument& e) {
        cout << "invalid: " << e.what() << endl;
    }
    try {
        withdraw(100, 30);
        withdraw(100, 250);
    } catch (const InsufficientFunds& e) {
        cout << "funds: " << e.what() << endl;
    }
    try {
        level(5);
    } catch (const exception& e) {
        cout << "caught " << e.what() << endl;
    }
    try {
        throw 42;
    } catch (int code) {
        cout << "int " << code << endl;
    }
    try {
        try {
            throw out_of_range("inner");
        } catch (const out_of_range& e) {
            cout << "rethrowing " << e.what() << endl;
            throw;
        }
    } catch (...) {
        cout << "outer caught" << endl;
    }
    try {
        int n = stoi("abc");
        cout << n << endl;
    } catch (const invalid_argument&) {
        cout << "stoi failed" << endl;
    }
    cout << "done" << endl;
    return 0;
}
