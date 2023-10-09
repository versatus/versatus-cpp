
#include "../src/versatus_cpp.hpp"

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

    return 0;
}