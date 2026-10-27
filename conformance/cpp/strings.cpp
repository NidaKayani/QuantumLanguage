#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    string s = "Hello, World";
    cout << s.length() << " " << s.size() << " " << s[0] << " " << s.at(7) << endl;
    cout << s.substr(7) << "|" << s.substr(0, 5) << endl;
    cout << s.find("World") << " " << s.find('o') << " " << s.rfind('o') << endl;
    if (s.find("xyz") == string::npos) cout << "not found" << endl;
    string t = s;
    t.replace(0, 5, "Howdy");
    cout << t << endl;
    t.insert(0, ">> ");
    t.erase(t.size() - 1);
    cout << t << endl;
    t += "!!";
    t.append("?");
    cout << t << " " << t.back() << " " << t.front() << endl;
    string r = s;
    reverse(r.begin(), r.end());
    cout << r << endl;
    string up = s;
    for (char &ch : up) ch = toupper(ch);
    cout << up << endl;
    cout << to_string(42) + "x" << " " << stoi("123") + 1 << " " << stod("2.5") * 2 << endl;
    cout << (string("abc") < string("abd")) << " " << s.compare("Hello") << endl;
    string words = "one two three";
    int spaces = count(words.begin(), words.end(), ' ');
    cout << spaces + 1 << " words" << endl;
    string empty;
    cout << empty.empty() << " " << s.empty() << endl;
    char cstr[] = "chars";
    string fromC(cstr);
    cout << fromC + "!" << " " << fromC.size() << endl;
    string rep(3, '*');
    cout << rep << endl;
    return 0;
}
