#include <iostream>
#include <fstream>
#include <string>

class ResourceHandler
{
	private:
	std::fstream* fileStream_;
	std::string filePath_;
	bool isResourceOpen_;

	public:
		// Default Constructor: Initialize the resource handler without binding it immediately to any resource.
		ResourceHandler() : fileStream_(nullptr), filePath_(""), isResourceOpen_(false){
			std::cout << "ResourceHandler created (no resource bound)" << std::endl;
		}

		/* Parameterized Constructor: Accept parameters to bind to specific resources (e.g., file paths) upon initialization. 
			Ensure proper handling if the resource fails to open.
		*/
		ResourceHandler(const std::string& filePath) {
			fileStream_ = new std::fstream();
			fileStream_->open(filePath, std::ios::in | std::ios::out | std::ios::app);
			// Acquisition: Resources should be acquired successfully before being used.
			if(fileStream_->is_open()) {
				filePath_ = filePath;
				isResourceOpen_ = true;
				std::cout << "Resource acquired: " << filePath << std::endl;

			}
			else {
				filePath_ = "";
				isResourceOpen_ = false;
				std::cout << "failed to acquire resource: " << filePath << std::endl;
			}
		}

		// Copy Constructor: Safely copy resources, ensuring that resources are neither duplicated nor leaked.
		ResourceHandler(const ResourceHandler& other) {
			fileStream_ = new std::fstream();
			fileStream_->open(other.filePath_, std::ios::in | std::ios::out | std::ios::app);
			// Acquisition: Resources should be acquired successfully before being used.
			if(fileStream_->is_open()) {
				filePath_ = other.filePath_;
				isResourceOpen_ = true;
				std::cout << "Resource acquired: " << filePath_ << std::endl;

			}
			else {
				delete fileStream_;
				fileStream_ = nullptr;
				filePath_ = "";
				isResourceOpen_ = false;
				std::cout << "failed to acquire resource: " << other.filePath_ << std::endl;
			}
		}
		// Destructor: Ensure proper cleanup by closing any open resources.
		~ResourceHandler() {
			if(isResourceOpen_ && fileStream_ && fileStream_->is_open()) {
				fileStream_->close();
				std::cout << "Resource released: " << filePath_ << std::endl;
			}
			// Release: Resources must be released automatically when an object goes out of scope.
			delete fileStream_;
			std::cout << "ResourceHandler destroyed" << std::endl;
		}

		// Method to check if resource is available
		bool isOpen() const {
			return isResourceOpen_;
		}

		// Method to write to resource (demonstrating usage)
		void writeData(const std::string& data) {
			if (isResourceOpen_ && fileStream_) {
				*fileStream_ << data << std::endl;
				std::cout << "Data written to " << filePath_ << std::endl;
			}
			else {
				std::cout << "Cannot write - resource not available" << std::endl;
			}
		}
};

int main() {
    std::cout << "=== Testing ResourceHandler ===" << std::endl;    
    // Test default constructor
    ResourceHandler handler1;
    std::cout << "Handler1 is open: " << handler1.isOpen() << std::endl;    
    // Test parameterized constructor with valid resource
    {
        ResourceHandler handler2("test_file.txt");
        std::cout << "Handler2 is open: " << handler2.isOpen() << std::endl;        
        if (handler2.isOpen()) {
            handler2.writeData("Testing RAII resource management");
        }        
        // Test copy constructor
        ResourceHandler handler3 = handler2;
        std::cout << "Handler3 is open: " << handler3.isOpen() << std::endl;        
        std::cout << "\n=== Objects going out of scope ===" << std::endl;
    } // handler2 and handler3 destructors called here    
    // Test with invalid resource
    ResourceHandler handler4("invalid/path/file.txt");
    std::cout << "Handler4 is open: " << handler4.isOpen() << std::endl;
    handler4.writeData("This should fail");
    std::cout << "\n=== Program ending ===" << std::endl;
    return 0;
}