#include <iostream>
#include <map>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cstdint>
using namespace std;

// ========================================
// STEP 1: Memory Manager Class Implementation
// ========================================

class MemoryManager {
private:
    struct AllocationInfo {
        size_t size;
        time_t timestamp;
        bool isArray;
        string location;  // Could store file/line if available
        
        AllocationInfo(size_t s = 0, bool arr = false, const string& loc = "") 
            : size(s), timestamp(time(nullptr)), isArray(arr), location(loc) {}
    };
    
    map<void*, AllocationInfo> allocations;
    size_t totalAllocations;
    size_t totalDeallocations;
    size_t currentAllocatedBytes;
    size_t peakAllocatedBytes;
    size_t totalBytesAllocated;
    ofstream logFile;

public:
    MemoryManager() : totalAllocations(0), totalDeallocations(0), 
                     currentAllocatedBytes(0), peakAllocatedBytes(0), 
                     totalBytesAllocated(0) {
        logFile.open("memory_log.txt", ios::app);
        logToFile("Memory Manager initialized");
        cout << "Memory Manager started - logging to memory_log.txt" << endl;
    }
    
    ~MemoryManager() {
        logToFile("Memory Manager shutting down");
        cout << "\n=== Final Memory Report ===" << endl;
        reportLeaks();
        printStatistics();
        
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void* allocateMemory(size_t size, bool isArray = false) {
        if (size == 0) {
            logToFile("Warning: Attempted to allocate 0 bytes");
            return nullptr;
        }
        
        void* ptr = malloc(size);
        if (!ptr) {
            logToFile("CRITICAL: Memory allocation failed for " + to_string(size) + " bytes");
            throw bad_alloc();
        }
        
        // Track the allocation
        allocations[ptr] = AllocationInfo(size, isArray, "Unknown");
        totalAllocations++;
        currentAllocatedBytes += size;
        totalBytesAllocated += size;
        
        if (currentAllocatedBytes > peakAllocatedBytes) {
            peakAllocatedBytes = currentAllocatedBytes;
        }
        
        string logMsg = "ALLOC: " + to_string(size) + " bytes at " + 
                       to_string(reinterpret_cast<uintptr_t>(ptr)) + 
                       (isArray ? " [ARRAY]" : " [SINGLE]");
        logToFile(logMsg);
        
        return ptr;
    }
    
    void deallocateMemory(void* ptr, bool isArray = false) {
        if (!ptr) {
            logToFile("Warning: Attempted to delete null pointer");
            return;
        }
        
        auto it = allocations.find(ptr);
        if (it == allocations.end()) {
            logToFile("ERROR: Attempted to delete untracked memory at " + 
                     to_string(reinterpret_cast<uintptr_t>(ptr)));
            cout << "WARNING: Deleting untracked memory!" << endl;
            return;
        }
        
        // Check for array/single mismatch
        if (it->second.isArray != isArray) {
            string error = "ERROR: Memory type mismatch - allocated as " + 
                          string(it->second.isArray ? "array" : "single") + 
                          " but deleted as " + string(isArray ? "array" : "single");
            logToFile(error);
            cout << "WARNING: " << error << endl;
        }
        
        size_t size = it->second.size;
        currentAllocatedBytes -= size;
        totalDeallocations++;
        
        string logMsg = "DEALLOC: " + to_string(size) + " bytes at " + 
                       to_string(reinterpret_cast<uintptr_t>(ptr)) + 
                       (isArray ? " [ARRAY]" : " [SINGLE]");
        logToFile(logMsg);
        
        allocations.erase(it);
        free(ptr);
    }
    
    void* allocateArray(size_t size) {
        return allocateMemory(size, true);
    }
    
    void deallocateArray(void* ptr) {
        deallocateMemory(ptr, true);
    }
    
    // ========================================
    // STEP 3: Leak Detection and Reporting
    // ========================================
    
    void reportLeaks() {
        cout << "\n=== Memory Leak Report ===" << endl;
        
        if (allocations.empty()) {
            cout << "✓ No memory leaks detected!" << endl;
            logToFile("LEAK REPORT: No leaks detected");
            return;
        }
        
        cout << "⚠ Memory leaks detected:" << endl;
        size_t totalLeakedBytes = 0;
        int leakCount = 0;
        
        for (const auto& pair : allocations) {
            void* addr = pair.first;
            const AllocationInfo& info = pair.second;
            
            cout << "  Leak #" << ++leakCount << ":" << endl;
            cout << "    Address: " << addr << endl;
            cout << "    Size: " << info.size << " bytes" << endl;
            cout << "    Type: " << (info.isArray ? "Array" : "Single") << endl;
            cout << "    Allocated: " << ctime(&info.timestamp);
            cout << endl;
            
            totalLeakedBytes += info.size;
        }
        
        cout << "Total leaked: " << leakCount << " blocks, " << totalLeakedBytes << " bytes" << endl;
        
        string leakLog = "LEAK REPORT: " + to_string(leakCount) + " leaks, " + 
                        to_string(totalLeakedBytes) + " bytes";
        logToFile(leakLog);
    }
    
    void printStatistics() {
        cout << "\n=== Memory Usage Statistics ===" << endl;
        cout << "Total allocations: " << totalAllocations << endl;
        cout << "Total deallocations: " << totalDeallocations << endl;
        cout << "Current allocated bytes: " << currentAllocatedBytes << endl;
        cout << "Peak allocated bytes: " << peakAllocatedBytes << endl;
        cout << "Total bytes ever allocated: " << totalBytesAllocated << endl;
        cout << "Active allocations: " << allocations.size() << endl;
        
        if (totalAllocations > 0) {
            double avgAllocationSize = static_cast<double>(totalBytesAllocated) / totalAllocations;
            cout << "Average allocation size: " << fixed << setprecision(2) << avgAllocationSize << " bytes" << endl;
        }
    }
    
    // ========================================
    // STEP 4: Enhanced Debugging Information
    // ========================================
    
    void logToFile(const string& message) {
        if (!logFile.is_open()) return;
        
        time_t now = time(nullptr);
        char timeStr[26];
        
        ctime_r(&now, timeStr);
        timeStr[24] = '\0';  // Remove newline
        
        logFile << "[" << timeStr << "] " << message << endl;
        logFile.flush();
    }
    
    bool validateMemory() {
        cout << "\n=== Memory Validation ===" << endl;
        
        // Check for any obvious inconsistencies
        if (totalDeallocations > totalAllocations) {
            cout << "❌ ERROR: More deallocations than allocations!" << endl;
            return false;
        }
        
        size_t calculatedBytes = 0;
        for (const auto& pair : allocations) {
            calculatedBytes += pair.second.size;
        }
        
        if (calculatedBytes != currentAllocatedBytes) {
            cout << "❌ ERROR: Byte count mismatch!" << endl;
            return false;
        }
        
        cout << "✓ Memory validation passed" << endl;
        return true;
    }
    
    // Additional utility methods
    size_t getCurrentUsage() const { return currentAllocatedBytes; }
    size_t getPeakUsage() const { return peakAllocatedBytes; }
    size_t getActiveAllocations() const { return allocations.size(); }
};

// Global instance of MemoryManager
MemoryManager* globalMemoryManager = nullptr;

// ========================================
// STEP 2: Override Global New and Delete Operators
// ========================================

void* operator new(size_t size) {
    if (globalMemoryManager) {
        return globalMemoryManager->allocateMemory(size, false);
    }
    // Fallback if manager not initialized
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    if (globalMemoryManager) {
        globalMemoryManager->deallocateMemory(ptr, false);
    } else if (ptr) {
        free(ptr);
    }
}

void* operator new[](size_t size) {
    if (globalMemoryManager) {
        return globalMemoryManager->allocateArray(size);
    }
    return malloc(size);
}

void operator delete[](void* ptr) noexcept {
    if (globalMemoryManager) {
        globalMemoryManager->deallocateArray(ptr);
    } else if (ptr) {
        free(ptr);
    }
}

// ========================================
// STEP 5: Testing Framework Implementation
// ========================================

class MemoryManagerTester {
public:
    static void testBasicOperations() {
        cout << "\n--- Testing Basic Operations ---" << endl;
        
        // Test single object allocation
        int* singleInt = new int(42);
        cout << "Allocated single int with value: " << *singleInt << endl;
        delete singleInt;
        cout << "Deallocated single int" << endl;
        
        // Test basic allocation pattern
        char* buffer = new char[100];
        strcpy(buffer, "Hello, Memory Manager!");
        cout << "Allocated and populated buffer: " << buffer << endl;
        delete[] buffer;
        cout << "Deallocated buffer" << endl;
        
        globalMemoryManager->validateMemory();
    }
    
