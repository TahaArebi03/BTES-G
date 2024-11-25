#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

// œ«·… ·· Õﬁﬁ „‰ ’Õ… —ﬁ„ »ÿ«ﬁ… «·«∆ „«‰ »«” Œœ«„ ŒÊ«—“„Ì… Luhn
bool isValidCardNumber(const string &cardNumber) {
    int sum = 0;
    bool alternate = false;

    // „⁄«·Ã… «·—ﬁ„ „‰ «·Ì„Ì‰ ≈·Ï «·Ì”«—
    for (int i = cardNumber.size() - 1; i >= 0; i--) {
        char c = cardNumber[i];
        if (!isdigit(c)) return false; //  Õﬁﬁ „‰ √‰ «·—ﬁ„ Ì ﬂÊ‰ „‰ √—ﬁ«„ ›ﬁÿ

        int n = c - '0'; //  ÕÊÌ· «·Õ—› ≈·Ï —ﬁ„

        if (alternate) {
            n *= 2; // „÷«⁄›… ﬂ· —ﬁ„ À«‰Ì
            if (n > 9) n -= 9; // ≈–« ﬂ«‰  «·‰ ÌÃ… √ﬂ»— „‰ 9° «ÿ—Õ 9 „‰Â«
        }
        sum += n;
        alternate = !alternate; //  »œÌ· «·Õ«·…
    }

    return (sum % 10 == 0); // ≈–« ﬂ«‰ «·„Ã„Ê⁄ ﬁ«»·« ··ﬁ”„… ⁄·Ï 10° «·—ﬁ„ ’ÕÌÕ
}

// --- Base Class: Payment ---
class Payment {
public:
    virtual void pay(double amount) = 0; // œ«·… œ›⁄ „Ã—œ…
};

class CreditCard : public Payment {
public:
    void pay(double amount) override {
        cout << "Paid $" << amount << " using Credit Card." << endl;
    }
};

class PayPal : public Payment {
public:
    void pay(double amount) override {
        cout << "Paid $" << amount << " using PayPal." << endl;
    }
};

class BankTransfer : public Payment {
public:
    void pay(double amount) override {
        cout << "Paid $" << amount << " using Bank Transfer." << endl;
    }
};

class CashOnDelivery : public Payment {
public:
    void pay(double amount) override {
        cout << "Order placed for $" << amount << " to be paid on delivery." << endl;
    }
};

// --- Base Class: Person ---
class Person {
protected:
    string username;
    string password;

public:
    Person(string user, string pass) : username(user), password(pass) {}
    virtual bool login() = 0; // œ«·…  ”ÃÌ· œŒÊ· „Ã—œ…
    string getUsername() const { return username; }
    string getPassword() const { return password; }
};

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

    void adminMenu();
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

    double getPrice() const { return price; } // Getter for price
    string getName() const { return name; }   // Getter for name

    static void addEvent(const string &eventName, const string &eventType, const string &eventDate, const string &eventLocation, double eventPrice) {
        events.push_back(Event(eventName, eventType, eventDate, eventLocation, eventPrice));
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
        auto it = find_if(events.begin(), events.end(), [eventID](Event &e) { return e.eventID == eventID; });
        return (it != events.end()) ? &(*it) : nullptr;
    }
};

// Definition of static members
vector<Event> Event::events; // Definition
int Event::nextEventID = 1;  // Definition

// --- Rating Class ---
class Rating {
public:
    int eventID;
    int rating;
    string comment;

    Rating(int eID, int r, const string &c) : eventID(eID), rating(r), comment(c) {}
};

// --- Booking Class ---
class Booking {
public:
    int bookingID;
    int eventID;
    double amount;

    Booking(int bID, int eID, double amt) : bookingID(bID), eventID(eID), amount(amt) {}
};

// --- User Class ---
class User : public Person {
    int userID;
    double balance; // —’Ìœ «·„” Œœ„
    vector<Booking> bookings; // · Œ“Ì‰ «·ÕÃÊ“« 
    vector<Rating> ratings; // · Œ“Ì‰ «· ﬁÌÌ„« 
    Payment* paymentMethod; // ÿ—Ìﬁ… «·œ›⁄ «·„Œ «—…

public:
    static vector<User> users; // · Œ“Ì‰ Ã„Ì⁄ «·„” Œœ„Ì‰

