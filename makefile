CXX      := g++
CXXFLAGS := -Wall -Wextra -g --coverage -fprofile-arcs -ftest-coverage
LDFLAGS  := --coverage

TARGET := OOP-Library-System.exe
SRC    := OOP-Library-System.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

coverage: run 
	gcovr ./. --exclude-unreachable-branches --exclude-throw-branches --html --html-details -o coverage.html

clean:
	rm -f $(TARGET) *.gcda *.gcno *.gcov *.html
