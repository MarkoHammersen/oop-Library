/**
 * Object-Oriented Library System
 * C++ Programming Fundamentals Capstone Project
 * 
 * This file provides the starter code structure for implementing
 * a library management system using object-oriented programming,
 * smart pointers, and exception handling in C++.
 */
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <chrono>
#include <iomanip>
#include <functional>
#include <sstream>
// Forward declarations
class LibraryItem;
class Book;
class Magazine;
class DVD;
class LibraryPatron;
class Student;
class Faculty;
class Transaction;
class Checkout;
class Return;
class LibraryException;
/**
 * Base exception class for library-related errors
 */
class LibraryException : public std::runtime_error {
public:
    // Constructor
    LibraryException(const std::string& message) : std::runtime_error(message) {}
};
// TODO: Create derived exception classes for specific error types
// Example:
 class ItemNotFoundException : public LibraryException {
    public:
        ItemNotFoundException(const std::string& id)  : LibraryException("Item not found: " + id) {}
 };
/**
 * Base class for all library items
 */
class LibraryItem {
private:
    std::string id_;
    std::string title_;
    bool available_;
    
protected:
    // Protected members for derived classes
    double dailyFine_;
    int maxLoanDays_;
public:
    // Constructor
    LibraryItem(std::string id, std::string title)
        : id_(std::move(id)), title_(std::move(title)), available_(true),
        dailyFine_(0.0), maxLoanDays_(0)
    {}
    
    // Virtual destructor for proper inheritance
    virtual ~LibraryItem() = default;
    
    // Getters
    std::string getId() const { return id_; }
    std::string getTitle() const { return title_; }
    bool isAvailable() const { return available_; }
    int getMaxLoanDays() const { return maxLoanDays_; }
    
    // Setters
    void setAvailable(bool available) { available_ = available; }
    
    // Pure virtual methods to be implemented by derived classes
    virtual std::string getItemType() const = 0;
    virtual double calculateFine(int daysOverdue) const = 0;
    virtual std::string getDetails() const = 0;
    
    // Common functionality
    void checkOut() {
        if (!available_) {
            throw LibraryException("Item is not available for checkout");
        }
        available_ = false;
    }
    
    void returnItem() {
        available_ = true;
    }
};
/**
 * Book class - derives from LibraryItem
 * TODO: Complete the implementation
 */
class Book : public LibraryItem {
private:
    std::string author_;
    std::string isbn_;
    std::string genre_;
    // TODO: Add any additional attributes
public:
    // Constructor
    Book(std::string id, std::string title, std::string author, std::string isbn, std::string genre)
        : LibraryItem(std::move(id), std::move(title)), 
      author_(std::move(author)), isbn_(std::move(isbn)), genre_(std::move(genre))
    {
        // TODO: Set appropriate values for dailyFine_ and maxLoanDays_
      dailyFine_ = 0.5; 
      maxLoanDays_ = 28;
    }
    
    // Getters
    std::string getAuthor() const { return author_; }
    std::string getIsbn() const { return isbn_; }
    std::string getGenre() const { return genre_; }
    
    // Implement pure virtual methods
    std::string getItemType() const override {
        // TODO: Implement this method
      return "Book";
    }
    
    double calculateFine(int daysOverdue) const override {
        if(daysOverdue > 0) {
            return daysOverdue * dailyFine_;
        }
        return 0.0;
    }
    
    std::string getDetails() const override {
      return "Book[ID: " + getId() + ", Title: " + getTitle() + ", Author: " + author_ +
        ", ISBN: " + isbn_ + ", Genre: " + genre_ + "]";
    }    
};
/**
 * TODO: Implement Magazine class - derives from LibraryItem
 */
/**
 * TODO: Implement DVD class - derives from LibraryItem
 */
/**
 * Base class for library patrons
 */
