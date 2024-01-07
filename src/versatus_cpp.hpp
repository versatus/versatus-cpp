#ifndef VERSATUS_CPP_HPP
#define VERSATUS_CPP_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <cassert>

#include <stdexcept>

using namespace std;
using json = nlohmann::json;

// Assumption: Eth Address is 20 bytes
constexpr std::size_t ADDRESS_SIZE = 20;
using Address = std::array<uint8_t, ADDRESS_SIZE>;
using uint256_t = boost::multiprecision::uint256_t;
const int CUSTOM_INDENT_SPACES = 2;

// Generic function to print an object to JSON string
template <typename T>
std::string print_json(const T& obj) {
    json j;
    obj.to_json(j);
    return j.dump(CUSTOM_INDENT_SPACES);
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

class Input {
public:
    virtual ~Input() = default;

    virtual void to_json(json& j) const {
        // Base class implementation (empty)
    }

    virtual void from_json(const json& j) {
        // Base class implementation (empty)
    }

    std::string print_json() const {
        json j;
        to_json(j);
        return j.dump(CUSTOM_INDENT_SPACES);
    }
};

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
        return j.dump(CUSTOM_INDENT_SPACES);
    }
};

class NameInput : public Input {
public:
    std::string value;

    void to_json(json& j) const override {
        if (!value.empty())
            j["value"] = value;
    }

    void from_json(const json& j) override {
        j.at("value").get_to(value);
    }
};

class SymbolInput : public Input {
public:
    std::string value;

    void to_json(json& j) const override {
        if (!value.empty())
            j["value"] = value;
    }

    void from_json(const json& j) override {
        j.at("value").get_to(value);
    }
};

class DecimalsInput : public Input {
public:
    uint8_t value;

    void to_json(json& j) const override {
        j = json{{"value", value}};
    }

    void from_json(const json& j) override {
        j.at("value").get_to(value);
    }
};

class TotalSupplyInput : public Input {
public:
    uint256_t value;

    void to_json(json& j) const override {
        j = json{{"value", value.str()}};
    }

    void from_json(const json& j) override {
        std::string value_str = j.at("value");
        value = boost::multiprecision::uint256_t(value_str);
    }
};

class BalanceOfInput : public Input {
public:
    Address account;

    void to_json(json& j) const override {
        j = json{{"address", addressToString(account)}};
    }

    void from_json(const json& j) override {
        std::string account_str = j.at("address");
        account = convertStringToAddress(account_str);
    }

};

class AllowanceInput : public Input {
public:
    Address owner;
    Address spender;

    // Custom serialization function for AllowanceInput
    void to_json(json& j) const override {
        j = json{{"owner", addressToString(owner)}, {"spender", addressToString(spender)}};
    }

    // Custom deserialization function for AllowanceInput
    void from_json(const json& j) override {
        std::string owner_str = j.at("owner");
        owner = convertStringToAddress(owner_str);
        std::string spender_str = j.at("spender");
        spender = convertStringToAddress(spender_str);
    }

};

class TransferInput : public Input {
public:
    Address address;
    uint256_t value;

    // Custom serialization function for TransferInput
    void to_json(json& j) const override {
        j = json{{"address", addressToString(address)}, {"value", "0x" + uint256_to_hex(value)}};
    }

    // Custom deserialization function for TransferInput
    void from_json(const json& j) override {
        std::string address_str = j.at("address");
        address = convertStringToAddress(address_str);
        std::string value_str = j.at("value");
        value = boost::multiprecision::uint256_t(value_str);
    }

};

class TransferFromInput : public Input {
public:
    Address from;
    Address to;
    uint256_t value;

    // Custom serialization function for TransferFromInput
    void to_json(json& j) const override {
        j = json{{"from", addressToString(from)}, {"to", addressToString(to)}, {"value", "0x" + uint256_to_hex(value)}};
    }

