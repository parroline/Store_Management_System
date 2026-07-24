#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <vector>
#include "Store.h"
#include "Bill.h"

using namespace std;
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

    cout << "Name: ";
    getline(cin >> ws, p.name);

    cout << "Price: ";
    cin >> p.price;

    cout << "Quantity: ";
    cin >> p.quantity;

    products.push_back(p);
    saveToFile();
}

void Store::showProducts(){
    if (products.empty()){
        cout << "Products Empty." << endl;
        return;
    }

    cout << string(50, '=') << endl;
    cout << right << setw(29) << "PRODUCTS" << endl;
    cout << string(50, '-') << endl << endl;
    
    cout << left
         << setw(10) << "ID"
         << setw(20) << "Name"
         << setw(10) << "Price"
         << setw(10) << "Quantity" << endl;
    cout << string(50, '-') << endl;

    for(const Product &p : products){
        cout << left
         << setw(10) << p.id
         << setw(20) << p.name
         << setw(10) << p.price
         << setw(10) << p.quantity << endl;
    }
}

void Store::updateProduct(int id){
    Product* p = findProduct(id);
    if(p == nullptr){
        cout << "Product Not Found." << endl;
        return;
    }

    cout << "New Name: ";
    getline(cin >> ws, p->name);

    cout << "New Price: ";
    cin >> p->price;

    cout << "New Quantity: ";
    cin >> p->quantity;

    saveToFile();
}

void Store::searchProduct(int id){
    Product* p = findProduct(id);

    if (p == nullptr){
        cout << "Product Not Found." << endl;
        return;
    }
    cout << "Name: " << p->name << endl;
    cout << "Price: " << p->price << endl;
    cout << "Quantity: " << p->quantity << endl;
}

void Store::deleteProduct(int id){
    for(auto it = products.begin(); it != products.end(); it++){
        if(it->id == id){
            products.erase(it);
            cout << "Product Deleted." << endl;
            saveToFile();
            return;
        }
    }
    cout << "Product Not Found." << endl;
}

void Store::saveToFile(){
    ofstream file("products.txt");

    if (!file){
        cout << "Cannot open file." << endl;
        return;
    }
    for(const Product &p : products){
        file << p.id << "|" << p.name << "|" << p.price << "|" << p.quantity << endl;
    }

    file.close();
}

void Store::loadFromFile(){
    products.clear();
    nextProductID=1;
    
    ifstream file("products.txt");

    if (!file){
        return;
    }

    string line;
    while(getline(file, line)){
        stringstream ss(line);
        string temp;

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
    cout << "Products cleared." << endl;
}

void Store::showBill(const Bill &bill){
    cout << string(50, '=') << endl;
    cout << right << setw(29) << "RECEIPT" << endl;
    cout << string(50, '-') << endl << endl;
    
    cout << "Bill ID: " << bill.billID << endl << endl;

    cout << left
         << setw(20) << "Product"
         << setw(10) << "Quantity"
         << setw(10) << "Price"
         << setw(10) << "Total" << endl;
    cout << string(50, '-') << endl;
    for(const BillItem &item : bill.items){
        cout << left
             << setw(20) << item.productName
             << setw(10) << item.quantity
             << setw(10) << item.unitPrice
             << setw(10) << item.total << endl;
    }
    cout << string(50, '-') << endl;
    cout << "Total:" << right << setw(39) << bill.grandTotal << endl;
}

void Store::saveBill(const Bill &bill){
    fs::create_directories("Bills");
    string filename = "Bills/bill_" + to_string(bill.billID) + ".txt";
    ofstream file(filename);

    if(!file){
        cout << "Cannot Create Bill File." << endl;
        return;
    }

    file << "BillID|" << bill.billID << endl;
    
    for(const BillItem &item : bill.items){
        file << item.productID << "|"
             << item.productName << "|"
             << item.quantity << "|"
             << item.unitPrice << "|"
             << item.total << endl;
    }
    file << "Total|" << bill.grandTotal;
    
    file.close();
}

void Store::sellProduct(){
    if(products.empty()){
        cout << "No Products Available." << endl;
        return;
    }
    Bill bill;
    bill.billID = nextBillID++;
    bill.grandTotal = 0;
    char cont = 'Y';
    do{
        int id;
        cout << "ID: ";
        cin >> id;
    
        Product* p = findProduct(id);
        if(p == nullptr){
            cout << "Product Not Found." << endl;
            continue;
        }

        int qt;
        cout << "Quantity: ";
        cin >> qt;
        if(qt > p->quantity){
            cout << "Not Enough Quantity." << endl;
            continue;
        }
        else if(qt <= 0){
            cout << "Quantity Invalid." << endl;
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

        cout << "Continue? (Y/N): ";
        cin >> cont;
        
        if(cont == 'n' || cont == 'N') break;
        else if(cont != 'y' && cont != 'Y'){
            cout << "Invalid Value." << endl;
            continue;
        }
    }while(cont == 'y' || cont == 'Y');
    
    if(bill.items.empty()){
        cout << "No Products Sold." << endl;
        return;
    }
    saveToFile();
    saveBill(bill);
    cout << "Sold Successfully." << endl;
    showBill(bill);
}

Bill Store::loadBill(const string &filename){
    ifstream file(filename);

    Bill bill;

    if(!file){
        cout << "Cannot Open Bill." << endl;
        return bill;
    }

    string line;

    while(getline(file, line)){
        stringstream ss(line);
        string temp;

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
        cout << "No Bills Found." << endl;
        return;
    }

    for(const auto &entry : fs::directory_iterator("Bills")){
        Bill bill = loadBill(entry.path().string());

        showBill(bill);
        cout << endl;
    }
}

void Store::statistics(){
    if(products.empty()){
        cout << "No Products." << endl;
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

    cout << string(50, '=') << endl;
    cout << right << setw(30) << "STATISTICS" << endl;
    cout << string(50, '-') << endl << endl;

    cout << "Products:" << right << setw(36) << products.size() << endl;
    cout << "Inventory Items:" << right << setw(29) << totalQuantity << endl;
    cout << "Inventory Value:" << right << setw(29) << inventoryValue << endl;
    cout << "Most Expensive:" << right << setw(30) << mostExpensive->name << endl;
    cout << "Cheapest:" << right << setw(36) << cheapest->name << endl;
}