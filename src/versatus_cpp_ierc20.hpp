#include <string>
#include <array>

#include "./versatus_cpp.hpp"

/*
    Based on https://github.com/OpenZeppelin/openzeppelin-contracts/blob/master/contracts/token/ERC20/IERC20.sol
*/
class IERC20 {
public:
    virtual ~IERC20() = default;

    // Events
    virtual void TransferEvent(const Address &from, const Address &to, uint256_t value) const = 0;
    virtual void ApprovalEvent(const Address &owner, const Address &spender, uint256_t value) const = 0;

    // Getters
    virtual std::string name() const = 0;
    virtual std::string symbol() const = 0;
    virtual uint8_t decimals() const = 0;

    virtual uint256_t totalSupply() const = 0;
    virtual uint256_t balanceOf(const Address &account) const = 0;
    virtual uint256_t allowance(const Address &owner, const Address &spender) const = 0;

    // Actions
    virtual bool transfer(const Address &to, uint256_t value) = 0;
    virtual bool approve(const Address &spender, uint256_t value) = 0;
    virtual bool transferFrom(const Address &from, const Address &to, uint256_t value) = 0;
};