    User(string user, string pass, int id, double initialBalance)
        : Person(user, pass), userID(id), balance(initialBalance), paymentMethod(nullptr) {
        users.push_back(*this); // ≈÷«›… «·„” Œœ„ ≈·Ï «·ﬁ«∆„…
    }

    int getUserID() const { return userID; }
    double getBalance() const { return balance; }

    bool login() override;
    static User createUser(int id);
    static void saveUserToFile(const User &user);
    void userMenu();
    void viewBookings() const; // œ«·… ⁄—÷ «·ÕÃÊ“«  ‰ﬁ
    void cancelBooking(size_t bookingID); // œ«·… ≈·€«¡ «·ÕÃ“
    void modifyBooking(size_t bookingID, int newEventID); // œ«·…  ⁄œÌ· «·ÕÃ“
    void rateAndComment(int eventID, int rating, const string &comment); // œ«·…  ﬁÌÌ„ Ê≈÷«›…  ⁄·Ìﬁ
    void displayRatings() const; // œ«·… ⁄—÷ «· ﬁÌÌ„« 
    void deleteRating(size_t ratingIndex); // œ«·… Õ–› «· ﬁÌÌ„
    void setPaymentMethod(Payment* method) { paymentMethod = method; } //  ⁄ÌÌ‰ ÿ—Ìﬁ… «·œ›⁄
    void displayBalance() const { cout << "Current Balance: $" << balance << endl; } // ⁄—÷ «·—’Ìœ
};

//  ⁄—Ì› «·„ €Ì—«  «·À«» …
vector<User> User::users; //  ⁄—Ì›

// --- Save User to File ---
void User::saveUserToFile(const User &user) {
    ofstream outFile("users.txt", ios::app);
    if (!outFile) {
        throw runtime_error("Error opening file for saving user.");
    }
    outFile << user.getUsername() << " " << user.getPassword() << " " << user.getBalance() << endl;
    outFile.close();
}

// --- Login Function ---
bool User::login() {
    string enteredUsername, enteredPassword;
    cout << "Enter username: ";
    cin >> enteredUsername;
    cout << "Enter password: ";
    cin >> enteredPassword;

    return (enteredUsername == username && enteredPassword == password);
}

// --- Create User Function ---
User User::createUser(int id) {
    string username, password;
    cout << "Enter new username: ";
    cin >> username;

    ifstream inFile("users.txt");
    string existingUser, existingPass;
    while (inFile >> existingUser >> existingPass) {
        if (existingUser == username) {
            throw invalid_argument("Username already exists. Try again.");
        }
    }
    inFile.close();

    cout << "Enter new password: ";
    cin >> password;

    User newUser(username, password, id, 1000.0); //  ⁄ÌÌ‰ —’Ìœ «» œ«∆Ì 1000
    saveUserToFile(newUser);
    cout << "User created successfully!\n";
    return newUser;
}

