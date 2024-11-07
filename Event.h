// Event.h
#pragma once
#include "Seats.h"
#include "Notification.h"
#include <fstream>
#include <string>

class Event {
private:
    int eventId;                     // ???? ?????
    std::string eventName;           // ??? ?????
    std::string eventDate;           // ????? ?????
    bool isCancelled;                // ???? ????? (????/????)
    Seats seats;                     // ???? ???????
    Notification notification;       // ???? ?????????

public:
    // Constructor
    Event(int id, const std::string& name, const std::string& date, int numberOfSeats, double seatPrice)
        : eventId(id), eventName(name), eventDate(date), isCancelled(false), seats(numberOfSeats, seatPrice) {}

    // Add a new event
    void addEvent(const std::string& name, const std::string& date) {
        eventName = name;
        eventDate = date;
        isCancelled = false;
        std::cout << "Event added successfully.\n";
    }

    // Cancel event and notify seat owners
    void cancelEvent() {
        if (!isCancelled) {
            isCancelled = true;
            std::string message = "The event '" + eventName + "' has been cancelled.";
            for (int i = 1; i <= seats.getReservedSeatsCount(); ++i) {
                std::string contact = "contact" + std::to_string(i);
                notification.sendNotification(contact, message);
            }
            std::cout << "Event cancelled. Notifications sent.\n";
        }
    }

    // Modify event details
    void modifyEvent(const std::string& newName, const std::string& newDate) {
        eventName = newName;
        eventDate = newDate;
        std::cout << "Event details modified successfully.\n";
    }

    // Check if event is cancelled
    bool isEventCancelled() const {
        return isCancelled;
    }

    // Check if event is full
    bool isEventFull() const {
        return seats.areAllSeatsReserved();
    }

    // Save event data to a file
    void saveEventData() const {
        std::ofstream file("event_data.txt", std::ios::app);
        if (file.is_open()) {
            file << "Event ID: " << eventId << "\n"
                << "Event Name: " << eventName << "\n"
                << "Event Date: " << eventDate << "\n"
                << "Status: " << (isCancelled ? "Cancelled" : "Active") << "\n";
            file.close();
            std::cout << "Event data saved to file.\n";
        }
        else {
            std::cerr << "Unable to open file.\n";
        }
    }

    // Display event statistics
    void displayEventStatistics() const {
        std::cout << "Event Statistics:\n";
        std::cout << "Event ID: " << eventId << "\n";
        std::cout << "Event Name: " << eventName << "\n";
        std::cout << "Event Date: " << eventDate << "\n";
        std::cout << "Status: " << (isCancelled ? "Cancelled" : "Active") << "\n";
        std::cout << "Reserved Seats: " << seats.getReservedSeatsCount() << "\n";
    }

    // Check event status
    std::string checkEventStatus() const {
        return isCancelled ? "Cancelled" : "Available";
    }
};
