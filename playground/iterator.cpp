#include <vector>
#include <iostream>

int main() {
    std::vector<int> vec;

    for (const auto &v: vec) {
        std::cout << v << '\n';
    }
    return 0;
}