    static void testLeakDetection() {
        cout << "\n--- Testing Leak Detection ---" << endl;
        
        // Intentionally create leaks for testing
        cout << "Creating intentional memory leaks for testing..." << endl;
        
        int* leakyInt = new int(999);
        char* leakyBuffer = new char[50];
        double* leakyArray = new double[10];
        
        cout << "Created 3 intentional leaks" << endl;
        cout << "Running leak detection..." << endl;
        globalMemoryManager->reportLeaks();
        
        // Clean up the leaks
        cout << "Cleaning up test leaks..." << endl;
        delete leakyInt;
        delete[] leakyBuffer;
        delete[] leakyArray;
    }
    
    static void testArrayOperations() {
        cout << "\n--- Testing Array Operations ---" << endl;
        
        // Test various array sizes
        int* smallArray = new int[5];
        for (int i = 0; i < 5; i++) {
            smallArray[i] = i * i;
        }
        
        float* mediumArray = new float[100];
        for (int i = 0; i < 100; i++) {
            mediumArray[i] = i * 0.5f;
        }
        
        char* largeArray = new char[1000];
        memset(largeArray, 'A', 999);
        largeArray[999] = '\0';
        
        cout << "Created arrays of sizes 5, 100, and 1000" << endl;
        globalMemoryManager->printStatistics();
        
        delete[] smallArray;
        delete[] mediumArray;
        delete[] largeArray;
        
        cout << "Cleaned up all arrays" << endl;
    }
    