    // Custom deserialization function for TransferFromInput
    void from_json(const json& j) override {
        std::string from_str = j.at("from");
        from = convertStringToAddress(from_str);
        std::string to_str = j.at("to");
        to = convertStringToAddress(to_str);
        std::string value_str = j.at("value");
        value = boost::multiprecision::uint256_t(value_str);
    }

};

class ApproveInput : public Input {
public:
    Address address;
    uint256_t value;

    // Custom serialization function for ApproveInput
    void to_json(json& j) const override {
        j = json{{"address", addressToString(address)}, {"value", "0x" + uint256_to_hex(value)}};
    }

    // Custom deserialization function for ApproveInput
    void from_json(const json& j) override {
        std::string address_str = j.at("address");
        address = convertStringToAddress(address_str);
        std::string value_str = j.at("value");
        value = boost::multiprecision::uint256_t(value_str);
    }

};


class ERC20Inputs : public Input {
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
    void to_json(json& j) const override {
        if (!name.value.empty()) name.to_json(j);
        if (!symbol.value.empty()) symbol.to_json(j);
        if (decimals.value) decimals.to_json(j);
        if (total_supply.value) total_supply.to_json(j);
        if (balance_of.account != Address{}) balance_of.to_json(j);
        if (transfer.address != Address{}) {
            json transferJson;
            transfer.to_json(transferJson);
            j["transfer"] = transferJson;
        }
        if (transfer_from.from != Address{} && transfer_from.to != Address{} && transfer_from.value != 0) {
            json transferFromJson;
            transfer_from.to_json(transferFromJson);
            j["transferFrom"] = transferFromJson;
        }
        if (approve.address != Address{} && approve.value != 0) {
            json approveJson;
            approve.to_json(approveJson);
            j["approve"] = approveJson;
        }
        if (allowance.owner != Address{} && allowance.spender != Address{}) {
            json allowanceJson;
            allowance.to_json(allowanceJson);
            j["allowance"] = allowanceJson;
        }

    }

    // Custom deserialization function for ERC20Inputs
    void from_json(const json& j) override {
        if (j.find("name") != j.end()) name.from_json(j.at("name"));
        if (j.find("symbol") != j.end()) symbol.from_json(j.at("symbol"));
        if (j.find("decimals") != j.end()) decimals.from_json(j.at("decimals"));
        if (j.find("totalSupply") != j.end()) total_supply.from_json(j.at("totalSupply"));

        if (j.find("balanceOf") != j.end()) balance_of.from_json(j.at("balanceOf"));
        if (j.find("transfer") != j.end()) {
            transfer.from_json(j.at("transfer"));
        }
        if (j.find("transferFrom") != j.end()) {
            transfer_from.from_json(j.at("transferFrom"));
        }
        if (j.find("approve") != j.end()) approve.from_json(j.at("approve"));
        if (j.find("allowance") != j.end()) allowance.from_json(j.at("allowance"));
    }

};

class FunctionInputs : public Input {
public:
    ERC20Inputs erc20;

    // Custom serialization function for FunctionInputs
    void to_json(json& j) const override {
        json erc20Json;
        erc20.to_json(erc20Json);
        j["erc20"] = erc20Json;
    }

    // Custom deserialization function for FunctionInputs
    void from_json(const json& j) override {
        if (j.find("erc20") != j.end()) {
            erc20.from_json(j.at("erc20"));
        }
    }

};

class ContractInputs : public Input {
public:
    std::string contract_fn;
    FunctionInputs function_inputs;

    // Custom serialization function for ContractInput
    void to_json(json& j) const override {
        j["contractFn"] = contract_fn;

        json functionInputsJson;
        function_inputs.to_json(functionInputsJson);
        j["functionInputs"] = functionInputsJson;
    }

    // Custom deserialization function for ContractInput
    void from_json(const json& j) override {
        j.at("contractFn").get_to(contract_fn);
        function_inputs.from_json(j.at("functionInputs"));
    }

};

