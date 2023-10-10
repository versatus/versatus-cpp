# versatus-cpp
❯ make clean

rm -f local contract.wasm contract.wat

❯ make

g++ -std=c++20 -o local main.cpp -I/Users/foobar/json/include/  

em++ -std=c++20 -o contract.wasm main.cpp -I/Users/foobar/json/include/  

wasm2wat contract.wasm  -o contract.wat



❯ ls

Makefile                   contract.wasm              contract.wat               local                      main.cpp                   sample-contract-input.json

❯



## 

## Local test ( Mach-O 64-bit executable arm64 )

## 



❯ ./local < sample-contract-input.json

inputs.application_input.amount:100

{"transactions":[{"amount":33,"recipient":"wallet_id_1"},{"amount":33,"recipient":"wallet_id_2"},{"amount":33,"recipient":"wallet_id_3"}]}



## 

## Wasmer test contract.wasm

## 



❯ wasmer contract.wasm \

< sample-contract-input.json

inputs.application_input.amount:100

{"transactions":[{"amount":33,"recipient":"wallet_id_1"},{"amount":33,"recipient":"wallet_id_2"},{"amount":33,"recipient":"wallet_id_3"}]}

