#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <memory>

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

// --- Event Class ---
class Event {
private:
    int eventID;
    string name;
    string type;
    string date;
    string location;
    double price;

public:
    static vector<Event> events; // Declaration
    static int nextEventID;      // Declaration

    Event(const string &eventName, const string &eventType, const string &eventDate, const string &eventLocation, double eventPrice)
        : eventID(nextEventID++), name(eventName), type(eventType), date(eventDate), location(eventLocation), price(eventPrice) {}

    // تعريف مشغل == للمقارنة بين كائنات Event
    bool operator==(const Event& other) const {
        return (eventID == other.eventID &&
                name == other.name &&
                type == other.type &&
                date == other.date &&
                location == other.location &&
                price == other.price);
    }

    double getPrice() const { return price; }
    string getName() const { return name; }

    static void addEvent(const string &eventName, const string &eventType, const string &eventDate, const string &eventLocation, double eventPrice, NotificationManager& nm) {
        events.push_back(Event(eventName, eventType, eventDate, eventLocation, eventPrice));
        nm.addNotification("New event added: " + eventName, "all_users"); // إعلام جميع المستخدمين
        cout << "Event added successfully.\n";
    }

    static void viewEvents() {
        cout << "--- Events ---\n";
        for (const Event &event : events) {
            cout << "ID: " << event.eventID << ", Name: " << event.getName()
                 << ", Type: " << event.type << ", Date: " << event.date
                 << ", Location: " << event.location << ", Price: $" << event.price << endl;
        }
    }

    static Event* findEventByID(int eventID) {
        auto it = find_if(events.begin(), events.end(), [eventID](const Event &e) { return e.eventID == eventID; });
        return (it != events.end()) ? &(*it) : nullptr;
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
            cout << "0. Logout\n"; // خيار تسجيل الخروج
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
                        if (balance >= event->getPrice()) {
                            bookings.push_back(Booking(bookings.size() + 1, eventID, event->getPrice()));
                            balance -= event->getPrice();
                            notificationManager.addNotification("You have successfully booked the event: " + event->getName(), username); // إعلام المستخدم
                            cout << "Booking successful for event: " << event->getName() << endl;
                        } else {
                            cout << "Insufficient balance to book this event.\n";
                        }
                    } else {
                        cout << "Event not found.\n";
                    }
                    break;
                }
                case 3:
                    cout << "--- Your Bookings ---\n";
                    for (const auto& booking : bookings) {
                        cout << "Booking ID: " << booking.bookingID << ", Event ID: " << booking.eventID << ", Amount: $" << booking.amount << endl;
                    }
                    break;
                case 4: {
                    int eventID;
                    cout << "Enter event ID to rate: ";
                    cin >> eventID;
                    int rating;
                    string comment;
                    cout << "Enter your rating (1-5): ";
                    cin >> rating;
                    cout << "Enter your comment: ";
                    cin.ignore();
                    getline(cin, comment);
                    ratings.push_back(Rating(eventID, rating, comment));
                    notificationManager.addNotification("You rated event ID " + to_string(eventID), username);
                    break;
                }
                case 5:
                    notificationManager.viewNotifications(username);
               case 6: { // خيار الدفع
    int bookingID;
    cout << "Enter booking ID to pay for: ";
    cin >> bookingID;
    Booking* booking = findBookingByID(bookingID);
    if (booking) {
        string paymentType;
        cout << "Select payment method:\n";
        cout << "1. Credit Card\n";
        cout << "2. PayPal\n";
        cout << "3. Bank Transfer\n";
        cout << "4. Cash\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                paymentType = "CreditCard";
                break;
            case 2:
                paymentType = "PayPal";
                break;
            case 3:
                paymentType = "BankTransfer";
                break;
            case 4:
                paymentType = "Cash";
                break;
            default:
                cout << "Invalid payment method. Defaulting to Cash.\n";
                paymentType = "Cash";
        }
        auto payment = PaymentFactory::createPayment(paymentType);
        payment->pay(booking->amount);
        balance -= booking->amount;

        // إرسال إشعار بعد الدفع
        notificationManager.addNotification(
            "Payment successful: $" + to_string(booking->amount) + " has been deducted. Remaining balance: $" + to_string(balance),
            username
        );

        cout << "Payment successful. You paid $" << booking->amount << ". Remaining balance: $" << balance << ".\n";
    } else {
        cout << "Booking not found.\n";
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
                    Event::addEvent(name, type, date, location, price, nm); // تمرير NotificationManager
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
                        nm.addNotification("Event canceled: " + eventToDelete->getName(), "all_users"); // إعلام جميع المستخدمين
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

                User user(username, password, User::users.size() + 1, 1000.0); // إنشاء مستخدم جديد
                if (user.login()) {
                    cout << "Login successful. Welcome, " << user.getUsername() << "!\n";
                    user.userMenu();
                } else {
                    cout << "Login failed. Try again.\n";
                }
                break;
            }
            case 2: {
                Admin admin("admin", "1234");
                if (admin.login()) {
                    cout << "Admin login successful.\n";
                    admin.adminMenu(nm); // pass the NotificationManager object
                } else {
                    cout << "Admin login failed. Try again.\n";
                }
                break;
            }
            case 3: {
                try {
                    User::createUser(User::users.size() + 1); // إضافة مستخدم جديد
                    cout << "New user created successfully.\n";
                } catch (const invalid_argument& e) {
                    cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 0:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}
