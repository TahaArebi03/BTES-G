// Notification.h
#pragma once
#include <iostream>
#include <string>

class Notification {
public:
    // Send a notification to the seat owner
    void sendNotification(const std::string& contactInfo, const std::string& message) const {
        std::cout << "Notification sent to " << contactInfo << ": " << message << "\n";
    }
};
