#include <iostream>
#include <fstream>
#include <string>
#include <memory>

class SaveFile {
private:
    std::string filePath_;
    std::unique_ptr<std::fstream> fileStream_; // Smart pointer for RAII
    bool isResourceOpen;

public:
    SaveFile() : filePath_(""), fileStream_(nullptr), isResourceOpen(false) {
        std::cout << "ResourceHandler created (no resource bound)" << std::endl;
    }

    SaveFile(const std::string& filePath) : filePath_(""), isResourceOpen(false) {
        fileStream_ = std::make_unique<std::fstream>(filePath, std::ios::in | std::ios::out | std::ios::app);
        
        if (fileStream_->is_open()) {
            filePath_ = filePath;
            isResourceOpen = true;
            std::cout << "Resource acquired: " << filePath << std::endl;
        } else {
            std::cout << "Failed to acquire resource: " << filePath << std::endl;
        }
    }

    ~SaveFile() {
        if (isResourceOpen && fileStream_ && fileStream_->is_open()) {
            fileStream_->close();
            std::cout << "Resource released: " << filePath_ << std::endl;
        }
        std::cout << "ResourceHandler destroyed" << std::endl;
    }
};

