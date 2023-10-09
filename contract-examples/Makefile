CXX = g++
CXX_WASM = em++

CXXFLAGS = -std=c++20

WASM_SRC = contract.wasm
WASM2WAT = wasm2wat

LDFLAGS = -I$(HOME)/json/include/  # Add any additional libraries here


SRC = main.cpp
HDRS = ../src/versatus_cpp.hpp

all: local contract.wasm contract.wat

local: $(SRC) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
	
contract.wasm: $(SRC) $(HDRS)
	$(CXX_WASM) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

contract.wat: $(WASM_SRC)
	$(WASM2WAT) $<  -o $@

clean:
	rm -f local contract.wasm contract.wat

.PHONY: clean

