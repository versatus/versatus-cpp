
/* 
    Based on OpenZeppelin 
    https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/ERC20.sol
*/

#include "./versatus_cpp_ierc20.hpp"

class ERC20 : public IERC20 {

private:
    struct AddressHash {
        size_t operator()(const Address& addr) const {
            // A simple hash function for std::array
            size_t result = 0;
            for (const auto& byte : addr) {
                result = (result << 8) ^ byte;
            }
            return result;
        }
    };

    std::string name_;
    std::string symbol_;
    uint8_t decimals_;
    uint256_t totalSupply_;
    std::unordered_map <Address, uint256_t, AddressHash> balances_;
    std::unordered_map <Address, std::unordered_map <Address, uint256_t, AddressHash>, AddressHash> allowances_;

public:

    ERC20(const std::string& name, const std::string& symbol)
        : name_(name), symbol_(symbol), decimals_(18), totalSupply_(0) {
    }


    virtual void TransferEvent(const Address &from, const Address &to, uint256_t value) const override {
        // TO DO
    }
    
    virtual void ApprovalEvent(const Address &owner, const Address &spender, uint256_t value) const override {
        // TO DO
    }

    // ERC20 Token Metadata
    virtual std::string name() const override {
        return name_;
    }

    virtual std::string symbol() const override {
        return symbol_;
    }

    virtual uint8_t decimals() const override {
        return decimals_;
    }

    // ERC20 Token Supply
    virtual uint256_t totalSupply() const override {
        return totalSupply_;
    }
    
    // ERC20 balanceOf
    virtual uint256_t balanceOf(const Address &account) const override {
        auto it = balances_.find(account);
        return (it != balances_.end()) ? it->second : 0;
    }

    // ERC20 Token Transfer
    virtual bool transfer(const Address &to, uint256_t value) override {
        auto owner = _msgSender();
        _transfer(owner, to, value);
        return true;
    }

    // ERC20 Token Allowance
    virtual uint256_t allowance(const Address &owner, const Address &spender) const override {
        auto itOuter = allowances_.find(owner);
        if (itOuter != allowances_.end()) {
            auto itInner = itOuter->second.find(spender);
            if (itInner != itOuter->second.end()) {
                return itInner->second;
            }
        }
        return 0;
    }

    // ERC20 Token Approval
    bool approve(const Address &spender, uint256_t value) override {
        auto owner = _msgSender();
        allowances_[owner][spender] = value;
        ApprovalEvent(owner, spender, value);
        return true;
    }

    // ERC20 Token transferFrom
    bool transferFrom(const Address &from, const Address &to, uint256_t value) override {
        auto owner = _msgSender();
        require(value <= balances_[from], "ERC20: transfer amount exceeds balance");
        require(value <= allowances_[from][owner], "ERC20: transfer amount exceeds allowance");
        balances_[from] -= value;
        balances_[to] += value;
        allowances_[from][owner] -= value;
        return true;
    }

    void mint(const Address &account, uint256_t value) {
        if (account == Address{}) {
            std::cerr << "ERC20: Invalid receiver" << std::endl;
            std::terminate();
        }
        _update(Address{}, account, value);
    }

    void burn(const Address &account, uint256_t value) {
        if (account == Address{}) {
            std::cerr << "ERC20: Invalid sender" << std::endl;
            std::terminate();
        }
        _update(account, Address{}, value);
    }

private:
    // Helper function to check conditions
    void require(bool condition, const std::string& message) {
        if (!condition) {
            std::cerr << message << std::endl;
            std::terminate();
        }
    }
    
    Address _msgSender() const {
        // TO DO
        // Replace this with  logic to determine the sender's address
        Address sender{};
        sender.fill(0xAA);
        return sender;
    }

    bool isZeroAddress(const Address& address) {
        return (address == Address{});
    }

    bool isNonZeroAddress(const Address& address) {
        return !isZeroAddress(address);
    }
    
    void _transfer(const Address &from, const Address &to, uint256_t value) {
        require(isNonZeroAddress(from), "ERC20: ERC20InvalidSender");
        require(isNonZeroAddress(to), "ERC20: ERC20InvalidReceiver");
        _update(from, to, value);
    }

    void _update(Address from, Address to, uint256_t value) {
        if (from == Address{}) {
            // Overflow check required: The rest of the code assumes that totalSupply never overflows
            totalSupply_ += value;
        } else {
            auto itFrom = balances_.find(from);
            if (itFrom == balances_.end()) {
                // Handle insufficient balance error
                std::cerr << "ERC20: Insufficient balance" << std::endl;
                std::terminate();
            }
            if (itFrom->second < value) {
                // Handle insufficient balance error
                std::cerr << "ERC20: Insufficient balance" << std::endl;
                std::terminate();
            }
            // Overflow not possible: value <= fromBalance <= totalSupply.
            itFrom->second -= value;
        }

        if (to == Address{}) {
            // Overflow not possible: value <= totalSupply or value <= fromBalance <= totalSupply.
            totalSupply_ -= value;
        } else {
            // Overflow not possible: balance + value is at most totalSupply, which we know fits into a uint256.
            balances_[to] += value;
        }

        // Emit Transfer event
        TransferEvent(from, to, value);
    }
};
