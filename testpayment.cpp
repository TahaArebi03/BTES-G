#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "testpayment.h" // ÊÃßÏ ãä ÊÖãíä ãáİ ÇáÑÃÓ ÇáãäÇÓÈ

using namespace std;

// --- Base Class: Payment ---
void Payment::pay(double amount) {
    // ÏÇáÉ ÏİÚ ãÌÑÏÉ¡ íÌÈ Ãä íÊã ÊäİíĞåÇ İí ÇáİÆÇÊ ÇáİÑÚíÉ
}

// --- CreditCard Class ---
void CreditCard::pay(double amount) {
    cout << "Paid $" << amount << " using Credit Card." << endl;
}

// --- PayPal Class ---
void PayPal::pay(double amount) {
    cout << "Paid $" << amount << " using PayPal." << endl;
}

// --- BankTransfer Class ---
void BankTransfer::pay(double amount) {
    cout << "Paid $" << amount << " using Bank Transfer." << endl;
}


// ÏæÇá ÇáÊí Êã ÇáÇÎÊÈÇÑ ÚáíåÇ
