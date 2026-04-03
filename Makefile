CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = src/chips.cpp src/freqGenerator.cpp src/feedback.cpp
BUILD = builds

TEST_NAMES = 4511 4029 4040 4017 frequency
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



# CLOCKWORK
runClock: $(BUILD) $(CLOCK_BIN)
	./$(CLOCK_BIN)


$(CLOCK_BIN): $(CLOCK_SRC) $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@


# CLEAR
clean:
	rm -rf $(BUILD)