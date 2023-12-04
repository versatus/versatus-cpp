CXX = g++
CXX_WASM = em++

CXXFLAGS = -std=c++20

WASM_SRC = contract.wasm
WASM2WAT = wasm2wat

BOOST_ROOT = /opt/homebrew/Cellar/boost/1.83.0
BOOST_LIBS = -L$(BOOST_ROOT)/lib -lboost_system

LDFLAGS = -I$(HOME)/json/include/ -I$(BOOST_ROOT)/include


SRC = main.cpp
HDRS = ../src/versatus_cpp.hpp ../src/versatus_cpp_erc20.hpp

all: local contract.wasm contract.wat

local: $(SRC) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(BOOST_LIBS)
	
contract.wasm: $(SRC) $(HDRS)
	$(CXX_WASM) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(BOOST_LIBS)

contract.wat: $(WASM_SRC)
	$(WASM2WAT) $<  -o $@

clean:
	rm -f local contract.wasm contract.wat

.PHONY: clean

