#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <vector>
#include "Store.h"
#include "Bill.h"

namespace fs = std::filesystem;

Store::Store(){
    loadFromFile();
}

Product* Store::findProduct(int id){
    for(Product &p : products){
        if(p.id == id){
            return &p;
        }
    }
    return nullptr;
}

void Store::addProduct(){
    Product p;
    
    p.id = nextProductID;
    nextProductID++;

    std::cout << "Name: ";
    std::getline(std::cin >> std::ws, p.name);

    std::cout << "Price: ";
    std::cin >> p.price;

    std::cout << "Quantity: ";
    std::cin >> p.quantity;

    products.push_back(p);
    saveToFile();
}

void Store::showProducts(){
    if (products.empty()){
        std::cout << "Products Empty." << std::endl;
        return;
    }

    std::cout << std::string(50, '=') << std::endl;
    std::cout << std::right << std::setw(29) << "PRODUCTS" << std::endl;
    std::cout << std::string(50, '-') << std::endl << std::endl;
    
    std::cout << std::left
         << std::setw(10) << "ID"
         << std::setw(20) << "Name"
         << std::setw(10) << "Price"
         << std::setw(10) << "Quantity" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    for(const Product &p : products){
        std::cout << std::left
         << std::setw(10) << p.id
         << std::setw(20) << p.name
         << std::setw(10) << p.price
         << std::setw(10) << p.quantity << std::endl;
    }
}

void Store::updateProduct(int id){
    Product* p = findProduct(id);
    if(p == nullptr){
        std::cout << "Product Not Found." << std::endl;
        return;
    }

    std::cout << "New Name: ";
    std::getline(std::cin >> std::ws, p->name);

    std::cout << "New Price: ";
    std::cin >> p->price;

    std::cout << "New Quantity: ";
    std::cin >> p->quantity;

    saveToFile();
}

void Store::searchProduct(int id){
    Product* p = findProduct(id);

    if (p == nullptr){
        std::cout << "Product Not Found." << std::endl;
        return;
    }
    std::cout << "Name: " << p->name << std::endl;
    std::cout << "Price: " << p->price << std::endl;
    std::cout << "Quantity: " << p->quantity << std::endl;
}

void Store::deleteProduct(int id){
    for(auto it = products.begin(); it != products.end(); it++){
        if(it->id == id){
            products.erase(it);
            std::cout << "Product Deleted." << std::endl;
            saveToFile();
            return;
        }
    }
    std::cout << "Product Not Found." << std::endl;
}

void Store::saveToFile(){
    std::ofstream file("products.txt");

    if (!file){
        std::cout << "Cannot open file." << std::endl;
        return;
    }
    for(const Product &p : products){
        file << p.id << "|" << p.name << "|" << p.price << "|" << p.quantity << std::endl;
    }

    file.close();
}

void Store::loadFromFile(){
    products.clear();
    nextProductID=1;
    
    std::ifstream file("products.txt");

    if (!file){
        return;
    }

    std::string line;
    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string temp;

        Product p;

        getline(ss, temp, '|');
        p.id = stoi(temp);

        getline(ss, temp, '|');
        p.name = temp;

        getline(ss, temp, '|');
        p.price = stod(temp);

        getline(ss, temp);
        p.quantity = stoi(temp);

        products.push_back(p);

        if(p.id >= nextProductID){
            nextProductID = p.id + 1;
        }
    }
    file.close();
}

void Store::clearProducts()
{
    products.clear();
    nextProductID = 1;

    saveToFile();
    std::cout << "Products cleared." << std::endl;
}

void Store::showBill(const Bill &bill){
    std::cout << std::string(50, '=') << std::endl;
    std::cout << std::right << std::setw(29) << "RECEIPT" << std::endl;
    std::cout << std::string(50, '-') << std::endl << std::endl;
    
    std::cout << "Bill ID: " << bill.billID << std::endl << std::endl;

    std::cout << std::left
         << std::setw(20) << "Product"
         << std::setw(10) << "Quantity"
         << std::setw(10) << "Price"
         << std::setw(10) << "Total" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    for(const BillItem &item : bill.items){
        std::cout << std::left
             << std::setw(20) << item.productName
             << std::setw(10) << item.quantity
             << std::setw(10) << item.unitPrice
             << std::setw(10) << item.total << std::endl;
    }
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Total:" << std::right << std::setw(39) << bill.grandTotal << std::endl;
}

