#ifndef PAYMENT_H
#define PAYMENT_H

#include <iostream>
#include <string>

using namespace std;

// --- Base Class: Payment ---
class Payment {
public:
    virtual void pay(double amount) = 0; // œ«·… œ›⁄ „Ã—œ…
};

// --- CreditCard Class ---
class CreditCard : public Payment {
public:
    void pay(double amount) override;
};

// --- PayPal Class ---
class PayPal : public Payment {
public:
    void pay(double amount) override;
};

// --- BankTransfer Class ---
class BankTransfer : public Payment {
public:
    void pay(double amount) override;
};

// ﬂ·«”« 

#endif // PAYMENT_H