struct Erc20TransferEvent {
    Address from;
    Address to;
    uint256_t value;

    Erc20TransferEvent& operator=(const Erc20TransferEvent& other) {
        if (this != &other) {
            from = other.from;
            to = other.to;
            value = other.value;
        }
        return *this;
    }
};

struct Erc20ApprovalEvent {
    Address owner;
    Address spender;
    uint256_t value;

    Erc20ApprovalEvent& operator=(const Erc20ApprovalEvent& other) {
        if (this != &other) {
            owner = other.owner;
            spender = other.spender;
            value = other.value;
        }
        return *this;
    }
};

#include <variant>

class Erc20Result {
public:
    enum class Erc20ResultType {
        EnumName,
        EnumSymbol,
        EnumDecimals,
        EnumTotalSupply,
        EnumBalanceOf,
        EnumTransfer,
        EnumTransferFrom,
        EnumApprove,
        EnumAllowance,
        EnumUnknown
    };

    union Erc20ResultValue {
        std::string name;
        std::string symbol;
        uint8_t decimals;
        uint256_t totalSupply;
        uint256_t balance;
        Erc20TransferEvent transfer;
        Erc20TransferEvent transferFrom;
        Erc20ApprovalEvent approve;
        bool success;
        uint256_t remaining;

        Erc20ResultValue() {}  // Default constructor for the union
        ~Erc20ResultValue() {} // Destructor for the union

    };

    // Constructors for convenience
    Erc20Result() : type(Erc20ResultType::EnumUnknown), value{} {}

    // Getter for type
    Erc20ResultType getType() const {
        return type;
    }

    // Setter for type
    void setType(Erc20ResultType newType) {
        type = newType;
    }

    // Getter for value
    const Erc20ResultValue& getValue() const {
        return value;
    }

    Erc20Result &operator=(const Erc20Result &other)
    {
        // Handle copy assignment for value based on the type
        switch (other.type) {
        case Erc20ResultType::EnumName:
            this->value.name = other.value.name;
            break;
        case Erc20ResultType::EnumSymbol:
            this->value.symbol = other.value.symbol;
            break;
        case Erc20ResultType::EnumDecimals:
            this->value.decimals = other.value.decimals;
            break;
        case Erc20ResultType::EnumTotalSupply:
            this->value.totalSupply = other.value.totalSupply;
            break;
        case Erc20ResultType::EnumBalanceOf:
            this->value.balance = other.value.balance;
            break;
        case Erc20ResultType::EnumTransfer:
        case Erc20ResultType::EnumTransferFrom:
            this->value.success = other.value.success;
            break;
        case Erc20ResultType::EnumApprove:
            this->value.approve = other.value.approve;
            break;
        case Erc20ResultType::EnumAllowance:
            this->value.remaining = other.value.remaining;
            break;
        default:
            break;
        }
        return *this;
    }

    Erc20ResultType type;
    Erc20ResultValue value;
};

class Erc721Result {

};

class ContractResult {
public:
    using ResultType = std::variant<Erc20Result, Erc721Result>;

    ContractResult() : result(std::in_place_type<Erc20Result>) {}

    // Getter for result
    const ResultType& getResult() const {
        return result;
    }

    void setType(Erc20Result::Erc20ResultType newType) {
        std::get<Erc20Result>(result).type = newType;
    }

    // Setter for result
    template <typename T>
    void setResult(const T& newResult) {
        result = newResult;
    }

    ResultType result;
};


class ContractOutputs {
    const char* const contractOutputStr = "contractOutput";

public:
    ContractResult result;

