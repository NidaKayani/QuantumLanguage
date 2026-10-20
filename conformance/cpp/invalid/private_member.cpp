class Box {
    int secret = 42;
public:
    int open() const { return secret; }
};
int main() {
    Box b;
    return b.secret;
}
