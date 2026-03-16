CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = src/chips.cpp
BUILD = builds

tests: test4511 test4029

testChip4511:
	$(CXX) $(CXXFLAGS) tests/4511test.cpp $(SRC) -o $(BUILD)/4511test

testChip4029:
	$(CXX) $(CXXFLAGS) tests/4029test.cpp $(SRC) -o $(BUILD)/4029test

runClock:
	$(CXX) $(CXXFLAGS) src/digitalClock.cpp $(SRC) -o $(BUILD)/digitalClock

clean:
	rm -f $(BUILD)/*