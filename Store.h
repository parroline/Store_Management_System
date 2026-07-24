#ifndef STORE_H
#define STORE_H

#include <vector>
#include <string>
#include "Product.h"
#include "Bill.h"

class Store{
    private:
        std::vector<Product> products;
        int nextProductID = 1;

        int nextBillID = 1;

    public:
        Store();
        
        void addProduct();
        void updateProduct(int);
        void showProducts();
        void searchProduct(int);
        void deleteProduct(int);

        void saveToFile();
        void loadFromFile();
        
        void clearProducts();
        Product* findProduct(int);
        
        void sellProduct();

        void showBill(const Bill &bill);
        void saveBill(const Bill &bill);
        Bill loadBill(const std::string &filename);
        void showAllBills();

        void statistics();
};

#endif