class LibraryPatron {
private:
    std::string id_;
    std::string name_;
    std::string contactInfo_;
    bool active_;
    
protected:
    int maxBorrowItems_;  // Maximum number of items a patron can borrow
public:
    // Constructor
    LibraryPatron(std::string id, std::string name, std::string contactInfo)
        : id_(std::move(id)), name_(std::move(name)), contactInfo_(std::move(contactInfo)),
      active_(true), maxBorrowItems_(0)
    {}
    
    // Virtual destructor
    virtual ~LibraryPatron() = default;
    
    // Getters
    std::string getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getContactInfo() const { return contactInfo_; }
    bool isActive() const { return active_; }
    int getMaxBorrowItems() const { return maxBorrowItems_; }
    
    // Setters
    void setActive(bool active) { active_ = active; }
    void setContactInfo(const std::string& contactInfo) { contactInfo_ = contactInfo; }
    
    // Pure virtual methods
    virtual std::string getPatronType() const = 0;
    virtual int getLoanExtensionDays() const = 0;
    
    // Common functionality
    void deactivate() { active_ = false; }
    void activate() { active_ = true; }
};
/**
 * Student class - derives from LibraryPatron
 * TODO: Complete the implementation
 */
class Student : public LibraryPatron {
private:
    std::string studentId_;
    std::string major_;
    // TODO: Add any additional attributes
public:
    // Constructor
    Student(std::string id, std::string name, std::string contactInfo,
            std::string studentId, std::string major)
        : LibraryPatron(std::move(id), std::move(name), std::move(contactInfo)),
      studentId_(std::move(studentId)), major_(std::move(major))
    {
        // TODO: Set appropriate value for maxBorrowItems_
        maxBorrowItems_ = 5;
    }
    
    // Getters
    std::string getStudentId() const { return studentId_; }
    std::string getMajor() const { return major_; }
    
    // Implement pure virtual methods
    std::string getPatronType() const override {
        // TODO: Implement this method
        return "Student";
    }
    
    int getLoanExtensionDays() const override {
        // TODO: Implement this method
      return 7;
    }
};
/**
 * TODO: Implement Faculty class - derives from LibraryPatron
 */
/**
 * Base class for transactions
 */
class Transaction {
private:
    std::string transactionId_;
    std::chrono::system_clock::time_point timestamp_;
    
public:
    // Constructor
    Transaction() : timestamp_(std::chrono::system_clock::now()) {
        // Generate a simple transaction ID based on timestamp
        auto time_t_now = std::chrono::system_clock::to_time_t(timestamp_);
        std::stringstream ss;
        ss << "TXN" << time_t_now;
        transactionId_ = ss.str();
    }
    
    // Virtual destructor
    virtual ~Transaction() = default;
    
    // Getters
    std::string getTransactionId() const { return transactionId_; }
    std::chrono::system_clock::time_point getTimestamp() const { return timestamp_; }
    
    // Format timestamp as string
    std::string getFormattedTimestamp() const {
        auto time_t_now = std::chrono::system_clock::to_time_t(timestamp_);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    // Pure virtual methods
    virtual std::string getTransactionType() const = 0;
    virtual std::string getDetails() const = 0;
};
/**
 * Checkout class - derives from Transaction
 * TODO: Complete the implementation
 */
class Checkout : public Transaction {
private:
    std::shared_ptr<LibraryItem> item_;
    std::shared_ptr<LibraryPatron> patron_;
    std::chrono::system_clock::time_point dueDate_;
    
public:
    // Constructor
    Checkout(std::shared_ptr<LibraryItem> item, std::shared_ptr<LibraryPatron> patron)
        : item_(item), patron_(patron)
    {
        // TODO: Implement checkout logic
        // - Check if item is available
        // - Check if patron is active and can borrow more items
        // - Update item status
        // - Calculate due date
        if (!item_->isAvailable()) {
            throw LibraryException("Item is not available for checkout");
        }
        if (!patron_->isActive()) {
            throw LibraryException("Patron is not active");
        }
        item_->checkOut();
        dueDate_ = std::chrono::system_clock::now() + std::chrono::hours(24 * item_->getMaxLoanDays());
    }
    
