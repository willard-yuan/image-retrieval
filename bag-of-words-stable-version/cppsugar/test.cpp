// unit test file for the func namespace

#include <iostream>
#include <string>
#include "cppsugar"

using namespace std;
using namespace func;
using namespace util;

template<typename T>
bool IsEqual(const vector<T> &a, const vector<T> &b) {
    if (a.size() != b.size()) { return false; }
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    // test basic functions
    auto vec = Range(0, 2, 20);
    if (!IsEqual(vec, vector<int>{0, 2, 4, 6, 8, 10, 12, 14, 16, 18})) { return -1; }
    cout << "[PASS] Range passed." << endl;

    vec = Map<int, int>(vec, [](int x) { return x + 1; });
    if (!IsEqual(vec, vector<int>{1, 3, 5, 7, 9, 11, 13, 15, 17, 19})) { return -1; }
    cout << "[PASS] Map passed." << endl;

    vec = Filter<int>(vec, [](int x) { return (x % 3 == 0); });
    if (!IsEqual(vec, vector<int>{3, 9, 15})) { return -1; }
    cout << "[PASS] Filter passed." << endl;

    if (Sum(vec) != 27) { return -1; }
    if (Average(vec) != 9) { return -1; }
    cout << "[PASS] Sum and average passed." << endl;

    if (Min(vec) != 3) { cout << "[FAIL] Min test failed." << endl; return -1; }
    if (Max(vec) != 15) { cout << "[FAIL] Max test failed." << endl; return -1; }
    if (ArgMin(vec) != 0) { cout << "[FAIL] Argmin test failed." << endl; return -1; }
    if (ArgMax(vec) != 2) { cout << "[FAIL] ArgMax test failed." << endl; return -1; }
    cout << "[PASS] Min/Max and ArgMin/ArgMax passed." << endl;

    // test chain

    // test string replace
    string str = "shability";
    Replace(str, "sha", "niu");
    if (str.compare("niubility")) {
        cout << "[FAIL] String replacement test failed." << endl; 
        return -1;
    }
    else {
        cout << "[PASS] String replacement test passed." << endl;
    }

    // test print
    cout << Print("var = ", 2) << endl;
    cout << Print("var = ", 2, ", var2 = ", 3.0) << endl;

    cout << "[PASS] All passed!" << endl;

    return 0;
}
