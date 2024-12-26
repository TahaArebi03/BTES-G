#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <fstream>


using namespace std;

// Forward Declarations
class Booking;
class Rating;
class Notification;
class Event;
class NotificationManager;
class PaymentFactory;

// --- Base Class: Person ---
class Person {
protected:
    string username;
    string password;

public:
    Person(string user, string pass) : username(user), password(pass) {}
    virtual bool login() = 0; // دالة تسجيل الدخول
    string getUsername() const { return username; }
    string getPassword() const { return password; }
};

// --- Booking Class ---
class Booking {
public:
    int bookingID;
    int eventID;
    double amount;

    Booking(int bID, int eID, double amt) : bookingID(bID), eventID(eID), amount(amt) {}
};

// --- Rating Class ---
class Rating {
public:
    int eventID;
    int rating;
    string comment;

    Rating(int eID, int r, const string &c) : eventID(eID), rating(r), comment(c) {}
};

// --- Notification Class ---
class Notification {
private:
    static int nextNotificationID;
    int notificationID;
    string message;
    string recipient;
    bool isRead;

public:
    Notification(const string& msg, const string& user)
        : notificationID(nextNotificationID++), message(msg), recipient(user), isRead(false) {}

    int getNotificationID() const { return notificationID; }
    string getMessage() const { return message; }
    string getRecipient() const { return recipient; }
    bool getIsRead() const { return isRead; }
    void markAsRead() { isRead = true; }

    static void sendNotification(vector<Notification>& notifications, const string& msg, const string& user) {
        notifications.push_back(Notification(msg, user));
        cout << "Notification sent to " << user << ": " << msg << endl;
    }

    static void viewNotifications(const vector<Notification>& notifications, const string& user) {
        cout << "--- Notifications for " << user << " ---\n";
        for (const auto& notification : notifications) {
            if (notification.getRecipient() == user) {
                cout << "ID: " << notification.getNotificationID()
                     << ", Message: " << notification.getMessage()
                     << ", Status: " << (notification.getIsRead() ? "Read" : "Unread") << endl;
            }
        }
    }

    static void markNotificationAsRead(vector<Notification>& notifications, int notificationID, const string& user) {
        for (auto& notification : notifications) {
            if (notification.getNotificationID() == notificationID && notification.getRecipient() == user) {
                notification.markAsRead();
                cout << "Notification ID " << notificationID << " marked as read.\n";
                return;
            }
        }
        cout << "Notification not found or not for this user.\n";
    }
};

// تعريف المتغير الثابت
int Notification::nextNotificationID = 1;

// --- NotificationManager Class ---
class NotificationManager {
private:
    vector<Notification> notifications;

public:
    void addNotification(const string& message, const string& recipient) {
        Notification::sendNotification(notifications, message, recipient);
    }

    void viewNotifications(const string& user) const {
        Notification::viewNotifications(notifications, user);
    }
};

// --- Seat Class ---
class Seat {
private:
    int seatNumber;       // رقم المقعد
    bool isReserved;      // حالة الحجز (محجوز أو متاح)
    int eventID;          // رقم الحدث المرتبط بالمقعد
    string reservedBy;    // اسم المستخدم الذي حجز المقعد

public:
    // Constructor
    Seat(int number, int eID) : seatNumber(number), eventID(eID), isReserved(false), reservedBy("") {}

    // Getter for seat number
    int getSeatNumber() const { return seatNumber; }

    // Getter for event ID
    int getEventID() const { return eventID; }

    // Check if the seat is reserved
    bool getIsReserved() const { return isReserved; }

    // Get reserved by
    string getReservedBy() const { return reservedBy; }

    // Reserve the seat
    bool reserveSeat(const string& username) {
        if (isReserved) {
            cout << "Seat " << seatNumber << " is already reserved.\n";
            return false;
        }
        isReserved = true;
        reservedBy = username;
        cout << "Seat " << seatNumber << " has been reserved by " << username << ".\n";
        return true;
    }

    // Cancel reservation
    bool cancelReservation(const string& username) {
        if (!isReserved) {
            cout << "Seat " << seatNumber << " is not reserved.\n";
            return false;
        }
        if (reservedBy != username) {
            cout << "You cannot cancel this reservation. It is reserved by " << reservedBy << ".\n";
            return false;
        }
        isReserved = false;
        reservedBy = "";
        cout << "Reservation for seat " << seatNumber << " has been canceled.\n";
        return true;
    }

