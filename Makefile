CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
# Boost and nlohmann/json are header-only — point at whichever prefix
# brew/apt installs them under. The brew path is the macOS default.
INCLUDES ?= -I/opt/homebrew/include -I/usr/local/include

main: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

test: main
	./main testcase1.json
	./main testcase.json

clean:
	rm -f main *.o
	rm -rf *.dSYM

.PHONY: test clean
