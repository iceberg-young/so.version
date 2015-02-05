#include <iostream>
#include "include/version.hpp"

using namespace std;
using namespace so;

int main() {
    version a, b;
    cout << std::string(a + version::change::incompatible + version::change::fixed);
    cout << (a != b) << ("0.0.0-x" == b);
    return 0;
}
