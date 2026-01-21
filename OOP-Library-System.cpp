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

#define UNIT_TEST

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

class ItemNotFoundException : public LibraryException {
public:
  ItemNotFoundException(const std::string& id) : LibraryException("Item not found: " + id) {}
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
  {
  }

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
    if (available_) {
      throw LibraryException("Item is already returned");
    }
    available_ = true;
  }
};
/**
 * Book class - derives from LibraryItem
 */
class Book : public LibraryItem {
private:
  std::string author_;
  std::string isbn_;
  std::string genre_;

public:
  // Constructor
  Book(std::string id, std::string title, std::string author, std::string isbn, std::string genre)
    : LibraryItem(std::move(id), std::move(title)),
    author_(std::move(author)), isbn_(std::move(isbn)), genre_(std::move(genre))
  {
    dailyFine_ = 0.5;
    maxLoanDays_ = 28;
  }

  // Getters
  std::string getAuthor() const { return author_; }
  std::string getIsbn() const { return isbn_; }
  std::string getGenre() const { return genre_; }

  // Implement pure virtual methods
  std::string getItemType() const override {
    return "Book";
  }

  double calculateFine(int daysOverdue) const override {
    if (daysOverdue > 0) {
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
 * Magazine class - derives from LibraryItem
 */
class Magazine : public LibraryItem {
private:
  std::string issueNumber_;
  std::string publisher_;
public:
  // Constructor
  Magazine(std::string id, std::string title, std::string issueNumber, std::string publisher)
    : LibraryItem(std::move(id), std::move(title)),
    issueNumber_(std::move(issueNumber)), publisher_(std::move(publisher))
  {
    dailyFine_ = 0.5;
    maxLoanDays_ = 28;
  };

  // Implement pure virtual methods
  std::string getItemType() const override {
    return "Magazine";
  }

  double calculateFine(int daysOverdue) const override {
    if (daysOverdue > 0) {
      return daysOverdue * dailyFine_;
    }
    return 0.0;
  }

  std::string getDetails() const override {
    return "Magazine[ID: " + getId() + ", Title: " + getTitle() +
      ", Issue Number: " + issueNumber_ + ", Publisher: " + publisher_ + "]";
  }
};


/**
 * DVD class - derives from LibraryItem
 */
class DVD : public LibraryItem {
private:
  std::string director_;
  int durationMinutes_;
public:
  // Constructor
  DVD(std::string id, std::string title, std::string director, int durationMinutes)
    : LibraryItem(std::move(id), std::move(title)),
    director_(std::move(director)), durationMinutes_(durationMinutes)
  {
    dailyFine_ = 1.0;
    maxLoanDays_ = 7;
  };

  // Implement pure virtual methods
  std::string getItemType() const override {
    return "DVD";
  }

  double calculateFine(int daysOverdue) const override {
    if (daysOverdue > 0) {
      return daysOverdue * dailyFine_;
    }
    return 0.0;
  }

  std::string getDetails() const override {
    return "DVD[ID: " + getId() + ", Title: " + getTitle() +
      ", Director: " + director_ + ", Duration: " + std::to_string(durationMinutes_) + " mins]";
  }
};


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
  {
  }

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
};
/**
 * Student class - derives from LibraryPatron
 */
class Student : public LibraryPatron {
private:
  std::string studentId_;
  std::string major_;

public:
  // Constructor
  Student(std::string id, std::string name, std::string contactInfo,
    std::string studentId, std::string major)
    : LibraryPatron(std::move(id), std::move(name), std::move(contactInfo)),
    studentId_(std::move(studentId)), major_(std::move(major))
  {
    maxBorrowItems_ = 5;
  }

  // Getters
  std::string getStudentId() const { return studentId_; }
  std::string getMajor() const { return major_; }

  // Implement pure virtual methods
  std::string getPatronType() const override {
    return "Student";
  }

  int getLoanExtensionDays() const override {
    return 7;
  }
};
/**
 * Faculty class - derives from LibraryPatron
 */
class Faculty : public LibraryPatron {
private:
  std::string facultyId_;
  std::string department_;
public:
  // Constructor
  Faculty(std::string id, std::string name, std::string contactInfo,
    std::string facultyId, std::string department)
    : LibraryPatron(std::move(id), std::move(name), std::move(contactInfo)),
    facultyId_(std::move(facultyId)), department_(std::move(department))
  {
    maxBorrowItems_ = 10;
  }

  // Getters
  std::string getFacultyId() const { return facultyId_; }

  std::string getPatronType() const override {
    return "Faculty";
  }

  std::string getDepartment() const { return department_; }

  int getLoanExtensionDays() const override {
    return 14;
  }
};

class PublicMember : public LibraryPatron {
private:
  std::string memberId_;
  std::string address_;
public:
  // Constructor
  PublicMember(std::string id, std::string name, std::string contactInfo,
    std::string memberId, std::string address)
    : LibraryPatron(std::move(id), std::move(name), std::move(contactInfo)),
    memberId_(std::move(memberId)), address_(std::move(address))
  {
    maxBorrowItems_ = 3;
  }

  // Getters
  std::string getMemberId() const { return memberId_; }
  std::string getPatronType() const override {
    return "PublicMember";
  }

  int getLoanExtensionDays() const override {
    return 0;
  }

  std::string getAddress() const { return address_; }
};


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
  */
class Checkout : public Transaction {
private:
  LibraryItem* item_;
  LibraryPatron* patron_;
  std::chrono::system_clock::time_point dueDate_;
public:
  // Constructor
  Checkout(LibraryItem* item, LibraryPatron* patron)
    : item_(item), patron_(patron)
  {
    if (!item_ || !item_->isAvailable())
      throw LibraryException("Item not available");
    if (!patron_ || !patron_->isActive())
      throw LibraryException("Patron inactive");

    item_->checkOut();
    dueDate_ = std::chrono::system_clock::now() + std::chrono::hours(24 * item_->getMaxLoanDays());
  }

  // Getters
  LibraryItem* getItem() const { return item_; }
  LibraryPatron* getPatron() const { return patron_; }
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

#ifdef UNIT_TEST
  void setDueDate(const std::chrono::system_clock::time_point& newDueDate) {
    dueDate_ = newDueDate;
  }
#endif

  // Implement pure virtual methods
  std::string getTransactionType() const override {
    return "Checkout";
  }

  std::string getDetails() const override {
    return "Checkout[Transaction ID: " + getTransactionId() +
      ", Item: " + item_->getTitle() +
      ", Patron: " + patron_->getName() +
      ", Due Date: " + getFormattedDueDate() +
      ", Overdue: " + (isOverdue() ? "Yes" : "No") +
      ", Timestamp: " + getFormattedTimestamp() + "]";
  }
};
/**
 * Return class - derives from Transaction
 */
class Return : public Transaction {
private:
  LibraryItem* item_;
  LibraryPatron* patron_;
  std::chrono::system_clock::time_point returnDate_;

public:
  // Constructor
  Return(LibraryItem* item, LibraryPatron* patron)
    : item_(item), patron_(patron), returnDate_(std::chrono::system_clock::now())
  {
    if (!item_ || !patron_) {
      throw LibraryException("Invalid item or patron for return transaction");
    }
  }

  // Getters
  LibraryItem* getItem() const { return item_; }
  LibraryPatron* getPatron() const { return patron_; }
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

  // Helper: find patron by ID
  LibraryPatron* findPatronById(const std::string& id) {
    for (const auto& p : patrons_)
      if (p->getId() == id) return p.get();
    return nullptr;
  }

  // Helper: find item by ID
  LibraryItem* findItemById(const std::string& id) {
    for (const auto& i : items_)
      if (i->getId() == id) return i.get();
    return nullptr;
  }

public:
  Library() = default;

  // Add item/patron
  void addItem(std::unique_ptr<LibraryItem> item) {
    items_.push_back(std::move(item));
  }

  void addPatron(std::unique_ptr<LibraryPatron> patron) {
    patrons_.push_back(std::move(patron));
  }

  // Checkout an item
  Checkout& checkoutItem(const std::string& itemId, const std::string& patronId) {
    LibraryItem* item = findItemById(itemId);
    if (!item) throw ItemNotFoundException(itemId);

    LibraryPatron* patron = findPatronById(patronId);
    if (!patron) throw LibraryException("Patron not found: " + patronId);

    auto checkout = std::make_unique<Checkout>(item, patron);
    transactions_.push_back(std::move(checkout));
    return static_cast<Checkout&>(*transactions_.back());
  }

  // Return an item
  Return& returnItem(const std::string& itemId) {
    for (const auto& t : transactions_) {
      if (auto* checkout = dynamic_cast<Checkout*>(t.get())) {
        if (checkout->getItem()->getId() == itemId) {
          auto returnTxn = std::make_unique<Return>(checkout->getItem(), checkout->getPatron());
          checkout->getItem()->returnItem();
          transactions_.push_back(std::move(returnTxn));
          return static_cast<Return&>(*transactions_.back());
        }
      }
    }
    throw LibraryException("No active checkout found for item: " + itemId);
  }

  // Search items by predicate
  std::vector<LibraryItem*> searchItems(const std::function<bool(const LibraryItem&)>& predicate) {
    std::vector<LibraryItem*> results;
    for (const auto& item : items_) {
      if (predicate(*item)) results.push_back(item.get());
    }
    return results;
  }

  // Print all inventory
  void printInventory() const {
    for (const auto& item : items_) {
      std::cout << item->getDetails() << ", Available: "
        << (item->isAvailable() ? "Yes" : "No") << std::endl;
    }
  }

  // Print overdue items
  void printOverdueItems() const {
    uint32_t count = 0;
    for (const auto& t : transactions_) {
      if (auto* checkout = dynamic_cast<const Checkout*>(t.get())) {
        if (checkout->isOverdue()) {
          count++;
          std::cout << checkout->getDetails()
            << ", Fine: $" << checkout->calculateFine() << std::endl;
        }
      }
    }
    if (count == 0)
      std::cout << "No overdue items." << std::endl;
  }

  // Print patron history
  void printPatronHistory(const std::string& patronId) const {
    for (const auto& t : transactions_) {
      if (auto* checkout = dynamic_cast<const Checkout*>(t.get())) {
        if (checkout->getPatron()->getId() == patronId)
          std::cout << checkout->getDetails() << std::endl;
      }
      if (auto* returnTxn = dynamic_cast<const Return*>(t.get())) {
        if (returnTxn->getPatron()->getId() == patronId)
          std::cout << returnTxn->getDetails() << std::endl;
      }
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
    std::cout << "\n========================================================================\n";
    try {
      testFunc();
      std::cout << "[PASS]: " << testName << std::endl;
      passed_++;
    }
    catch (const std::exception& e) {
      std::cout << "[!!! FAIL !!!]: " << testName << " - " << e.what() << std::endl;
      failed_++;
    }

    std::cout << "===== TEST RESULTS =====\n";
    std::cout << "Passed: " << passed_ << std::endl;
    std::cout << "Failed: " << failed_ << std::endl;
    //std::cout << "n========================================================================\n";
  }
};

static void runTestsBook()
{
  UnitTest tester;

  tester.test("Create Book", []() {
    Book book("B123", "The Great Gatsby", "F. Scott Fitzgerald", "978-3-16-148410-0", "Fiction");
    if (book.getId() != "B123") {
      throw std::runtime_error("Book ID does not match");
    }
    if (book.isAvailable() != true) {
      throw std::runtime_error("Book availability does not match");
    }
    if (book.getMaxLoanDays() != 28) {
      throw std::runtime_error("Book max loan days does not match");
    }
    if (book.getTitle() != "The Great Gatsby") {
      throw std::runtime_error("Book title does not match");
    }
    if (book.calculateFine(5) != 2.5) {
      throw std::runtime_error("Book fine calculation is incorrect");
    }
    if (book.calculateFine(0) != 0.0) {
      throw std::runtime_error("Book fine calculation is incorrect");
    }
    if (book.getItemType() != "Book") {
      throw std::runtime_error("Book type does not match");
    }
    if (book.getAuthor() != "F. Scott Fitzgerald") {
      throw std::runtime_error("Book author does not match");
    }
    if (book.getIsbn() != "978-3-16-148410-0") {
      throw std::runtime_error("Book ISBN does not match");
    }
    if (book.getGenre() != "Fiction") {
      throw std::runtime_error("Book genre does not match");
    }
    if (book.getDetails().empty()) {
      throw std::runtime_error("Book details should not be empty");
    }
  });


  tester.test("Book Availability Toggle", []() {
    Book book("B123", "The Great Gatsby", "F. Scott Fitzgerald", "978-3-16-148410-0", "Fiction");
    book.setAvailable(false);
    if (book.isAvailable() != false) {
      throw std::runtime_error("Book availability setter does not work");
    }
    book.setAvailable(true);
    if (book.isAvailable() != true) {
      throw std::runtime_error("Book availability setter does not work");
    }
    book.setAvailable(true);
    book.checkOut();
    if (book.isAvailable() != false) {
      throw std::runtime_error("Book checkOut does not work");
    }
    try {
      book.checkOut();
      throw std::runtime_error("Expected exception for checking out unavailable book");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
    book.returnItem();
    if (book.isAvailable() != true) {
      throw std::runtime_error("Book returnItem does not work");
    }
    try {
      book.returnItem();
      throw std::runtime_error("Expected exception for returning already returned book");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
  });

}

static void runTestsMagazine()
{
  UnitTest tester;
  tester.test("Create Magazine", []() {
    Magazine mag("M456", "National Geographic", "2023-09", "NatGeo Society");
    if (mag.getId() != "M456") {
      throw std::runtime_error("Magazine ID does not match");
    }
    if (mag.isAvailable() != true) {
      throw std::runtime_error("Magazine availability does not match");
    }
    if (mag.getMaxLoanDays() != 28) {
      throw std::runtime_error("Magazine max loan days does not match");
    }
    if (mag.getTitle() != "National Geographic") {
      throw std::runtime_error("Magazine title does not match");
    }
    if (mag.calculateFine(3) != 1.5) {
      throw std::runtime_error("Magazine fine calculation is incorrect");
    }
    if (mag.calculateFine(0) != 0.0) {
      throw std::runtime_error("Magazine fine calculation is incorrect");
    }
    if (mag.getItemType() != "Magazine") {
      throw std::runtime_error("Magazine type does not match");
    }
    if (mag.getDetails().empty()) {
      throw std::runtime_error("Magazine details should not be empty");
    }
  });


  tester.test("Magazine Availability Toggle", []() {
    Magazine mag("M456", "National Geographic", "2023-09", "NatGeo Society");
    mag.setAvailable(false);
    if (mag.isAvailable() != false) {
      throw std::runtime_error("Magazine availability setter does not work");
    }
    mag.setAvailable(true);
    if (mag.isAvailable() != true) {
      throw std::runtime_error("Magazine availability setter does not work");
    }
    mag.setAvailable(true);
    mag.checkOut();
    if (mag.isAvailable() != false) {
      throw std::runtime_error("Magazine checkOut does not work");
    }
    try {
      mag.checkOut();
      throw std::runtime_error("Expected exception for checking out unavailable magazine");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
    mag.returnItem();
    if (mag.isAvailable() != true) {
      throw std::runtime_error("Magazine returnItem does not work");
    }
    try {
      mag.returnItem();
      throw std::runtime_error("Expected exception for returning already returned magazine");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
  });
}

static void runTestsDvd()
{
  UnitTest tester;
  tester.test("Create DVD", []() {
    DVD dvd("D789", "Inception", "Christopher Nolan", 148);
    if (dvd.getId() != "D789") {
      throw std::runtime_error("DVD ID does not match");
    }
    if (dvd.isAvailable() != true) {
      throw std::runtime_error("DVD availability does not match");
    }
    if (dvd.getMaxLoanDays() != 7) {
      throw std::runtime_error("DVD max loan days does not match");
    }
    if (dvd.getTitle() != "Inception") {
      throw std::runtime_error("DVD title does not match");
    }
    if (dvd.calculateFine(2) != 2.0) {
      throw std::runtime_error("DVD fine calculation is incorrect");
    }
    if (dvd.calculateFine(0) != 0.0) {
      throw std::runtime_error("DVD fine calculation is incorrect");
    }
    if (dvd.getItemType() != "DVD") {
      throw std::runtime_error("DVD type does not match");
    }
    if (dvd.getDetails().empty()) {
      throw std::runtime_error("DVD details should not be empty");
    }
  });


  tester.test("DVD Availability Toggle", []() {
    DVD dvd("D789", "Inception", "Christopher Nolan", 148);
    dvd.setAvailable(false);
    if (dvd.isAvailable() != false) {
      throw std::runtime_error("DVD availability setter does not work");
    }
    dvd.setAvailable(true);
    if (dvd.isAvailable() != true) {
      throw std::runtime_error("DVD availability setter does not work");
    }
    dvd.setAvailable(true);
    dvd.checkOut();
    if (dvd.isAvailable() != false) {
      throw std::runtime_error("DVD checkOut does not work");
    }
    try {
      dvd.checkOut();
      throw std::runtime_error("Expected exception for checking out unavailable DVD");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
    dvd.returnItem();
    if (dvd.isAvailable() != true) {
      throw std::runtime_error("DVD returnItem does not work");
    }
    try {
      dvd.returnItem();
      throw std::runtime_error("Expected exception for returning already returned DVD");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
  });
}

static void runTestsStudent()
{
  UnitTest tester;
  tester.test("Create Student Patron", []() {
    Student student("P001", "Alice Smith", "alice.smith@example.com", "123", "History");
    if (student.getId() != "P001") {
      throw std::runtime_error("Student ID does not match");
    }
    if (student.getName() != "Alice Smith") {
      throw std::runtime_error("Student name does not match");
    }
    if (student.getContactInfo() != "alice.smith@example.com") {
      throw std::runtime_error("Student contact info does not match");
    }
    if (student.getMaxBorrowItems() != 5) {
      throw std::runtime_error("Student max borrow items does not match");
    }
    if (student.getPatronType() != "Student") {
      throw std::runtime_error("Student patron type does not match");
    }
    if (student.getLoanExtensionDays() != 7) {
      throw std::runtime_error("Student loan extension days do not match");
    }
  });


  tester.test("Student Activation Toggle", []() {
    Student student("P001", "Alice Smith", "alice.smith@example.com", "123", "History");
    student.setActive(false);
    if (student.isActive() != false) {
      throw std::runtime_error("Student deactivate does not work");
    }
    student.setActive(true);
    if (student.isActive() != true) {
      throw std::runtime_error("Student activate does not work");
    }
    student.setContactInfo("alice.smith@smith.com");
    if (student.getContactInfo() != "alice.smith@smith.com") {
      throw std::runtime_error("Student setContactInfo does not work");
    }
  });

}

static void runTestsFaculty()
{
  UnitTest tester;
  tester.test("Create Faculty Patron", []() {
    Faculty faculty("P002", "Dr. John Doe", "joe.doe@example.com", "F456", "Physics");
    if (faculty.getId() != "P002") {
      throw std::runtime_error("Faculty ID does not match");
    }
    if (faculty.getName() != "Dr. John Doe") {
      throw std::runtime_error("Faculty name does not match");
    }
    if (faculty.getFacultyId() != "F456") {
      throw std::runtime_error("Faculty faculty ID does not match");
    }
    if (faculty.getContactInfo() != "joe.doe@example.com") {
      throw std::runtime_error("Faculty contact info does not match");
    }
    if (faculty.getMaxBorrowItems() != 10) {
      throw std::runtime_error("Faculty max borrow items does not match");
    }
    if (faculty.getPatronType() != "Faculty") {
      throw std::runtime_error("Faculty patron type does not match");
    }
    if (faculty.getLoanExtensionDays() != 14) {
      throw std::runtime_error("Faculty loan extension days does not match");
    }
    if (faculty.getDepartment() != "Physics") {
      throw std::runtime_error("Faculty department does not match");
    }
  });


  tester.test("Faculty Activation Toggle", []() {
    Faculty faculty("P002", "Dr. Johnjoe Doe", "joe.doe@example.com", "F456", "Physics");
    faculty.setActive(false);
    if (faculty.isActive() != false) {
      throw std::runtime_error("Faculty deactivate does not work");
    }
    faculty.setActive(true);
    if (faculty.isActive() != true) {
      throw std::runtime_error("Faculty activate does not work");
    }
    faculty.setContactInfo("joe doe no email");
    if (faculty.getContactInfo() != "joe doe no email") {
      throw std::runtime_error("Faculty setContactInfo does not work");
    }
  });
}

static void runTestsPublicMember() {
  UnitTest tester;
  tester.test("Create Public Member Patron", []() {
    PublicMember member("P003", "Jane Doe", "jane.doe@example.com", "M789", "123 Main St");
    if (member.getId() != "P003") {
      throw std::runtime_error("Public Member ID does not match");
    }
    if (member.getName() != "Jane Doe") {
      throw std::runtime_error("Public Member name does not match");
    }
    if (member.getContactInfo() != "jane.doe@example.com") {
      throw std::runtime_error("Public Member contact info does not match");
    }
    if (member.getAddress() != "123 Main St") {
      throw std::runtime_error("Public Member address does not match");
    }
    if (member.getMaxBorrowItems() != 3) {
      throw std::runtime_error("Public Member max borrow items does not match");
    }
    if (member.getPatronType() != "PublicMember") {
      throw std::runtime_error("Public Member patron type does not match");
    }
    if (member.getLoanExtensionDays() != 0) {
      throw std::runtime_error("Public Member loan extension days does not match");
    }
  });
  tester.test("Public Member Activation Toggle", []() {
    PublicMember member("P003", "Jane Doe", "jane.doe@example.com", "M789", "123 Main St");
    member.setActive(false);
    if (member.isActive() != false) {
      throw std::runtime_error("Public Member deactivate does not work");
    }
    member.setActive(true);
    if (member.isActive() != true) {
      throw std::runtime_error("Public Member activate does not work");
    }
    member.setContactInfo("no address");
    if (member.getContactInfo() != "no address") {
      throw std::runtime_error("Public Member setContactInfo does not work");
    }
  });
}

static void runTestsCheckout()
{
  UnitTest tester;
  tester.test("Checkout Transaction", []() {
    auto book = std::make_shared<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian");
    auto student = std::make_shared<Student>("P001", "Alice Smith", "alice.smith@example.com", "123", "Computer Science");
    Checkout checkout(book.get(), student.get());
    if (checkout.getItem()->getId() != "B001") {
      throw std::runtime_error("Checkout item ID does not match");
    }
    if (checkout.getPatron()->getId() != "P001") {
      throw std::runtime_error("Checkout patron ID does not match");
    }
    if (checkout.getTransactionType() != "Checkout") {
      throw std::runtime_error("Checkout transaction type does not match");
    }
    if (checkout.getDetails().empty()) {
      throw std::runtime_error("Checkout details should not be empty");
    }
  });

  tester.test("Checkout Overdue and Fine Calculation", []() {
    auto book = std::make_shared<Book>("B002", "To Kill a Mockingbird", "Harper Lee", "978-0061120084", "Fiction");
    auto student = std::make_shared<Student>("P002", "Bob Johnson", "b.j@example.com", "456", "Mathematics");
    Checkout checkout(book.get(), student.get());
    // Simulate overdue by manipulating due date (not normally possible, but for testing)
    auto pastDueDate = std::chrono::system_clock::now() - std::chrono::hours(24 * 5); // 5 days ago
    // This requires modifying the Checkout class to allow setting due date for testing purposes
    checkout.setDueDate(pastDueDate);
    // For this test, we will assume the due date is 5 days ago
    if (!checkout.isOverdue()) {
      throw std::runtime_error("Checkout should be overdue");
    }
    if (checkout.calculateFine() != 2.5) {
      throw std::runtime_error("Checkout fine calculation is incorrect");
    }
  });

  tester.test("Checkout Unavailable Item", []() {
    auto book = std::make_shared<Book>("B003", "The Great Gatsby", "F. Scott Fitzgerald", "978-0743273565", "Classic");
    auto student1 = std::make_shared<Student>("P003", "Charlie Brown", "charlie.brown@example.com", "789", "Literature");
    auto student2 = std::make_shared<Student>("P004", "Daisy Miller", "daisy.miller@example.com", "012", "History");
    Checkout checkout1(book.get(), student1.get());
    try {
      Checkout checkout2(book.get(), student2.get());
      throw std::runtime_error("Expected exception for checking out unavailable item");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
  });

  tester.test("Checkout Inactive Patron", []() {
    auto book = std::make_shared<Book>("B004", "The Catcher in the Rye", "J.D. Salinger", "978-0316769488", "Fiction");
    auto student = std::make_shared<Student>("P005", "Eve Adams", "eve.adams@example.com", "345", "Philosophy");
    student->setActive(false);
    try {
      Checkout checkout(book.get(), student.get());
      throw std::runtime_error("Expected exception for inactive patron");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
  });
}

static void runTestsReturn()
{
  UnitTest tester;
  tester.test("Return Transaction", []() {
    auto book = std::make_shared < Book>("B005", "Brave New World", "Aldous Huxley", "978-0060850524", "Dystopian");
    auto student = std::make_shared<Student>("P006", "Frank Miller", "frank.miller@example.com", "678", "Engineering");
    Return returnTxn(book.get(), student.get());
    if (returnTxn.getItem()->getId() != "B005") {
      throw std::runtime_error("Return item ID does not match");
    }
    if (returnTxn.getPatron()->getId() != "P006") {
      throw std::runtime_error("Return patron ID does not match");
    }
    if (returnTxn.getTransactionType() != "Return") {
      throw std::runtime_error("Return transaction type does not match");
    }
    if (returnTxn.getDetails().empty()) {
      throw std::runtime_error("Return details should not be empty");
    }
  });

  tester.test("Invalid Return Transaction", []() {
    auto book = std::make_shared < Book>("B005", "Brave New World", "Aldous Huxley", "978-0060850524", "Dystopian");
    auto student = std::make_shared<Student>("P006", "Frank Miller", "frank.miller@example.com", "678", "Engineering");
 
    try {
      Return returnTxn(book.get(), nullptr);
      throw std::runtime_error("Expected exception for invalid return transaction");
    }
    catch (const LibraryException&) {
      // Expected exception
    }

    try {
      Return returnTxn(nullptr, student.get());
      throw std::runtime_error("Expected exception for invalid return transaction");
    }
    catch (const LibraryException&) {
      // Expected exception
    }

    try {
      Return returnTxn(nullptr, nullptr);
      throw std::runtime_error("Expected exception for invalid return transaction");
    }
    catch (const LibraryException&) {
      // Expected exception
    }

  });
}

static void runTestsLibrary()
{
  UnitTest tester;
  tester.test("Library Add Item and Patron", []() {
    Library library;
    auto book = std::make_unique<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian");
    auto student = std::make_unique<Student>("P001", "Alice Smith", "alice.smith@example.com", "123", "Computer Science");
    library.addItem(std::move(book));
    library.addPatron(std::move(student));
    library.printInventory();
    library.printPatronHistory("P001");
    library.printOverdueItems();
  });

  tester.test("Library Checkout Item", []() {
    Library library;
    auto book = std::make_unique<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian");
    auto student = std::make_unique<Student>("P001", "Alice Smith", "alice.smith@example.com", "123", "Computer Science");
    library.addItem(std::move(book));
    library.addPatron(std::move(student));
    library.checkoutItem("B001", "P001");
    library.printInventory();
    library.printPatronHistory("P001");

    try {
      library.checkoutItem("B001", "P001");
      throw std::runtime_error("Expected exception for checking out unavailable item");
    }
    catch (const LibraryException&) {
      // Expected exception
    }

    library.printOverdueItems();
    library.returnItem("B001");
    library.printInventory();

    try
    {
      library.returnItem("B001");
      throw std::runtime_error("Expected exception for returning already returned item");
    }
    catch (const LibraryException&)
    {
      // Expected exception
    }
  });

  tester.test("Library Search Items", []() {
    Library library;
    library.addItem(std::make_unique<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian"));
    library.addItem(std::make_unique<Magazine>("M001", "National Geographic", "2023-09", "NatGeo Society"));
    library.addItem(std::make_unique<DVD>("D001", "Inception", "Christopher Nolan", 148));
    auto results = library.searchItems([](const LibraryItem& item) {
      return item.getTitle().find("Inception") != std::string::npos;
      });
    if (results.size() != 1 || results[0]->getId() != "D001") {
      throw std::runtime_error("Library search items failed");
    }
  });

  tester.test("Library Overdue Items", []() {
    Library library;
    library.addItem(std::make_unique<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian"));
    library.addPatron(std::make_unique<Student>("P001", "Alice Smith", "alice.smith@example.com", "123", "Computer Science"));
    auto& checkout = library.checkoutItem("B001", "P001");
    // Simulate overdue by manipulating due date (not normally possible, but for testing)
    auto pastDueDate = std::chrono::system_clock::now() - std::chrono::hours(24 * 5); // 5 days ago
    checkout.setDueDate(pastDueDate);
    library.printOverdueItems();
  });

  tester.test("Library Patron History", []() {
    Library library;
    library.addItem(std::make_unique<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian"));
    library.addPatron(std::make_unique<Student>("P001", "Alice Smith", "alice.smith@example.com", "123", "Computer Science"));
    library.checkoutItem("B001", "P001");
    library.printPatronHistory("P001");
  });

  tester.test("Library Invalid Checkout and Return", []() {
    Library library;
    library.addItem(std::make_unique<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian"));
    library.addPatron(std::make_unique<Student>("P001", "Alice Smith", "alice.smith@example.com", "123", "Computer Science"));
    try {
      library.checkoutItem("B999", "P001");
      throw std::runtime_error("Expected exception for invalid item ID");
    }
    catch (const ItemNotFoundException&) {
      // Expected exception
    }
  });

  tester.test("Library Invalid Return", []() {
    Library library;
    library.addItem(std::make_unique<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian"));
    library.addPatron(std::make_unique<Student>("P001", "Alice Smith", "alice.smith@example.com", "123", "Computer Science"));
    try {
      library.returnItem("B001");
      throw std::runtime_error("Expected exception for returning item not checked out");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
  });

  tester.test("Invalid Patron Checkout", []() {
    Library library;
    library.addItem(std::make_unique<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian"));
    try {
      library.checkoutItem("B001", "P999");
      throw std::runtime_error("Expected exception for invalid patron ID");
    }
    catch (const LibraryException&) {
      // Expected exception
    }
  });

  tester.test("Invalid Item Checkout", []() {
    Library library;
    library.addPatron(std::make_unique<Student>("P001", "Alice Smith", "alice.smith@example.com", "123", "Computer Science"));
    try {
      library.checkoutItem("B999", "P001");
      throw std::runtime_error("Expected exception for invalid item ID");
    }
    catch (const ItemNotFoundException&) {
      // Expected exception
    }
  });
}

/**
 * Function to run all unit tests
 */
static void runTests() {
  // Run tests for library items
  runTestsBook();
  runTestsMagazine();
  runTestsDvd();

  //// Run tests for library patrons
  runTestsStudent();
  runTestsFaculty();
  runTestsPublicMember();

  // Run tests for transactions
  runTestsCheckout();
  runTestsReturn();

  runTestsLibrary();
}

/**
 * Main function with basic setup and demonstration
 */
int main() {
  std::cout << "=== Library Management System ===" << std::endl;

  // Run unit tests
  std::cout << "\nRunning unit tests..." << std::endl;
  runTests();
  //
  ////// Create sample data and demonstrate system functionality
  std::cout << "\n\nCreate sample data and demonstrate system functionality..." << std::endl;
  Library library;
  library.addItem(std::make_unique<Book>("B001", "1984", "George Orwell", "978-0451524935", "Dystopian"));
  library.addItem(std::make_unique<Magazine>("M001", "National Geographic", "2023-09", "NatGeo Society"));
  library.addItem(std::make_unique<DVD>("D001", "Inception", "Christopher Nolan", 148));
  
  library.addPatron(std::make_unique<Student>("P001", "Alice Smith", "alice.smith@example.com", "123", "Computer Science"));
  library.addPatron(std::make_unique<Faculty>("P002", "Dr. Jane Doe", "jane.doe@noemail.com", "F456", "Physics"));
  library.addPatron(std::make_unique<PublicMember>("P003", "Bob Johnson", "b.j@noemail.com", "M789", "456 Elm St"));

  library.printInventory();

  library.checkoutItem("B001", "P001");
  library.printPatronHistory("P001");

  library.checkoutItem("M001", "P002");
  library.printPatronHistory("P002");

  library.returnItem("B001");
  library.checkoutItem("D001", "P003");
  library.printPatronHistory("P003");


    

  

  //library.printInventory();
  //library.printOverdueItems();

  return 0;
}