    // View seat details
    void viewSeatDetails() const {
        cout << "Seat Number: " << seatNumber
             << ", Event ID: " << eventID
             << ", Status: " << (isReserved ? "Reserved" : "Available")
             << (isReserved ? ", Reserved by: " + reservedBy : "") << endl;
    }
};

// --- Event Class ---
class Event {
private:
    int eventID;
    string name;
    string type;
    string date;
    string location;
    double price;
    vector<Seat> seats;

public:
    static vector<Event> events; // Declaration
    static int nextEventID;
    bool operator==(const Event& other) const {
        return eventID == other.eventID; // مقارنة باستخدام ID
    }
    const vector<Seat>& getSeats() const {
        return seats;
    }    // Declaration

    Event(const string &eventName, const string &eventType, const string &eventDate, const string &eventLocation, double eventPrice, int totalSeats)
        : eventID(nextEventID++), name(eventName), type(eventType), date(eventDate), location(eventLocation), price(eventPrice) {
        // إنشاء المقاعد عند إنشاء الحدث
        for (int i = 1; i <= totalSeats; ++i) {
            seats.emplace_back(i, eventID);
        }
    }

    double getPrice() const { return price; }
    string getName() const { return name; }

    static void viewEvents() {
        cout << "--- Events ---\n";
        for (const Event &event : events) {
            cout << "ID: " << event.eventID
                 << ", Name: " << event.getName()
                 << ", Type: " << event.type
                 << ", Date: " << event.date
                 << ", Location: " << event.location
                 << ", Price: $" << event.price << endl;

            // عرض المقاعد المتاحة
            cout << "Available Seats: ";
            for (const auto& seat : event.seats) {
                if (!seat.getIsReserved()) {
                    cout << seat.getSeatNumber() << " ";
                }
            }
            cout << endl; // الانتقال للسطر الجديد بعد عرض المقاعد
        }
    }

    static Event* findEventByID(int eventID) {
        auto it = find_if(events.begin(), events.end(), [eventID](const Event &e) { return e.eventID == eventID; });
        return (it != events.end()) ? &(*it) : nullptr;
    }

    void viewSeats() const {
        cout << "--- Seats for Event: " << name << " ---\n";
        for (const auto& seat : seats) {
            seat.viewSeatDetails();
        }
    }

    bool reserveSeat(int seatNumber, const string& username) {
        Seat* seat = findSeat(seatNumber);
        if (seat) {
            return seat->reserveSeat(username);
        }
        cout << "Seat not found.\n";
        return false;
    }

    bool cancelSeatReservation(int seatNumber, const string& username) {
        Seat* seat = findSeat(seatNumber);
        if (seat) {
            return seat->cancelReservation(username);
        }
        cout << "Seat not found.\n";
        return false;
    }

    Seat* findSeat(int seatNumber) {
        for (auto& seat : seats) {
            if (seat.getSeatNumber() == seatNumber) {
                return &seat;
            }
        }
        return nullptr;
    }


};

// تعريف المتغيرات الثابتة
vector<Event> Event::events; // Definition
int Event::nextEventID = 1;  // Definition

// --- Payment Interface and Implementations ---
class Payment {
public:
    virtual void pay(double amount) const = 0;
    virtual ~Payment() = default;
};

class CreditCardPayment : public Payment {
public:
    void pay(double amount) const override {
        cout << "Paying " << amount << " using Credit Card.\n";
    }
};

class PayPalPayment : public Payment {
public:
    void pay(double amount) const override {
        cout << "Paying " << amount << " using PayPal.\n";
    }
};

class BankTransferPayment : public Payment {
public:
    void pay(double amount) const override {
        cout << "Paying " << amount << " using Bank Transfer.\n";
    }
};

class CashPayment : public Payment {
public:
    void pay(double amount) const override {
        cout << "Paying " << amount << " in Cash.\n";
    }
};

class PaymentFactory {
public:
    static unique_ptr<Payment> createPayment(const string& type) {
        if (type == "CreditCard") {
            return make_unique<CreditCardPayment>();
        } else if (type == "PayPal") {
            return make_unique<PayPalPayment>();
        } else if (type == "BankTransfer") {
            return make_unique<BankTransferPayment>();
        } else if (type == "Cash") {
            return make_unique<CashPayment>();
        } else {
            throw invalid_argument("Invalid payment type");
        }
    }
};

