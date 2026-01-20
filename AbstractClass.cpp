#include <iostream>

class Storage{
  public:
    virtual ~Storage() {} 
    virtual void save(const std::string& name) = 0;
};

class FileStorage: virtual public Storage {
  public:
    ~FileStorage(){
      std::cout << "FileStorage Deconstructor called" << std::endl;
    }

    void save(const std::string &name) override {
      std::cout << "FileStorage: " << name << " saved!" << std::endl;
    }
};

class DatabaseStorage: virtual public Storage {
  public:
    ~DatabaseStorage(){
      std::cout << "DatabaseStorage Deconstructor called" << std::endl;
    }

    void save(const std::string &name) override {
      std::cout << "DatabaseStorage: " << name << " saved!" << std::endl;
    }
};

class Logger: virtual public Storage {
    public:
      virtual ~Logger() {
        std::cout << "Logger Destructor called" << std::endl;
      }
      virtual void log(const std::string& message) = 0;

      void save(const std::string &name) override {
        std::cout << "Logger: " << name << " saved!" << std::endl;
      }
  };

  class FileLogger : public FileStorage, public Logger {
    public:
      ~FileLogger() override {
        std::cout << "FileLogger Destructor called" << std::endl;
      }
      void log(const std::string& message) override {
        std::cout << "FileLogger: " << message << std::endl;
      }
      // Disambiguate save() by overriding and explicitly calling desired base
      void save(const std::string& name) override {
        FileStorage::save(name); // or Logger::save(name) if you want Logger's behavior
      }
  };


int main() {

  /*Demonstrates subtype polymorphism by storing derived objects in a Storage* collection and calling save() on each.*/
  Storage* storages[] = { new FileStorage(), new DatabaseStorage() };
  for (Storage* storage : storages) {
    storage->save("example_data");
    delete storage; // Clean up to avoid memory leaks
  }

  /*Includes a simple multiple inheritance example where a class implements both Storage and another abstract interface (e.g., Logger), resolving any ambiguity using virtual inheritance if necessary.*/

  FileLogger* fileLogger = new FileLogger();
  fileLogger->save("log_data");
  delete fileLogger; // Clean up to avoid memory leaks
    
  return 0;

}