    static void testErrorConditions() {
        cout << "\n--- Testing Error Conditions ---" << endl;
        
        // Test null pointer deletion
        cout << "Testing null pointer deletion..." << endl;
        int* nullPtr = nullptr;
        delete nullPtr;  // Should handle gracefully
        
        // Test zero-size allocation
        cout << "Testing zero-size allocation..." << endl;
        void* zeroPtr = globalMemoryManager->allocateMemory(0);
        if (zeroPtr) {
            globalMemoryManager->deallocateMemory(zeroPtr);
        }
        
        // Test type mismatch (allocate as single, delete as array)
        cout << "Testing allocation type mismatch..." << endl;
        int* mismatchPtr = new int(123);
        // This should generate a warning
        globalMemoryManager->deallocateArray(mismatchPtr);
        
        cout << "Error condition tests completed" << endl;
    }
    
    static void runAllTests() {
        cout << "=== Memory Manager Test Suite ===" << endl;
        
        testBasicOperations();
        testArrayOperations();
        testErrorConditions();
        testLeakDetection();
        
        cout << "\n=== Test Suite Complete ===" << endl;
        globalMemoryManager->printStatistics();
    }
};

// ========================================
// IMAGE PROCESSING SIMULATION (Scenario Context)
// ========================================

class ImageProcessor {
private:
    int width, height;
    unsigned char* imageData;
    
public:
    ImageProcessor(int w, int h) : width(w), height(h), imageData(nullptr) {
        cout << "Creating " << w << "x" << h << " image processor..." << endl;
        size_t dataSize = width * height * 3;  // RGB
        imageData = new unsigned char[dataSize];
        
        // Initialize with gradient pattern
        for (int i = 0; i < width * height * 3; i++) {
            imageData[i] = i % 256;
        }
        
        cout << "Allocated " << dataSize << " bytes for image data" << endl;
    }
    
    ~ImageProcessor() {
        if (imageData) {
            cout << "Cleaning up image processor (" << width << "x" << height << ")" << endl;
            delete[] imageData;
            imageData = nullptr;
        }
    }
    
    void processImage() {
        cout << "Processing " << width << "x" << height << " image..." << endl;
        
        // Simulate processing by allocating temporary buffers
        size_t tempSize = width * height;
        unsigned char* tempBuffer1 = new unsigned char[tempSize];
        unsigned char* tempBuffer2 = new unsigned char[tempSize];
        
        // Simulate some processing work
        for (size_t i = 0; i < tempSize; i++) {
            tempBuffer1[i] = imageData[i * 3];     // Extract red channel
            tempBuffer2[i] = imageData[i * 3 + 1]; // Extract green channel
        }
        
        cout << "Image processing complete (used " << (tempSize * 2) << " bytes temp memory)" << endl;
        
        // Clean up temporary buffers
        delete[] tempBuffer1;
        delete[] tempBuffer2;
    }
    
