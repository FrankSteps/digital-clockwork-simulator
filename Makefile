CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = src/chips.cpp src/freqGenerator.cpp
BUILD = builds

TESTS = 4511 4029 4040 4017 frequency

.PHONY: all tests clean $(addprefix testChip,$(TESTS)) runClock

all: tests

tests: $(addprefix testChip,$(TESTS))

$(BUILD):
	mkdir -p $(BUILD)

testChip4511:
	$(CXX) $(CXXFLAGS) tests/4511test.cpp $(SRC) -o $(BUILD)/4511test

testChip4029:
	$(CXX) $(CXXFLAGS) tests/4029test.cpp $(SRC) -o $(BUILD)/4029test

testChip4040:
	$(CXX) $(CXXFLAGS) tests/4040test.cpp $(SRC) -o $(BUILD)/4040test

testChip4017:
	$(CXX) $(CXXFLAGS) tests/4017test.cpp $(SRC) -o $(BUILD)/4017test

testChipfrequency:
	$(CXX) $(CXXFLAGS) tests/freq-test.cpp $(SRC) -o $(BUILD)/freqtest


runClock:
	$(CXX) $(CXXFLAGS) src/digitalClock.cpp $(SRC) -o $(BUILD)/digitalClock

clean:
	rm -rf $(BUILD)/*