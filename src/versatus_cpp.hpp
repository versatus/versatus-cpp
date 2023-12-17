#ifndef VERSATUS_CPP_HPP
#define VERSATUS_CPP_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <cassert>

using namespace std;
using json = nlohmann::json;

// Assumption: Eth Address is 20 bytes
constexpr std::size_t ADDRESS_SIZE = 20;
using Address = std::array<uint8_t, ADDRESS_SIZE>;
using uint256_t = boost::multiprecision::uint256_t;

// Generic function to print an object to JSON string
template <typename T>
std::string print_json(const T& obj) {
    json j;
    obj.to_json(j);
    return j.dump(2);  // The second argument specifies the indentation level
}

std::string uint256_to_hex(const uint256_t& value) {
    std::stringstream ss;
    ss << std::hex << value;
    return ss.str();
}

// Converts string to Eth address of ADDRESS_SIZE bytes
Address convertStringToAddress(const std::string& account_address) {
    try {
        // Check if the address starts with "0x"
        if (account_address.size() >= 2 && account_address.substr(0, 2) == "0x") {
            // Remove the "0x" prefix
            auto hex_value = account_address.substr(2);

            // Ensure the hex value length is valid
            if (hex_value.size() > (ADDRESS_SIZE * 2)) {
                throw std::length_error("Input address string is too long");
            }

            Address result{};

            for (std::size_t i = 0; i < result.size(); ++i) {
                std::string byte_str = hex_value.substr(i * 2, 2);
                result[i] = static_cast<uint8_t>(std::stoi(byte_str, nullptr, 16));
            }

            return result;
        } else {
            throw std::invalid_argument("Input address string must start with '0x'");
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR Address parse: " << e.what() << std::endl;
        assert(0);
        // Return the magic address in case of an exception
        return Address{0xDE, 0xAD, 0xBE, 0xEF};
    }
}

std::string addressToString(const Address& address) {
    std::stringstream ss;
    ss << "0x"; // Add "0x" at the beginning

    for (const auto& byte : address) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    ss << std::dec;
    return ss.str();
}


class ApplicationInputs {
    const char* const contractFnStr = "contractFn";
    const char* const amountStr = "amount";
    const char* const recipientsStr = "recipients";

public:
    std::string contract_fn;
    uint64_t amount;
    std::vector<std::string> recipients;

    // Custom serialization function for ApplicationInputs
    void to_json(json& j) const {
        j[contractFnStr] = contract_fn;
        j[amountStr] = amount;
        j[recipientsStr] = recipients;
    }

    // Custom deserialization function for ApplicationInputs
    void from_json(const json& j) {
        j.at(contractFnStr).get_to(contract_fn);
        j.at(amountStr).get_to(amount);
        j.at(recipientsStr).get_to(recipients);
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};

class NameInput {
public:
    std::string value;

    void to_json(json& j) const {
        if (!value.empty())
            j["value"] = value;
    }

    void from_json(const json& j) {
        j.at("value").get_to(value);
    }
};

class SymbolInput {
public:
    std::string value;

    void to_json(json& j) const {
        if (!value.empty())
            j["value"] = value;
    }

    void from_json(const json& j) {
        j.at("value").get_to(value);
    }
};

class DecimalsInput {
public:
    uint8_t value;

    void to_json(json& j) const {
        j = json{{"value", value}};
    }

    void from_json(const json& j) {
        j.at("value").get_to(value);
    }
};

class TotalSupplyInput {
public:
    uint256_t value;

    void to_json(json& j) const {
        j = json{{"value", value.str()}};
    }

    void from_json(const json& j) {
        std::string value_str = j.at("value");
        value = boost::multiprecision::uint256_t(value_str);
    }
};

class BalanceOfInput {
public:
    Address account;

    void to_json(json& j) const {
        j = json{{"address", addressToString(account)}};
    }

    void from_json(const json& j) {
        std::string account_str = j.at("address");
        account = convertStringToAddress(account_str);
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};

class AllowanceInput {
public:
    Address owner;
    Address spender;

    // Custom serialization function for AllowanceInput
    void to_json(json& j) const {
        j = json{{"owner", addressToString(owner)}, {"spender", addressToString(spender)}};
    }

    // Custom deserialization function for AllowanceInput
    void from_json(const json& j) {
        std::string owner_str = j.at("owner");
        owner = convertStringToAddress(owner_str);
        std::string spender_str = j.at("spender");
        spender = convertStringToAddress(spender_str);
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};

class TransferInput {
public:
    Address address;
    uint256_t value;

    // Custom serialization function for TransferInput
    void to_json(json& j) const {
        j = json{{"address", addressToString(address)}, {"value", "0x" + uint256_to_hex(value)}};
    }

    // Custom deserialization function for TransferInput
    void from_json(const json& j) {
        std::string address_str = j.at("address");
        address = convertStringToAddress(address_str);
        std::string value_str = j.at("value");
        value = boost::multiprecision::uint256_t(value_str);
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};

class TransferFromInput {
public:
    Address from;
    Address to;
    uint256_t value;

    // Custom serialization function for TransferFromInput
    void to_json(json& j) const {
        j = json{{"from", addressToString(from)}, {"to", addressToString(to)}, {"value", "0x" + uint256_to_hex(value)}};
    }

    // Custom deserialization function for TransferFromInput
    void from_json(const json& j) {
        std::string from_str = j.at("from");
        from = convertStringToAddress(from_str);
        std::string to_str = j.at("to");
        to = convertStringToAddress(to_str);
        std::string value_str = j.at("value");
        value = boost::multiprecision::uint256_t(value_str);
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};

class ApproveInput {
public:
    Address address;
    uint256_t value;

    // Custom serialization function for ApproveInput
    void to_json(json& j) const {
        j = json{{"address", addressToString(address)}, {"value", "0x" + uint256_to_hex(value)}};
    }

    // Custom deserialization function for ApproveInput
    void from_json(const json& j) {
        std::string address_str = j.at("address");
        address = convertStringToAddress(address_str);
        std::string value_str = j.at("value");
        value = boost::multiprecision::uint256_t(value_str);
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};


class ERC20Inputs {
public:
    NameInput name;
    SymbolInput symbol;
    DecimalsInput decimals;
    TotalSupplyInput total_supply;
    BalanceOfInput balance_of;
    TransferInput transfer;
    TransferFromInput transfer_from;
    ApproveInput approve;
    AllowanceInput allowance;

    ERC20Inputs()
        : name(), symbol(), decimals(), total_supply(), balance_of(),
          transfer(), transfer_from(), approve(), allowance() {}

    // Custom serialization function for ERC20Inputs
    void to_json(json& j) const {
        if (!name.value.empty()) name.to_json(j);
        if (!symbol.value.empty()) symbol.to_json(j);
        if (decimals.value) decimals.to_json(j);
        if (total_supply.value) total_supply.to_json(j);
        if (balance_of.account != Address{}) balance_of.to_json(j);
        if (transfer.address != Address{}) transfer.to_json(j);
        if (transfer_from.from != Address{} && transfer_from.to != Address{} && transfer_from.value != 0) transfer_from.to_json(j);
        if (approve.address != Address{} && approve.value != 0) approve.to_json(j);
        if (allowance.owner != Address{} && allowance.spender != Address{}) allowance.to_json(j);

    }

    void from_json(const json& j) {
        if (j.find("name") != j.end()) name.from_json(j.at("name"));
        if (j.find("symbol") != j.end()) symbol.from_json(j.at("symbol"));
        if (j.find("decimals") != j.end()) decimals.from_json(j.at("decimals"));
        if (j.find("totalSupply") != j.end()) total_supply.from_json(j.at("totalSupply"));

        if (j.find("balanceOf") != j.end()) balance_of.from_json(j.at("balanceOf"));
        if (j.find("transfer") != j.end()) {
            transfer.from_json(j.at("transfer"));
        }
        if (j.find("transferFrom") != j.end()) transfer_from.from_json(j.at("transferFrom"));
        if (j.find("approve") != j.end()) approve.from_json(j.at("approve"));
        if (j.find("allowance") != j.end()) allowance.from_json(j.at("allowance"));
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }

};

class FunctionInputs {
public:
    ERC20Inputs erc20;

    // Custom serialization function for FunctionInputs
    void to_json(json& j) const {
        erc20.to_json(j);
    }

    // Custom deserialization function for FunctionInputs
    void from_json(const json& j) {
        if (j.find("erc20") != j.end()) {
            cout << "erc20" << endl;
            erc20.from_json(j.at("erc20"));
        }
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};

class ContractInputs {
public:
    std::string contract_fn;
    FunctionInputs function_inputs;

    // Custom serialization function for ContractInput
    void to_json(json& j) const {
        j["contractFn"] = contract_fn;

        json functionInputsJson;
        function_inputs.to_json(functionInputsJson);
        j["functionInputs"] = functionInputsJson;
    }

    // Custom deserialization function for ContractInput
    void from_json(const json& j) {
        j.at("contractFn").get_to(contract_fn);
        function_inputs.from_json(j.at("functionInputs"));
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};

class ProtocolInputs {
    const char* const versionStr = "version";
    const char* const blockHeightStr = "blockHeight";
    const char* const blockTimeStr = "blockTime";
public:
    int32_t version;
    uint64_t block_height;
    uint64_t block_time;

    // Custom serialization function for ProtocolInputs
    void to_json(json& j) const {
        j = json{{versionStr, version}, {blockHeightStr, block_height}, {blockTimeStr, block_time}};
    }

    // Custom deserialization function for ProtocolInputs
    void from_json(const json& j) {
        if (j.find(versionStr) != j.end()) j.at(versionStr).get_to(version);
        if (j.find(blockHeightStr) != j.end()) j.at(blockHeightStr).get_to(block_height);
        if (j.find(blockTimeStr) != j.end()) j.at(blockTimeStr).get_to(block_time);
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};

class AccountInfo {
    const char* const accountAddressStr = "accountAddress";
    const char* const accountBalanceStr = "accountBalance";

public:
    Address account_address;
    uint256_t account_balance;

    // Custom serialization function for AccountInfo
    void to_json(json& j) const {
        j[accountAddressStr] = addressToString(account_address);
        // Convert uint256_t to a string in hexadecimal format
        std::ostringstream oss;
        oss << std::hex << "0x" << account_balance;
        j[accountBalanceStr] = oss.str();
    }

    // Custom deserialization function for AccountInfo
    void from_json(const json& j) {
        std::string account_address_str = j.at(accountAddressStr);
        account_address = convertStringToAddress(account_address_str);

        std::string account_balance_str = j.at(accountBalanceStr);
        account_balance = boost::multiprecision::uint256_t(account_balance_str);
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(2);  // The second argument specifies the indentation level
    }
};


class ComputeInputs {

public:
    int32_t version;
    AccountInfo account_info;
    ProtocolInputs protocol_input;
    ApplicationInputs application_input;
    ContractInputs contract_input;

    static ComputeInputs gather() {
        const char* const versionStr = "version";
        const char* const applicationInputStr = "applicationInput";
        const char* const accountInfoStr = "accountInfo";
        const char* const protocolInputStr = "protocolInput";
        const char* const contractInputStr = "contractInput";

        json json_obj;
        cin >> json_obj;
        ComputeInputs inputs;
        
        inputs.version = json_obj[versionStr];

        inputs.account_info.from_json(json_obj[accountInfoStr]);

        inputs.protocol_input.from_json(json_obj[protocolInputStr]);

        if (json_obj.contains(applicationInputStr)) {
            inputs.application_input.from_json(json_obj[applicationInputStr]);
            cout << "Object " << applicationInputStr << " :" << print_json(inputs.application_input) << std::endl;
        }
        
        if (json_obj.contains(contractInputStr)) {
            inputs.contract_input.from_json(json_obj[contractInputStr]);
            cout << "Object " << contractInputStr << " :" << print_json(inputs.contract_input) << std::endl;
        }

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
