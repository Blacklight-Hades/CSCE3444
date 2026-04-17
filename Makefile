BREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I$(BREW_PREFIX)/include
LDFLAGS = -L$(BREW_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system

TARGET = CasinoApp

# Collect sources from organized subdirectories
GAME_BJ_SRCS  := $(wildcard games/blackjack/*.cpp)
GAME_SL_SRCS  := $(wildcard games/slots/*.cpp)
GAME_RL_SRCS  := $(wildcard games/roulette/*.cpp)
CORE_SRCS     := $(wildcard core/*.cpp)
STATS_SRCS    := $(wildcard stats/*.cpp)

# Recursively find UI sources (new modular structure), excluding old/ folder
UI_SRCS       := $(shell find ui -name '*.cpp' -not -path 'ui/old/*')

ALL_SRCS := main.cpp $(GAME_BJ_SRCS) $(GAME_SL_SRCS) $(GAME_RL_SRCS) $(CORE_SRCS) $(STATS_SRCS) $(UI_SRCS)

all: $(TARGET)

$(TARGET): $(ALL_SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(ALL_SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