    // Getters
    std::shared_ptr<LibraryItem> getItem() const { return item_; }
    std::shared_ptr<LibraryPatron> getPatron() const { return patron_; }
    std::chrono::system_clock::time_point getDueDate() const { return dueDate_; }
    
    // Format due date as string
    std::string getFormattedDueDate() const {
        auto time_t_due = std::chrono::system_clock::to_time_t(dueDate_);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t_due), "%Y-%m-%d");
        return ss.str();
    }
    
    // Check if item is overdue
    bool isOverdue() const {
        return std::chrono::system_clock::now() > dueDate_;
    }
    
    // Calculate overdue fine
    double calculateFine() const {
      return item_->calculateFine(std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::now() - dueDate_).count() / 24);
    }
    
    // Implement pure virtual methods
    std::string getTransactionType() const override {
        return "Checkout";
    }
    
    std::string getDetails() const override {
      return "Checkout[Transaction ID: " + getTransactionId() +
        ", Item: " + item_->getTitle() +
        ", Patron: " + patron_->getName() +
        ", Due Date: " + getFormattedDueDate() + "]";
    }
};
/**
 * TODO: Implement Return class - derives from Transaction
 */
class Return : public Transaction {
  private:
    std::shared_ptr<LibraryItem> item_;
    std::shared_ptr<LibraryPatron> patron_;
    std::chrono::system_clock::time_point returnDate_;
public:
    // Constructor
    Return(std::shared_ptr<LibraryItem> item, std::shared_ptr<LibraryPatron> patron)
        : item_(item), patron_(patron), returnDate_(std::chrono::system_clock::now())
    {
    };
    // Getters
    std::shared_ptr<LibraryItem> getItem() const { return item_; }
    std::shared_ptr<LibraryPatron> getPatron() const { return patron_; }
    std::chrono::system_clock::time_point getReturnDate() const { return returnDate_; }
    // Format return date as string
    std::string getFormattedReturnDate() const {
        auto time_t_return = std::chrono::system_clock::to_time_t(returnDate_);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t_return), "%Y-%m-%d");
        return ss.str();
    }
    // Implement pure virtual methods
    std::string getTransactionType() const override {
        return "Return";
    }
    std::string getDetails() const override {
      return "Return[Transaction ID: " + getTransactionId() +
        ", Item: " + item_->getTitle() +
        ", Patron: " + patron_->getName() +
        ", Return Date: " + getFormattedReturnDate() + "]";
    }
};

 /**
 * Library class to manage the entire system
 */
class Library {
private:
    std::vector<std::unique_ptr<LibraryItem>> items_;
    std::vector<std::unique_ptr<LibraryPatron>> patrons_;
    std::vector<std::unique_ptr<Transaction>> transactions_;
    
    // Helper methods
    LibraryPatron* findPatronById(const std::string& id) {
        for (const auto& patron : patrons_) {
            if (patron->getId() == id) {
                return patron.get();
            }
        }
        return nullptr;
    }
    
    LibraryItem* findItemById(const std::string& id) {
        for(const auto& LibraryItem: items_)
        {
            if(LibraryItem->getId() == id)
            {
                return LibraryItem.get();
            }
        }
        return nullptr;
    }
    
public:
    // Constructor
    Library() = default;
    
    // Add methods
    void addItem(std::unique_ptr<LibraryItem> item) {
        items_.push_back(std::move(item));
    }
    
    void addPatron(std::unique_ptr<LibraryPatron> patron) {
      patrons_.push_back(std::move(patron));
    }
    
    // Checkout and return methods
    std::unique_ptr<Checkout> checkoutItem(const std::string& itemId, const std::string& patronId) {
        // TODO: Implement this method
        // - Find the item and patron
        // - Create a checkout transaction
        // - Update item status
        // - Store the transaction
      LibraryItem* item = findItemById(itemId);
      if (!item) {
          throw ItemNotFoundException(itemId);
      }
      LibraryPatron* patron = findPatronById(patronId);
      if (!patron) {
          throw LibraryException("Patron not found: " + patronId);
      }
      auto checkout = std::make_unique<Checkout>(std::shared_ptr<LibraryItem>(item), std::shared_ptr<LibraryPatron>(patron));
      transactions_.push_back(std::move(checkout));
      return checkout;        
    }
    
