
#include "../src/versatus_cpp.hpp"
#include "../src/versatus_cpp_erc20.hpp"

std::string addressToString(const Address& address) {
    std::stringstream ss;
    for (const auto& byte : address) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    ss << std::dec;
    return ss.str();
}

int main() {

    ComputeInputs inputs = ComputeInputs::gather();

    // Do contract stuff to generate proposed transactions
    std::vector<ComputeTransaction> transactions;
    uint64_t amount_each = inputs.application_input.amount / inputs.application_input.recipients.size();
    for (const auto& recipient : inputs.application_input.recipients) {
        ComputeTransaction txn;
        txn.recipient = recipient;
        txn.amount = amount_each;
        transactions.push_back(txn);
    }

    // Create output object containing proposed transactions
    ComputeOutputs output;
    output.transactions = transactions;

    // Write the smart contract results/transactions to stdout
    output.commit();

    // Create an instance of ERC20
    ERC20 myToken("MyToken", "MTK");

    Address owner{};
    owner.fill(0xAA);

    myToken.mint(owner, 10000);

    // Example usage
    std::cout << "Total Supply: " << myToken.totalSupply() << std::endl;
    std::cout << "Balance of my Address: " << addressToString(owner) << " is : " << myToken.balanceOf(owner) << std::endl;

    return 0;
}