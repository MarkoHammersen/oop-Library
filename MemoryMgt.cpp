#include <iostream>
int main () {
    // Your code here
    int *ptr = new int[5];
    for(int i = 0; i < 5; ++i) {
        ptr[i] = i * 10;
    }

    for(int i = 0; i < 5; ++i) {
        std::cout << ptr[i] << " ";
    }
    std::cout << std::endl;

    delete[] ptr;

    return 0;
}