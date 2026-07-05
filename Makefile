CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LIBS = -lpthread

SRC = src/chips.cpp src/freqGenerator.cpp src/feedback.cpp
BUILD = builds

TEST_NAMES = 4511 4029 4040 4017 4013 4063 555 frequency keyboard
TEST_BINS = $(addprefix $(BUILD)/, $(addsuffix test, $(TEST_NAMES)))

CLOCK_SRC = src/main.cpp src/digitalClockwork.cpp src/digitalAlarm.cpp
CLOCK_BIN = $(BUILD)/digitalClock


# OS DETECTION
UNAME_S := $(shell uname -s 2>/dev/null)

ifeq ($(UNAME_S),Linux)
	KEYBOARD_SRC := src/keyboard_linux.cpp
	AUDIO_SRC := src/audio_output_linux.cpp
	CXXFLAGS += $(shell pkg-config --cflags libevdev)
	LIBS += $(shell pkg-config --libs libevdev) -lasound
endif

ifeq ($(UNAME_S),Darwin)
	KEYBOARD_SRC := src/keyboard_macintosh.cpp
	AUDIO_SRC := src/audio_output_macintosh.cpp
	LIBS += -framework ApplicationServices -framework Carbon -framework AudioToolbox
endif

ifeq ($(OS),Windows_NT)
	KEYBOARD_SRC := src/keyboard_windows.cpp
	AUDIO_SRC := src/audio_output_windows.cpp
	LIBS += -lwinmm
endif


CLOCK_SRC += $(KEYBOARD_SRC) $(AUDIO_SRC)

.PHONY: all tests clean clock

all: tests


$(BUILD):
	mkdir -p $(BUILD)



# TESTS
tests: $(BUILD) $(TEST_BINS)

$(BUILD)/%test: tests/%test.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $< $(SRC) -o $@ $(LIBS)

$(BUILD)/555test: tests/555test.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $< $(SRC) -o $@ $(LIBS)

$(BUILD)/keyboardtest: tests/keyboardtest.cpp $(KEYBOARD_SRC)
	$(CXX) $(CXXFLAGS) $< $(KEYBOARD_SRC) -o $@ $(LIBS)



# CLOCKWORK
clockwork: $(BUILD) $(CLOCK_BIN)

$(CLOCK_BIN): $(CLOCK_SRC) $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)


# CLEAR
clean:
	rm -rf $(BUILD)