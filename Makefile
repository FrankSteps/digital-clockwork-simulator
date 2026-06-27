CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude $(shell pkg-config --cflags libevdev)

LIBS = $(shell pkg-config --libs libevdev) -lasound
SRC = src/chips.cpp src/freqGenerator.cpp src/feedback.cpp

BUILD = builds
TEST_NAMES = 4511 4029 4040 4017 4013 4063 555 frequency keyboard

TEST_BINS = $(addprefix $(BUILD)/, $(addsuffix test, $(TEST_NAMES)))
CLOCK_SRC = src/main.cpp src/digitalClockwork.cpp src/digitalAlarm.cpp
CLOCK_BIN = $(BUILD)/digitalClock

.PHONY: all tests clean runClock

all: tests

$(BUILD):
	mkdir -p $(BUILD)


# TESTS
tests: $(BUILD) $(TEST_BINS)

$(BUILD)/%test: tests/%test.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $< $(SRC) -o $@ $(LIBS)


$(BUILD)/keyboardtest: tests/keyboardtest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)


$(BUILD)/555test: tests/555test.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $< $(SRC) -o $@ $(LIBS) -lasound



# CLOCKWORK
runClock: $(BUILD) $(CLOCK_BIN)
	./$(CLOCK_BIN)


$(CLOCK_BIN): $(CLOCK_SRC) $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)


# CLEAR
clean:
	rm -rf $(BUILD)