// --- User Class ---
class User : public Person {
    int userID;
    double balance;
    vector<Booking> bookings;

public:
    vector<Rating> ratings; // Make ratings public
    static vector<User> users;
    NotificationManager notificationManager; // إضافة NotificationManager للمستخدم

    User(string user, string pass, int id, double initialBalance)
        : Person(user, pass), userID(id), balance(initialBalance) {}

    int getUserID() const { return userID; }
    double getBalance() const { return balance; }

    bool login() override {
        string enteredUsername, enteredPassword;
        cout << "Enter username: ";
        cin >> enteredUsername;
        cout << "Enter password: ";
        cin >> enteredPassword;

        return (enteredUsername == username && enteredPassword == password);
    }

    static User createUser(int id) {
        string username, password;
        cout << "Enter new username: ";
        cin >> username;

        for (const auto& user : users) {
            if (user.getUsername() == username) {
                throw invalid_argument("Username already exists. Try again.");
            }
        }

        cout << "Enter new password: ";
        cin >> password;

        User newUser(username, password, id, 1000.0); // تعيين رصيد أولي 1000
        users.push_back(newUser); // إضافة المستخدم إلى القائمة
        return newUser;
    }

    Booking* findBookingByID(int bookingID) {
        for (auto& booking : bookings) {
            if (booking.bookingID == bookingID) {
                return &booking;
            }
        }
        return nullptr;
    }

    void userMenu() {
        int choice;
        while (true) {
            cout << "\n--- User Menu ---\n";
            cout << "1. View Events\n";
            cout << "2. Book Event\n";
            cout << "3. View Bookings\n";
            cout << "4. Rate and Comment on Event\n";
            cout << "5. View Notifications\n";
            cout << "6. Pay for Booking\n";
            cout << "7. Choose Seat\n";  // إضافة خيار اختيار مقعد
            cout << "0. Logout\n";       // خيار تسجيل الخروج
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    Event::viewEvents();
                    break;
                case 2: {
                    int eventID;
                    cout << "Enter event ID to book: ";
                    cin >> eventID;
                    Event* event = Event::findEventByID(eventID);
                    if (event) {
                        // حجز الحدث
                        if (balance >= event->getPrice()) {
                            bookings.push_back(Booking(bookings.size() + 1, eventID, event->getPrice()));
                            balance -= event->getPrice();
                            cout << "Booking successful for event: " << event->getName() << endl;
                        } else {
                            cout << "Insufficient balance to book this event.\n";
                        }
                    } else {
                        cout << "Event not found.\n";
                    }
                    break;
                }case 3: {
    cout << "--- Your Bookings ---\n";
    for (const auto& booking : bookings) {
        cout << "Booking ID: " << booking.bookingID
             << ", Event ID: " << booking.eventID
             << ", Amount: $" << booking.amount << endl;

        Event* event = Event::findEventByID(booking.eventID);
        if (event) {
            // عرض المقاعد المحجوزة في الحدث
            cout << "Reserved Seats: ";
            for (const auto& seat : event->getSeats()) { // استخدام الدالة الجديدة
                if (seat.getIsReserved() && seat.getReservedBy() == username) {
                    cout << seat.getSeatNumber() << " ";
                }
            }
            cout << endl;
        }
    }
    break;
}
                case 4:
                    // تقييم الحدث
                    break;
                case 5:
                    // عرض الإشعارات
                    notificationManager.viewNotifications(username);
                    break;
                case 6:
                    // الدفع للحجز
                    break;
                case 7: {
                    // اختيار المقعد
                    int eventID, seatNumber;
                    cout << "Enter Event ID to choose a seat: ";
                    cin >> eventID;
                    Event* event = Event::findEventByID(eventID);
                    if (event) {
                        event->viewSeats();
                        cout << "Enter Seat Number to reserve: ";
                        cin >> seatNumber;
                        if (event->reserveSeat(seatNumber, username)) {
                            cout << "Seat reserved successfully.\n";
                        } else {
                            cout << "Failed to reserve seat.\n";
                        }
                    } else {
                        cout << "Event not found.\n";
                    }
                    break;
                }
                case 0:
                    cout << "Logging out...\n";
                    return;
                default:
                    cout << "Invalid choice! Try again.\n";
            }
        }
    }


};

