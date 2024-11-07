// Seats.h
#pragma once
#include "Seat.h"
#include <vector>

class Seats {
private:
    std::vector<Seat> seatList; // قائمة المقاعد

public:
    // Constructor to initialize seats with prices
    Seats(int numberOfSeats, double price) {
        for (int i = 1; i <= numberOfSeats; ++i) {
            seatList.emplace_back(i, price, "");  // إنشاء مقاعد بدون معلومات اتصال حالياً
        }
    }

    // Add a seat to the list
    void addSeat(const Seat& seat) { seatList.push_back(seat); }

    // Reserve a seat by ID
    bool reserveSeat(int seatId, const std::string& contactInfo) {
        if (seatId >= 1 && seatId <= seatList.size() && seatList[seatId - 1].isSeatAvailable()) {
            seatList[seatId - 1].reserveSeat();
            seatList[seatId - 1].getContactInfo() = contactInfo; // تحديث معلومات الاتصال
            return true;
        }
        return false;
    }

    // Check if all seats are reserved
    bool areAllSeatsReserved() const {
        for (const auto& seat : seatList) {
            if (seat.isSeatAvailable()) return false;
        }
        return true;
    }

    // Get count of reserved seats
    int getReservedSeatsCount() const {
        int count = 0;
        for (const auto& seat : seatList) {
            if (!seat.isSeatAvailable()) count++;
        }
        return count;
    }

    // Check seat availability by seat ID
    bool isSeatAvailable(int seatId) const {
        return seatList[seatId - 1].isSeatAvailable();
    }

    // Get available seat count
    int getAvailableSeatCount() const {
        int count = 0;
        for (const auto& seat : seatList) {
            if (seat.isSeatAvailable()) count++;
        }
        return count;
    }
};
