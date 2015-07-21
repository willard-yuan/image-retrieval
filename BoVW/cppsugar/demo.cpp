#include <iostream>
#include <vector>
#include "cppsugar"

using namespace std;
using namespace func;
using namespace util;

int main() {
    // some func usage
    auto vec = Range(20);
    // select the even numbers
    vec = Filter<int>(vec, [](int v) { return v % 2 == 0; });
    // compute x * (20 - x)
    vec = Map<int, int>(vec, [](int v) { return v * (20 - v); });
    cout << vec << endl;
    // unique!
    cout << endl;
    cout << Unique(vec) << endl;
    // min/max/sum
    cout << endl
        << "The minimum element is: " << Min(vec) << endl
        << "The sum is: " << Sum(vec) << endl
        << "The max of square is: " << Max<int, int>(vec, [](int x) { return x * x; }) << endl;

    // some util usage
    cout << Print("var = ", 2, ", var2 = ", 3.0) << endl << endl;
    auto lines = ReadAllLinesFromFile("demo.cpp");
    for (auto l : lines) {
        cout << l << endl;
    }
    
    return 0;
}