    void resize(int newWidth, int newHeight) {
        cout << "Resizing image from " << width << "x" << height 
             << " to " << newWidth << "x" << newHeight << endl;
        
        size_t newSize = newWidth * newHeight * 3;
        unsigned char* newData = new unsigned char[newSize];
        
        // Simple resize simulation (just fill with pattern)
        for (size_t i = 0; i < newSize; i++) {
            newData[i] = (i * 255) / newSize;
        }
        
        // Clean up old data
        delete[] imageData;
        
        // Update to new data
        imageData = newData;
        width = newWidth;
        height = newHeight;
        
        cout << "Resize complete" << endl;
    }
};

// ========================================
// MAIN FUNCTION - Complete Demonstration
// ========================================

int main() {
    cout << "=== Memory Tracking and Management System - SOLUTION ===" << endl;
    cout << "Image Processing Application Scenario" << endl;
    
    // Initialize global memory manager
    globalMemoryManager = new MemoryManager();
    
    cout << "\n--- Phase 1: Basic Memory Manager Testing ---" << endl;
    MemoryManagerTester::testBasicOperations();
    
    cout << "\n--- Phase 2: Image Processing Simulation ---" << endl;
    {
        ImageProcessor img1(800, 600);
        img1.processImage();
        
        ImageProcessor img2(1024, 768);
        img2.processImage();
        
        cout << "\nMemory usage during image processing:" << endl;
        globalMemoryManager->printStatistics();
        
        img1.resize(400, 300);
        img2.resize(512, 384);
        
    }  // ImageProcessor objects destroyed here
    
    cout << "\nAfter image processors destroyed:" << endl;
    globalMemoryManager->printStatistics();
    
    cout << "\n--- Phase 3: Comprehensive Testing ---" << endl;
    MemoryManagerTester::runAllTests();
    
    cout << "\n--- Phase 4: Final Memory Validation ---" << endl;
    globalMemoryManager->validateMemory();
    
    // Clean up global memory manager
    delete globalMemoryManager;
    globalMemoryManager = nullptr;
    
    cout << "\n=== Program Complete - Check memory_log.txt for detailed logs ===" << endl;
    return 0;
}

/*
SOLUTION FEATURES IMPLEMENTED:

STEP 1 - Memory Manager Class:
✓ Complete AllocationInfo struct with size, timestamp, type tracking
✓ Map-based allocation tracking with comprehensive metadata
✓ Statistical counters for allocations, deallocations, peak usage
✓ Robust error handling and validation

STEP 2 - Global Operator Overrides:
✓ Full override of new, delete, new[], delete[] operators
✓ Seamless integration with MemoryManager
✓ Fallback behavior for uninitialized manager
✓ Type-safe array vs single object tracking

STEP 3 - Leak Detection and Reporting:
✓ Comprehensive leak reporting with detailed information
✓ Memory statistics with peak usage tracking
✓ Clear console output and log file reporting
✓ Automated leak detection on manager destruction

STEP 4 - Enhanced Debugging:
✓ File logging with timestamps for all operations
✓ Memory validation and integrity checking
✓ Warning system for common errors (type mismatches)
✓ Detailed error reporting and handling

STEP 5 - Testing Framework:
✓ Complete test suite covering all scenarios
✓ Basic operations, arrays, error conditions
✓ Intentional leak creation for testing
✓ Edge case handling and validation

ADVANCED FEATURES:
✓ Image processing scenario implementation
✓ Real-world memory usage patterns
✓ Automatic cleanup and reporting
✓ Professional-grade error handling
✓ Comprehensive logging system

MEMORY SAFETY FEATURES:
✓ Null pointer handling
✓ Double deletion prevention
✓ Type mismatch detection
✓ Memory validation and integrity checks
✓ Graceful error recovery

This implementation provides enterprise-level memory tracking
capabilities suitable for large-scale C++ applications.
*/