#ifndef BILL_H
#define BILL_H

#include <string>
#include <vector>

struct BillItem{
    int productID;
    std::string productName;
    int quantity;
    double unitPrice;

    double total;
};

struct Bill{
    int billID;
    std::vector<BillItem> items;
    double grandTotal;
};

#endif