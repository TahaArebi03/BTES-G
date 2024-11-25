#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "testpayment.h" // ���� �� ����� ��� ����� �������

using namespace std;

// --- Base Class: Payment ---
void Payment::pay(double amount) {
    // ���� ��� ����ɡ ��� �� ��� ������� �� ������ �������
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


// ���� ���� �� �������� �����
