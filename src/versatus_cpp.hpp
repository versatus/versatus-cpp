#ifndef VERSATUS_CPP_HPP
#define VERSATUS_CPP_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class ApplicationInputs {
public:
    std::string contract_fn;
    uint64_t amount;
    std::vector<std::string> recipients;
};

class ProtocolInputs {
public:
    int32_t version;
    uint64_t block_height;
    uint64_t block_time;
};

class AccountInfo {
public:
    std::string account_address;
    uint64_t account_balance;
};

const char* const versionStr = "version";

const char* const accountInfoStr = "accountInfo";
const char* const accountAddressStr = "accountAddress";
const char* const accountBalanceStr = "accountBalance";

const char* const protocolInputStr = "protocolInput";
const char* const blockHeightStr = "blockHeight";
const char* const blockTimeStr = "blockTime";
const char* const applicationInputStr = "applicationInput";
const char* const contractFnStr = "contractFn";
const char* const amountStr = "amount";
const char* const recipientsStr = "recipients";

class ComputeInputs {
public:
    int32_t version;
    AccountInfo account_info;
    ProtocolInputs protocol_input;
    ApplicationInputs application_input;

    static ComputeInputs gather() {
 
        json json_obj;
        cin >> json_obj;
        ComputeInputs inputs;
        
        inputs.version = json_obj[versionStr];
        inputs.account_info.account_address = json_obj[accountInfoStr][accountAddressStr];
        inputs.account_info.account_balance = json_obj[accountInfoStr][accountBalanceStr];
        inputs.protocol_input.version = json_obj[protocolInputStr][versionStr];
        inputs.protocol_input.block_height = json_obj[protocolInputStr][blockHeightStr];
        inputs.protocol_input.block_time = json_obj[protocolInputStr][blockTimeStr];
        inputs.application_input.contract_fn = json_obj[applicationInputStr][contractFnStr];
        inputs.application_input.amount = json_obj[applicationInputStr][amountStr];
        inputs.application_input.recipients = json_obj[applicationInputStr][recipientsStr].get<std::vector<std::string>>();
    
        cout << "inputs.application_input.amount:" << inputs.application_input.amount << endl;
        
        return inputs;
    }
};

class ComputeTransaction {
public:
    std::string recipient;
    uint64_t amount;

    // Custom serialization function for ComputeTransaction
    void to_json(json& j) const {
        j = json{{"recipient", recipient}, {"amount", amount}};
    }

    // Custom deserialization function for ComputeTransaction
    void from_json(const json& j) {
        j.at("recipient").get_to(recipient);
        j.at("amount").get_to(amount);
    }
};

class ComputeOutputs {
public:
    std::vector<ComputeTransaction> transactions;

    json toJSON() {
        json json_obj;
        json_obj["transactions"] = json::array();

        for (const auto& transaction : transactions) {
            json transaction_json;
            transaction.to_json(transaction_json);
            json_obj["transactions"].push_back(transaction_json);
        }

        return json_obj;
    }

    void commit() {
        std::cout << toJSON() << std::endl;
    }

};

#endif  // VERSATUS_CPP_HPP
