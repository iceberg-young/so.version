#include <iostream>
#include "include/version.hpp"

using namespace std;
using namespace so;

int main() {
    auto a = version::parse("123.456.789-hello.world.10+2015.3.1");
    auto b = version::parse("123.456.789-hello.world.3+2015.3.1");
    cout << (a < b);

    cout << std::string(a + version::change::incompatible + version::change::fixed);
    cout << (a != b) << ("0.0.0-x" == b);
    return 0;
}