    json to_json() const {
        json j;
        const auto& contract_result = result;
        json result_json;

        // Use std::visit to handle the variant type
        std::visit([&result_json](const auto& resultType) {
            using ResultType = std::decay_t<decltype(resultType)>;
            if constexpr (std::is_same_v<ResultType, Erc20Result>) {
                switch (resultType.getType()) {

                    case Erc20Result::Erc20ResultType::EnumName:
                        result_json["type"] = "EnumName";
                        result_json["value"] = resultType.value.name;
                        break;

                    case Erc20Result::Erc20ResultType::EnumSymbol:
                        result_json["type"] = "EnumSymbol";
                        result_json["value"] = resultType.value.symbol;
                        break;

                    case Erc20Result::Erc20ResultType::EnumDecimals:
                        result_json["type"] = "EnumDecimals";
                        result_json["value"] = resultType.value.decimals;
                        break;

                    case Erc20Result::Erc20ResultType::EnumTotalSupply:
                        result_json["type"] = "EnumTotalSupply";
                        result_json["value"] = resultType.value.totalSupply.str();
                        break;

                    case Erc20Result::Erc20ResultType::EnumBalanceOf:
                        result_json["type"] = "EnumBalanceOf";
                        result_json["value"] = resultType.value.balance.str();
                        break;

                    case Erc20Result::Erc20ResultType::EnumTransfer:
                        result_json["type"] = "EnumTransfer";
                        result_json["value"] = std::to_string(resultType.value.success);
                        break;

                    case Erc20Result::Erc20ResultType::EnumTransferFrom:
                        result_json["type"] = "EnumTransferFrom";
                        result_json["value"] = std::to_string(resultType.value.success);
                        break;

                    case Erc20Result::Erc20ResultType::EnumApprove:
                        result_json["type"] = "EnumApprove";
                        result_json["value"] = std::to_string(resultType.value.success);
                        break;

                    default:
                        // Handle unknown result type
                        result_json["type"] = "Unknown";
                        result_json["value"] = "N/A";
                        break;
                }
            } else if constexpr (std::is_same_v<ResultType, Erc721Result>) {
                    // Handle Erc721Result
                    result_json["type"] = "Erc721";
                    // Access Erc721Result fields using result.erc721Result
            }
        }, contract_result.getResult());
        j["results"].push_back(result_json);

        return j;
    }

    void commit() const {
        cout << contractOutputStr << " :" << to_json().dump(CUSTOM_INDENT_SPACES) << std::endl;
    }
};

class ProtocolInputs : public Input {
    const char* const versionStr = "version";
    const char* const blockHeightStr = "blockHeight";
    const char* const blockTimeStr = "blockTime";
public:
    int32_t version;
    uint64_t block_height;
    uint64_t block_time;

    // Custom serialization function for ProtocolInputs
    void to_json(json& j) const override {
        j = json{{versionStr, version}, {blockHeightStr, block_height}, {blockTimeStr, block_time}};
    }

    // Custom deserialization function for ProtocolInputs
    void from_json(const json& j) override {
        if (j.find(versionStr) != j.end()) j.at(versionStr).get_to(version);
        if (j.find(blockHeightStr) != j.end()) j.at(blockHeightStr).get_to(block_height);
        if (j.find(blockTimeStr) != j.end()) j.at(blockTimeStr).get_to(block_time);
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
        return j.dump(CUSTOM_INDENT_SPACES);
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

        if (json_obj.contains(accountInfoStr)) {
            inputs.account_info.from_json(json_obj[accountInfoStr]);
        }

        if (json_obj.contains(protocolInputStr)) {
            inputs.protocol_input.from_json(json_obj[protocolInputStr]);
        }

        if (json_obj.contains(applicationInputStr)) {
            inputs.application_input.from_json(json_obj[applicationInputStr]);
            cout << "Object " << applicationInputStr << " :" << print_json(inputs.application_input) << std::endl;
        }
        
        if (json_obj.contains(contractInputStr)) {
            inputs.contract_input.from_json(json_obj[contractInputStr]);
            cout << contractInputStr << " :" << print_json(inputs.contract_input) << std::endl;
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
