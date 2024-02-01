#include <iostream>

#include "common/relocation_t.h"

int main() {
    std::cout << "I'm the emulator!\n";
    std::cout << common::relocation_t{0,0,0};
    return 0;
}
