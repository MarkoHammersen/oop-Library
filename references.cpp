
#include <iostream>

int main() {
    int value = 10;

    // Create a reference and a pointer to the same variable
    int& ref = value;      // Reference (must be initialized)
    int* ptr = &value;     // Pointer (can be reassigned)

    std::cout << "Initial value: " << value << std::endl;

    // Modify using reference
    ref = 20;
    std::cout << "After modifying via reference: " << value << std::endl;

    // Modify using pointer
    *ptr = 30;
    std::cout << "After modifying via pointer: " << value << std::endl;

    return 0;
}