    std::unique_ptr<Return> returnItem(const std::string& itemId) {
      // TODO: Implement this method
      for(const auto& transaction : transactions_) {
          if(transaction->getTransactionType() == "Checkout") {
              Checkout* checkout = dynamic_cast<Checkout*>(transaction.get());
              if(checkout && checkout->getItem()->getId() == itemId) {
                  auto returnTxn = std::make_unique<Return>(checkout->getItem(), checkout->getPatron());
                  checkout->getItem()->returnItem();
                  transactions_.push_back(std::move(returnTxn));
                  return returnTxn;
              }
          }
      }      
    }
    
    // Search methods
    std::vector<LibraryItem*> searchItems(const std::function<bool(const LibraryItem&)>& predicate) {
      std::vector<LibraryItem*> results;
      for (const auto& item : items_) {
          if (predicate(*item)) {
              results.push_back(item.get());
          }
      }
      return results;        
    }
    
    // Report methods
    void printOverdueItems() const {
      for (const auto& transaction : transactions_) { // ai generated code  
          if(transaction->getTransactionType() == "Checkout") {
              const Checkout* checkout = dynamic_cast<const Checkout*>(transaction.get());
              if(checkout && checkout->isOverdue()) {
                  std::cout << checkout->getDetails() << ", Fine: $" << checkout->calculateFine() << std::endl;
              }
          }
      }        
    }
    
    void printPatronHistory(const std::string& patronId) const {
        for(const auto& transaction : transactions_) {
            if(transaction->getTransactionType() == "Checkout" || transaction->getTransactionType() == "Return") {
                const auto* checkout = dynamic_cast<const Checkout*>(transaction.get());
                const auto* returnTxn = dynamic_cast<const Return*>(transaction.get());
                if(checkout && checkout->getPatron()->getId() == patronId) {
                    std::cout << checkout->getDetails() << std::endl;
                }
                if(returnTxn && returnTxn->getPatron()->getId() == patronId) {
                    std::cout << returnTxn->getDetails() << std::endl;
                }
            }
        }
    }
    
    void printInventory() const {
        for(const auto& item : items_) {
            std::cout << item->getDetails() << ", Available: " << (item->isAvailable() ? "Yes" : "No") << std::endl;
        }
    }
};
/**
 * Simple test framework for unit testing
 */
class UnitTest {
private:
    int passed_ = 0;
    int failed_ = 0;
    
public:
    template<typename Func>
    void test(const std::string& testName, Func testFunc) {
        try {
            testFunc();
            std::cout << "✅ PASS: " << testName << std::endl;
            passed_++;
        }
        catch (const std::exception& e) {
            std::cout << "❌ FAIL: " << testName << " - " << e.what() << std::endl;
            failed_++;
        }
    }
    
    void printSummary() const {
        std::cout << "\n===== TEST RESULTS =====\n";
        std::cout << "Passed: " << passed_ << std::endl;
        std::cout << "Failed: " << failed_ << std::endl;
    }
};
/**
 * Function to run all unit tests
 */
void runTests() {
    UnitTest tester;
    
    // TODO: Add your unit tests here
    // Example:
    tester.test("Create Book", []() {
        Book book("B123", "The Great Gatsby", "F. Scott Fitzgerald", "978-3-16-148410-0", "Fiction");
        if (book.getTitle() != "The Great Gatsby") {
          throw std::runtime_error("Book title does not match");
      }
    });
    
    tester.printSummary();
}
/**
 * Main function with basic setup and demonstration
 */
int main() {
    std::cout << "=== Library Management System ===" << std::endl;
    
    // Run unit tests
    std::cout << "\nRunning unit tests..." << std::endl;
    runTests();
    
    // TODO: Create sample data and demonstrate system functionality
    
    return 0;
}

