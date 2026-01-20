/*
Defines a custom exception class (e.g., InvalidDataException) that inherits from std::exception.

Implements a function that processes input data and throws the custom exception if invalid data is detected.

Uses a try-catch block in main() to handle the exception gracefully, printing a meaningful error message.

Demonstrates at least one other standard exception (e.g., std::runtime_error or std::out_of_range) being thrown and caught in the same program.
*/
#include <iostream>
#include <exception>
#include <string>
#include <stdexcept>

class InvalidDataException : public std::exception {
  private:
    std::string message;
public:
    InvalidDataException(const std::string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

void processData(int data) {
    if (data < 0) {
        throw InvalidDataException("Invalid data: Data cannot be negative.");
    }
    if (data > 100) {
        throw std::out_of_range("Data out of range: Data must be between 0 and 100.");
    }
    std::cout << "Processing data: " << data << std::endl;
}

int main() {
    try {
        processData(-10); // This will throw InvalidDataException
    } catch (const InvalidDataException& e) {
        std::cerr << "Caught InvalidDataException: " << e.what() << std::endl;
    }
    try {
        processData(150); // This will throw std::out_of_range
    } catch (const std::out_of_range& e) {
        std::cerr << "Caught std::out_of_range: " << e.what() << std::endl;
    }
    try {
        processData(50); // This should process successfully
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }
    return 0;
}