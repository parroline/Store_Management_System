#include "Store.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int main(){
    Store store;

    int choice;

    do{
        cout << string(50, '=') << endl;
        cout << right << setw(33) << "STORE MANAGEMENT" << endl;
        cout << string(50, '-') << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Update Product" << endl;
        cout << "3. Delete Product" << endl;
        cout << "4. Search Product" << endl;
        cout << "5. Show Products" << endl << endl;
        cout << "6. Sell Product" << endl << endl;
        cout << "7. Show Bills" << endl << endl;
        cout << "8. Statistics" << endl << endl;
        cout << "9. Clear All Products" << endl;
        cout << "0. Exit" << endl;
        cout << string(50, '-') << endl;
        
        cout << "Choose: ";
        cin >> choice;
        int id;
        cout << endl;
        switch (choice){
            case 1:
                store.addProduct();
                break;
            case 2:
                cout << "ID: ";
                cin >> id;

                store.updateProduct(id);
                break;
            case 3:
                cout << "ID: ";
                cin >> id;

                store.deleteProduct(id);
                break;
            case 4:
                cout << "ID: ";
                cin >> id;

                store.searchProduct(id);
                break;
            case 5:
                cout << endl;
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
                cout << "Exited." << endl;
                break;
            default:
                cout << "Invalid Choice." << endl;
        }
        cout << string(50, '=') << endl << endl;
    } while(choice != 0);
}