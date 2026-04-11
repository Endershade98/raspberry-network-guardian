#include <iostream>
using namespace std;

// tests declarations
void test_connection_initial_state();
void test_dummy_packet();

int main() {
    cout << "Running tests..." << endl;
    test_connection_initial_state();
    test_dummy_packet();
    cout << "All tests passed!" << endl;
    return 0;
}