void Store::saveBill(const Bill &bill){
    fs::create_directories("Bills");
    std::string filename = "Bills/bill_" + std::to_string(bill.billID) + ".txt";
    std::ofstream file(filename);

    if(!file){
        std::cout << "Cannot Create Bill File." << std::endl;
        return;
    }

    file << "BillID|" << bill.billID << std::endl;
    
    for(const BillItem &item : bill.items){
        file << item.productID << "|"
             << item.productName << "|"
             << item.quantity << "|"
             << item.unitPrice << "|"
             << item.total << std::endl;
    }
    file << "Total|" << bill.grandTotal;
    
    file.close();
}

void Store::sellProduct(){
    if(products.empty()){
        std::cout << "No Products Available." << std::endl;
        return;
    }
    Bill bill;
    bill.billID = nextBillID++;
    bill.grandTotal = 0;
    char cont = 'Y';
    do{
        int id;
        std::cout << "ID: ";
        std::cin >> id;
    
        Product* p = findProduct(id);
        if(p == nullptr){
            std::cout << "Product Not Found." << std::endl;
            continue;
        }

        int qt;
        std::cout << "Quantity: ";
        std::cin >> qt;
        if(qt > p->quantity){
            std::cout << "Not Enough Quantity." << std::endl;
            continue;
        }
        else if(qt <= 0){
            std::cout << "Quantity Invalid." << std::endl;
            continue;
        }
        p->quantity -= qt;

        BillItem item;
        item.productID = p->id;
        item.productName = p->name;
        item.unitPrice = p->price;
        item.quantity = qt;
        item.total = qt * p->price;

        bill.items.push_back(item);
        bill.grandTotal += item.total;

        std::cout << "Continue? (Y/N): ";
        std::cin >> cont;
        
        if(cont == 'n' || cont == 'N') break;
        else if(cont != 'y' && cont != 'Y'){
            std::cout << "Invalid Value." << std::endl;
            continue;
        }
    }while(cont == 'y' || cont == 'Y');
    
    if(bill.items.empty()){
        std::cout << "No Products Sold." << std::endl;
        return;
    }
    saveToFile();
    saveBill(bill);
    std::cout << "Sold Successfully." << std::endl;
    showBill(bill);
}

Bill Store::loadBill(const std::string &filename){
    std::ifstream file(filename);

    Bill bill;

    if(!file){
        std::cout << "Cannot Open Bill." << std::endl;
        return bill;
    }

    std::string line;

    while(getline(file, line)){
        std::stringstream ss(line);
        std::string temp;

        if(line.starts_with("BillID")){
            getline(ss, temp, '|');
            getline(ss, temp);      
            bill.billID = stoi(temp);
            continue;
        }
        else if(line.starts_with("Total")){
            getline(ss, temp, '|');
            getline(ss, temp);      
            bill.grandTotal = stod(temp);
            continue;
        }
        else{
            BillItem item;

            getline(ss, temp, '|');
            item.productID = stoi(temp);

            getline(ss, temp, '|');
            item.productName = temp;

            getline(ss, temp, '|');
            item.quantity = stoi(temp);

            getline(ss, temp, '|');
            item.unitPrice = stod(temp);

            getline(ss, temp);
            item.total = stod(temp);

            bill.items.push_back(item);
        }
    }

    file.close();
    return bill;
}

void Store::showAllBills(){
    if(!fs::exists("Bills")){
        std::cout << "No Bills Found." << std::endl;
        return;
    }

    for(const auto &entry : fs::directory_iterator("Bills")){
        Bill bill = loadBill(entry.path().string());

        showBill(bill);
        std::cout << std::endl;
    }
}

void Store::statistics(){
    if(products.empty()){
        std::cout << "No Products." << std::endl;
        return;
    }

    int totalQuantity = 0;
    double inventoryValue = 0;
    Product *mostExpensive = &products[0];
    Product *cheapest = &products[0];

    for(Product &p : products){
        totalQuantity += p.quantity;
        inventoryValue += p.price * p.quantity;
        if(p.price > mostExpensive->price){
            mostExpensive = &p;
        }
        if(p.price < cheapest->price){
            cheapest = &p;
        }
    }

    std::cout << std::string(50, '=') << std::endl;
    std::cout << std::right << std::setw(30) << "STATISTICS" << std::endl;
    std::cout << std::string(50, '-') << std::endl << std::endl;

    std::cout << "Products:" << std::right << std::setw(36) << products.size() << std::endl;
    std::cout << "Inventory Items:" << std::right << std::setw(29) << totalQuantity << std::endl;
    std::cout << "Inventory Value:" << std::right << std::setw(29) << inventoryValue << std::endl;
    std::cout << "Most Expensive:" << std::right << std::setw(30) << mostExpensive->name << std::endl;
    std::cout << "Cheapest:" << std::right << std::setw(36) << cheapest->name << std::endl;
}