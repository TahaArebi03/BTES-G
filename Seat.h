#pragma once
// Seat.h
#pragma once
#include <string>

class Seat {
private:
    int seatId;               // معرف المقعد
    bool isAvailable;         // حالة المقعد: متاح أم لا
    std::string contactInfo;  // معلومات اتصال صاحب المقعد
    double price;             // سعر المقعد

public:
    // Constructor
    Seat(int id, double price, const std::string& contact)
        : seatId(id) , price(price), contactInfo(contact), isAvailable(true) {}

    // Check if seat is available
    bool isSeatAvailable() const { return isAvailable; }

    // Reserve the seat
    void reserveSeat() { isAvailable = false; }

    // Release the seat
    void releaseSeat() { isAvailable = true; }

    // Get contact info
    std::string getContactInfo() const { return contactInfo; }

    // Get seat price
    double getPrice() const { return price; }
};





