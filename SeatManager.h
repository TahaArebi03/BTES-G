#pragma once
#include<Seat.h>
class SeatManager {
private:
    vector<Seat> seats;

public:
    SeatManager(int totalSeats) {
        for (int i = 1; i <= totalSeats; ++i) {
            seats.emplace_back(i);
        }
    }

    int getAvailableSeats() const {
        int availableSeats = 0;
        for (const Seat& seat : seats) {
            if (seat.checkAvailability()) ++availableSeats;
        }
        return availableSeats;
    }

    void reserveSeat(int seatNumber) {
        if (seatNumber < 1 || seatNumber > seats.size()) {
            throw out_of_range("Invalid seat number.");
        }
        seats[seatNumber - 1].reserve();
    }

    void cancelReservation(int seatNumber) {
        if (seatNumber < 1 || seatNumber > seats.size()) {
            throw out_of_range("Invalid seat number.");
        }
        seats[seatNumber - 1].cancelReservation();
    }

    void displaySeats() const {
        cout << "Available Seats:\n";
        for (const Seat& seat : seats) {
            cout << "Seat " << seat.getSeatNumber()
                << " - " << (seat.checkAvailability() ? "Available" : "Reserved") << endl;
        }
    }
};

