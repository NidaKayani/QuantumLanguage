#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>
using namespace std;

class Account {
protected:
    string owner;
    double balance;
    vector<string> log;
public:
    Account(const string& o, double b) : owner(o), balance(b) {}
    virtual ~Account() {}
    virtual void deposit(double amt) {
        if (amt <= 0) throw invalid_argument("deposit must be positive");
        balance += amt;
        log.push_back("deposit " + to_string((int)amt));
    }
    virtual void withdraw(double amt) {
        if (amt > balance) throw runtime_error("insufficient funds for " + owner);
        balance -= amt;
        log.push_back("withdraw " + to_string((int)amt));
    }
    virtual string type() const { return "Basic"; }
    double getBalance() const { return balance; }
    void report() const {
        cout << type() << " " << owner << ": " << fixed << setprecision(2) << balance << " [";
        for (size_t i = 0; i < log.size(); i++) cout << (i ? ", " : "") << log[i];
        cout << "]" << endl;
    }
};

class Savings : public Account {
    double rate;
public:
    Savings(const string& o, double b, double r) : Account(o, b), rate(r) {}
    void addInterest() { balance += balance * rate; log.push_back("interest"); }
    string type() const override { return "Savings"; }
};

class Checking : public Account {
    double overdraft;
public:
    Checking(const string& o, double b, double od) : Account(o, b), overdraft(od) {}
    void withdraw(double amt) override {
        if (amt > balance + overdraft) throw runtime_error("overdraft limit exceeded");
        balance -= amt;
        log.push_back("withdraw " + to_string((int)amt));
    }
    string type() const override { return "Checking"; }
};

int main() {
    Savings s("Alice", 1000, 0.05);
    Checking c("Bob", 100, 50);
    vector<Account*> accounts = {&s, &c};
    s.deposit(500);
    s.addInterest();
    c.withdraw(120);
    try { c.withdraw(100); } catch (const exception& e) { cout << "Error: " << e.what() << endl; }
    try { s.deposit(-5); } catch (const exception& e) { cout << "Error: " << e.what() << endl; }
    try { s.withdraw(1e6); } catch (const exception& e) { cout << "Error: " << e.what() << endl; }
    double total = 0;
    for (Account* a : accounts) { a->report(); total += a->getBalance(); }
    cout << "Total: " << total << endl;
    return 0;
}
