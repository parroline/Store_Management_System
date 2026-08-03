#include "Store.h"
#include <iostream>
#include <iomanip>
#include <string>

int main(){
    Store store;

    int choice;

    do{
        std::cout << std::string(50, '=') << std::endl;
        std::cout << std::right << std::setw(33) << "STORE MANAGEMENT" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "1. Add Product" << std::endl;
        std::cout << "2. Update Product" << std::endl;
        std::cout << "3. Delete Product" << std::endl;
        std::cout << "4. Search Product" << std::endl;
        std::cout << "5. Show Products" << std::endl << std::endl;
        std::cout << "6. Sell Product" << std::endl << std::endl;
        std::cout << "7. Show Bills" << std::endl << std::endl;
        std::cout << "8. Statistics" << std::endl << std::endl;
        std::cout << "9. Clear All Products" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        std::cout << "Choose: ";
        std::cin >> choice;
        int id;
        std::cout << std::endl;
        switch (choice){
            case 1:
                store.addProduct();
                break;
            case 2:
                std::cout << "ID: ";
                std::cin >> id;

                store.updateProduct(id);
                break;
            case 3:
                std::cout << "ID: ";
                std::cin >> id;

                store.deleteProduct(id);
                break;
            case 4:
                std::cout << "ID: ";
                std::cin >> id;

                store.searchProduct(id);
                break;
            case 5:
                std::cout << std::endl;
                store.showProducts();
                break;
            case 6:
                store.sellProduct();
                break;
            case 7:
                store.showAllBills();
                break;
            case 8:
                break;
            case 9:
                store.clearProducts();
                break;
            case 0:
                std::cout << "Exited." << std::endl;
                break;
            default:
                std::cout << "Invalid Choice." << std::endl;
        }
        std::cout << std::string(50, '=') << std::endl << std::endl;
    } while(choice != 0);
}