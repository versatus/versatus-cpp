
#include "../src/versatus_cpp.hpp"
#include "../src/versatus_cpp_erc20.hpp"

int main() {

    process_erc20();

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