// --- Admin Menu ---
void Admin::adminMenu() {
    int choice;
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Event\n";
        cout << "2. View Events\n";
        cout << "3. View Ratings and Comments\n"; // ŒÌ«— ⁄—÷ «· ﬁÌÌ„« 
        cout << "4. Delete Rating\n"; // ŒÌ«— Õ–› «· ﬁÌÌ„
        cout << "0. Logout\n";
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
                Event::addEvent(name, type, date, location, price);
                break;
            }
            case 2:
                Event::viewEvents();
                break;
            case 3: { // ⁄—÷ «· ﬁÌÌ„« 
                for (const auto& user : User::users) {
                    user.displayRatings();
                }
                break;
            }
            case 4: { // Õ–› «· ﬁÌÌ„
                size_t userIndex, ratingIndex;
                cout << "Enter user index to delete rating from: ";
                cin >> userIndex; // ÌÃ» √‰ ÌﬂÊ‰ ·œÌﬂ Ê”Ì·… · ÕœÌœ «·„” Œœ„
                cout << "Enter rating index to delete: ";
                cin >> ratingIndex;

                if (userIndex < User::users.size()) {
                    User::users[userIndex].deleteRating(ratingIndex);
                } else {
                    cout << "Invalid user index.\n";
                }
                break;
            }
            case 0:
                return;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}void User::userMenu() {
    int choice;
    while (true) {
        cout << "\n--- User Menu ---\n";
        cout << "1. View Events\n";
        cout << "2. Book Event\n";
        cout << "3. View Bookings\n";
        cout << "4. Rate and Comment on Event\n";
        cout << "5. Cancel Booking\n";
        cout << "6. Modify Booking\n";
        cout << "7. Set Payment Method\n";
        cout << "8. Display Balance\n";
        cout << "0. Logout\n";
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
                    //  Õﬁﬁ „‰  Ê«›— «·—’Ìœ
                    if (balance >= event->getPrice()) {
                        // ≈÷«›… «·ÕÃ“
                        bookings.push_back(Booking(bookings.size(), eventID, event->getPrice()));
                        cout << "Booking successful for event: " << event->getName() << endl;

                        // ⁄—÷ ÿ—ﬁ «·œ›⁄ «·„ «Õ…
                        int paymentChoice;
                        cout << "Select Payment Method:\n";
                        cout << "1. Credit Card\n";
                        cout << "2. PayPal\n";
                        cout << "3. Bank Transfer\n";
                        cout << "4. Cash on Delivery\n";
                        cout << "Enter your choice: ";
                        cin >> paymentChoice;

                        //  ⁄—Ì› «·„ €Ì—«  Â‰«
                        Payment* paymentMethod = nullptr;
                        string cardNumber, paypalEmail; //  ⁄—Ì› «·„ €Ì—«  „‰ ﬁ»·

                        switch (paymentChoice) {
                            case 1: // »ÿ«ﬁ… «·«∆ „«‰
                                cout << "Enter your credit card number: ";
                                cin >> cardNumber; // ﬁ—«¡… —ﬁ„ «·»ÿ«ﬁ…

                                //  Õﬁﬁ „‰ ’Õ… —ﬁ„ «·»ÿ«ﬁ…
                                if (!isValidCardNumber(cardNumber)) {
                                    cout << "Invalid credit card number. Please try again.\n";
                                    break; // ≈·€«¡ «·⁄„·Ì… ≈–« ﬂ«‰ «·—ﬁ„ €Ì— ’ÕÌÕ
                                }
                                paymentMethod = new CreditCard();
                                break;
                            case 2: // »«Ì »«·
                                cout << "Enter your PayPal email: ";
                                cin >> paypalEmail; // ﬁ—«¡… »—Ìœ »«Ì »«·
                                paymentMethod = new PayPal();
                                break;
                            case 3: //  ÕÊÌ· »‰ﬂÌ
                                paymentMethod = new BankTransfer();
                                break;
                            case 4: // «·œ›⁄ ⁄‰œ «·«” ·«„
                                paymentMethod = new CashOnDelivery();
                                break;
                            default:
                                cout << "Invalid payment method.\n";
                                break;
                        }

                        if (paymentMethod) {
                            paymentMethod->pay(event->getPrice()); // „⁄«·Ã… «·œ›⁄
                            balance -= event->getPrice(); // Œ’„ «·„»·€ „‰ «·—’Ìœ
                            delete paymentMethod; //  Œ·’ „‰ ﬂ«∆‰ ÿ—Ìﬁ… «·œ›⁄ »⁄œ «·«” Œœ«„
                            cout << "Payment successful. Remaining balance: $" << balance << endl;
                        }
                    } else {
                        cout << "Insufficient balance to book this event.\n";
                    }
                } else {
                    cout << "Event not found.\n";
                }
                break;
            }
            case 3:
                viewBookings();
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

                rateAndComment(eventID, rating, comment);
                break;
            }
            case 5: {
                size_t bookingID;
                cout << "Enter booking ID to cancel: ";
                cin >> bookingID;
                cancelBooking(bookingID);
                break;
            }
            case 6: {
                size_t bookingID;
                int newEventID;
                cout << "Enter booking ID to modify: ";
                cin >> bookingID;
                cout << "Enter new event ID: ";
                cin >> newEventID;
                modifyBooking(bookingID, newEventID);
                break;
            }
            case 7: {
                // ·« Ì „ «” Œœ«„ Â–« «·ŒÌ«— ›Ì «·ﬂÊœ «·Õ«·Ì° ·ﬂ‰Â ÌÕ ›Ÿ »‰›” «·ﬂÊœ
                break;
            }
            case 8: // ⁄—÷ «·—’Ìœ
                displayBalance();
                break;
            case 0:
                return;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}