// تعريف قائمة المستخدمين
vector<User> User::users;

// --- Admin Class ---
class Admin : public Person {
public:
    Admin(string user, string pass) : Person(user, pass) {}

    bool login() override {
        string enteredUsername, enteredPassword;
        cout << "Enter admin username: ";
        cin >> enteredUsername;
        cout << "Enter admin password: ";
        cin >> enteredPassword;

        return (enteredUsername == username && enteredPassword == password);
    }

    void adminMenu(NotificationManager& nm) { // تمرير NotificationManager
        int choice;
        while (true) {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Add Event\n";
            cout << "2. View Events\n";
            cout << "3. View Ratings and Comments\n";
            cout << "4. Delete Event\n"; // خيار لإلغاء حدث
            cout << "0. Logout\n"; // خيار تسجيل الخروج
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string name, type, date, location;
                    double price;
                    int totalSeats;
                    cout << "Enter event name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter event type: ";
                    getline(cin, type);
                    cout << "Enter event date: ";
                    getline(cin, date);
                    cout << "Enter event location: ";
                    getline(cin, location);
                    cout << "Enter event price: ";
                    cin >> price;
                    cout << "Enter total seats: ";
                    cin >> totalSeats;
                    Event newEvent(name, type, date, location, price, totalSeats);
                    Event::events.push_back(newEvent);
                    nm.addNotification("New event added: " + name, "all_users");
                    cout << "Event added successfully.\n";
                    break;
                }
                case 2:
                    Event::viewEvents();
                    break;
                case 4: {
                    int eventID;
                    cout << "Enter event ID to delete: ";
                    cin >> eventID;
                    Event* eventToDelete = Event::findEventByID(eventID);
                    if (eventToDelete) {
                        Event::events.erase(remove(Event::events.begin(), Event::events.end(), *eventToDelete), Event::events.end());
                        nm.addNotification("Event canceled: " + eventToDelete->getName(), "all_users");
                        cout << "Event deleted successfully.\n";
                    } else {
                        cout << "Event not found.\n";
                    }
                    break;
                }
                case 3: {
                    cout << "--- Ratings and Comments ---\n";
                    for (const auto& user : User::users) {
                        cout << "User: " << user.getUsername() << "\n";
                        for (const auto& rating : user.ratings) { // Access ratings directly
                            Event* event = Event::findEventByID(rating.eventID);
                            if (event) {
                                cout << "Event: " << event->getName() << ", Rating: " << rating.rating << ", Comment: " << rating.comment << "\n";
                            }
                        }
                    }
                    break;
                }
                case 0: // تسجيل الخروج
                    cout << "Logging out...\n";
                    return; // تنتهي الحلقة وتعود إلى القائمة الرئيسية
                default:
                    cout << "Invalid choice! Try again.\n";
            }
        }
    }
};

int main() {
    NotificationManager nm; // Create a NotificationManager object
    int choice;
    while (true) {
        cout << "\nMain Menu:\n";
        cout << "1. Login as User\n";
        cout << "2. Login as Admin\n";
        cout << "3. Create New User\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                auto it = find_if(User::users.begin(), User::users.end(),
                                  [&username, &password](const User& user) {
                                      return user.getUsername() == username && user.getPassword() == password;
                                  });

                if (it != User::users.end()) {
                    it->userMenu(); // دخول المستخدم
                } else {
                    cout << "Login failed. Try again.\n";
                }
                break;
            }
            case 2: {
                string adminUsername = "admin";
                string adminPassword = "1234"; // كلمة المرور الافتراضية
                Admin admin(adminUsername, adminPassword);

                if (admin.login()) {
                    admin.adminMenu(nm); // دخول المدير
                } else {
                    cout << "Admin login failed. Try again.\n";
                }
                break;
            }
            case 3: {
                try {
                    User newUser = User::createUser(User::users.size() + 1);
                    cout << "User created successfully with username: " << newUser.getUsername() << endl;
                } catch (const invalid_argument& e) {
                    cout << e.what() << endl;
                }
                break;
            }
            case 0:
                cout << "Exiting...\n";
                return 0; // إنهاء البرنامج
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
