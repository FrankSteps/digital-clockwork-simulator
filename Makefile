CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude $(shell pkg-config --cflags libevdev)

SRC = src/chips.cpp src/freqGenerator.cpp src/feedback.cpp
BUILD = builds

TEST_NAMES = 4511 4029 4040 4017 frequency keyboard
TEST_BINS = $(addprefix $(BUILD)/, $(addsuffix test, $(TEST_NAMES)))

CLOCK_SRC = src/digitalClockwork.cpp
CLOCK_BIN = $(BUILD)/digitalClock


.PHONY: all tests clean runClock

all: tests

$(BUILD):
	mkdir -p $(BUILD)


# TESTS
tests: $(BUILD) $(TEST_BINS)


# Regra genérica para todos os testes
$(BUILD)/%test: tests/%test.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $< $(SRC) -o $@


$(BUILD)/keyboardtest: tests/keyboardtest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(shell pkg-config --libs libevdev)


# CLOCKWORK
runClock: $(BUILD) $(CLOCK_BIN)
	./$(CLOCK_BIN)


$(CLOCK_BIN): $(CLOCK_SRC) $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(shell pkg-config --libs libevdev)



# CLEAR
clean:
	rm -rf $(BUILD)