// --- œ«·… ⁄—÷ «·ÕÃÊ“«  ---
void User::viewBookings() const {
    cout << "--- Your Bookings ---\n";
    for (const auto& booking : bookings) {
        cout << "Booking ID: " << booking.bookingID << ", Event ID: " << booking.eventID << ", Amount: $" << booking.amount << endl;
    }
}

// --- œ«·… ≈·€«¡ «·ÕÃ“ ---
void User::cancelBooking(size_t bookingID) {
    if (bookingID >= bookings.size()) {
        cout << "Invalid booking ID.\n";
        return;
    }

    double refundAmount = bookings[bookingID].amount;
    balance += refundAmount; // ≈⁄«œ… «·—’Ìœ
    bookings.erase(bookings.begin() + bookingID);
    cout << "Booking cancelled successfully.\n";
}

// --- œ«·…  ⁄œÌ· «·ÕÃ“ ---
void User::modifyBooking(size_t bookingID, int newEventID) {
    if (bookingID >= bookings.size()) {
        cout << "Invalid booking ID.\n";
        return;
    }

    Event* newEvent = Event::findEventByID(newEventID);
    if (!newEvent) {
        cout << "New event not found.\n";
        return;
    }

    // ≈·€«¡ «·ÕÃ“ «·ﬁœÌ„
    cancelBooking(bookingID);

    // ÕÃ“ ÃœÌœ
    bookings.push_back(Booking(bookings.size(), newEventID, newEvent->getPrice()));
    cout << "Booking modified successfully to event: " << newEvent->getName() << endl;
}

// --- œ«·…  ﬁÌÌ„ Ê≈÷«›…  ⁄·Ìﬁ ---
void User::rateAndComment(int eventID, int rating, const string &comment) {
    ratings.push_back(Rating(eventID, rating, comment));
    cout << "Your rating for event ID " << eventID << " has been submitted with comment: " << comment << endl;
}

// --- œ«·… ⁄—÷ «· ﬁÌÌ„«  ---
void User::displayRatings() const {
    cout << "--- Your Ratings and Comments ---\n";
    for (size_t i = 0; i < ratings.size(); ++i) {
        cout << "Index: " << i << ", Event ID: " << ratings[i].eventID
             << ", Rating: " << ratings[i].rating
             << ", Comment: " << ratings[i].comment << endl;
    }
}

// --- œ«·… Õ–› «· ﬁÌÌ„ ---
void User::deleteRating(size_t ratingIndex) {
    if (ratingIndex >= ratings.size()) {
        cout << "Invalid rating index.\n";
        return;
    }

    ratings.erase(ratings.begin() + ratingIndex);
    cout << "Rating deleted successfully.\n";
}

// --- Main Function ---
int main() {
    Admin admin("admin", "1234"); // ≈⁄œ«œ Õ”«» «·„œÌ—
    int choice;
    static int userID = 1; // ID «·„” Œœ„

    while (true) {
        cout << "\n--- Welcome to the Event Management System ---\n";
        cout << "1. Admin Login\n";
        cout << "2. User Login\n";
        cout << "3. Create New User\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (admin.login()) {
                    cout << "Admin logged in successfully!\n";
                    admin.adminMenu();
                } else {
                    cout << "Invalid admin credentials.\n";
                }
                break;
            case 2: {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                User user(username, password, userID++, 1000.0); // ≈‰‘«¡ „” Œœ„ ÃœÌœ
                if (user.login()) {
                    cout << "User logged in successfully!\n";
                    user.userMenu();
                } else {
                    cout << "Invalid user credentials.\n";
                }
                break;
            }
            case 3: {
              try {
                    User::createUser(userID++);
                } catch (const invalid_argument &e) {
                    cout << e.what() << endl; // ⁄—÷ —”«·… «·Œÿ√ ≈–« ﬂ«‰ «”„ «·„” Œœ„ „ÊÃÊœ« »«·›⁄·
                }
                break;
            }
            case 0:
                cout << "Exiting the application. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}
