#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <map>
#include <fstream>

using namespace std;

// Class to represent a person (Admin or User)
class Person {
protected:
    string username;
    string password;

public:
    Person(string user, string pass) : username(user), password(pass) {}

    virtual bool login() = 0; // Pure virtual function for login

    string getUsername() const { return username; }
    string getPassword() const { return password; }
};

// Admin class inheriting from Person
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

// User class inheriting from Person
class User : public Person {
    int userID; // Unique identifier for the user

public:
    User(string user, string pass, int id) : Person(user, pass), userID(id) {}

    int getUserID() const { return userID; }

    bool login() override {
        string enteredUsername, enteredPassword;
        cout << "Enter username: ";
        cin >> enteredUsername;
        cout << "Enter password: ";
        cin >> enteredPassword;

        // Check credentials from file
        ifstream inFile("users.txt");
        string user, pass;
        while (inFile >> user >> pass) {
            if (user == enteredUsername && pass == enteredPassword) {
                return true;
            }
        }
        return false; // Invalid credentials
    }

    void userMenu(); // User menu function
    static User createUser(int id); // Static function to create a user
    static void saveUserToFile(const User& user); // Save user to file
};

// Event Management Classes
class Seat {
    int seatNumber;
    bool isReserved;

public:
    Seat(int number) : seatNumber(number), isReserved(false) {}
    int getSeatNumber() const { return seatNumber; }
    bool checkAvailability() const { return !isReserved; }
    void reserve() {
        if (isReserved) throw runtime_error("Seat already reserved.");
        isReserved = true;
    }
};

class Event {
    int eventID;
    string name;
    string type;
    string date;
    string location;
    vector<Seat> seats;
    static vector<Event> events;
    static int nextEventID;

public:
    Event(const string& eventName, const string& eventType, const string& eventDate, const string& eventLocation, int seatCount);
    static void addEvent(const string& eventName, const string& eventType, const string& eventDate, const string& eventLocation, int seatCount);
    static void deleteEvent(int eventID);
    static void viewEvents();
    static const vector<Event>& getEvents();
    static Event* findEventByID(int eventID);
    int getEventID() const { return eventID; }
    string getName() const;
    string getLocation() const { return location; }
    int getAvailableSeats() const;
    void reserveSeat(int seatNumber);
    void displaySeats() const;
    static void saveToFile(const string& filename);
};

vector<Event> Event::events;
int Event::nextEventID = 1;

Event::Event(const string& eventName, const string& eventType, const string& eventDate, const string& eventLocation, int seatCount)
    : eventID(nextEventID++), name(eventName), type(eventType), date(eventDate), location(eventLocation) {
    for (int i = 1; i <= seatCount; ++i) {
        seats.push_back(Seat(i));
    }
}

void Event::addEvent(const string& eventName, const string& eventType, const string& eventDate, const string& eventLocation, int seatCount) {
    if (eventName.empty() || seatCount <= 0) throw invalid_argument("Invalid event name or seat count.");
    events.push_back(Event(eventName, eventType, eventDate, eventLocation, seatCount));
    cout << "Event added successfully.\n";
    saveToFile("events.txt");
}

void Event::deleteEvent(int eventID) {
    for (auto it = events.begin(); it != events.end(); ++it) {
        if (it->getEventID() == eventID) {
            events.erase(it);
            cout << "Event deleted successfully.\n";
            saveToFile("events.txt");
            return;
        }
    }
    throw out_of_range("Event ID does not exist.");
}

void Event::viewEvents() {
    cout << "--- Current Events ---\n";
    for (const Event& event : events) {
        cout << "ID: " << event.getEventID() << ", Name: " << event.getName()
             << " - Type: " << event.type << ", Date: " << event.date
             << ", Location: " << event.location
             << " - Available Seats: " << event.getAvailableSeats() << endl;
    }
}

const vector<Event>& Event::getEvents() {
    return events;
}

Event* Event::findEventByID(int eventID) {
    for (Event& event : events) {
        if (event.getEventID() == eventID) return &event;
    }
    return nullptr;
}

string Event::getName() const {
    return name;
}

int Event::getAvailableSeats() const {
    int availableSeats = 0;
    for (const Seat& seat : seats) {
        if (seat.checkAvailability()) ++availableSeats;
    }
    return availableSeats;
}

void Event::reserveSeat(int seatNumber) {
    if (seatNumber < 1 || seatNumber > seats.size()) throw out_of_range("Invalid seat number.");
    seats[seatNumber - 1].reserve();
}

void Event::displaySeats() const {
    cout << "Seats for event '" << name << "':\n";
    for (const Seat& seat : seats) {
        cout << "Seat " << seat.getSeatNumber() << " - " << (seat.checkAvailability() ? "Available" : "Reserved") << endl;
    }
}

void Event::saveToFile(const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        throw runtime_error("Unable to open file for writing.");
    }
    for (const Event& event : events) {
        outFile << event.getEventID() << " " << event.getName() << " " << event.type << " " << event.date << " "
                << event.getLocation() << " " << event.getAvailableSeats() << endl;
    }
    outFile.close();
}

// Booking Management
class Booking {
private:
    int userID;
    int eventID;
    vector<int> reservedSeats;

public:
    Booking(int userID, int eventID, const vector<int>& seats)
        : userID(userID), eventID(eventID), reservedSeats(seats) {}

    int getUserID() const { return userID; }
    int getEventID() const { return eventID; }
    const vector<int>& getReservedSeats() const { return reservedSeats; }

    void cancelBooking();
    void modifyBooking(const vector<int>& newSeats);
    static void viewBookings(int userID);
};

map<int, vector<Booking>> userBookings;

void Booking::cancelBooking() {
    auto& bookings = userBookings[userID];
    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        if (it->getEventID() == eventID) {
            bookings.erase(it);
            cout << "Booking cancelled successfully for event ID: " << eventID << endl;
            return;
        }
    }
    throw runtime_error("No booking found to cancel.");
}

void Booking::modifyBooking(const vector<int>& newSeats) {
    auto& bookings = userBookings[userID];
    for (Booking& booking : bookings) {
        if (booking.getEventID() == eventID) {
            booking.reservedSeats = newSeats;  // Modify seats
            cout << "Booking modified successfully for event ID: " << eventID << endl;
            return;
        }
    }
    throw runtime_error("No booking found to modify.");
}

void Booking::viewBookings(int userID) {
    const auto& bookings = userBookings[userID];
    if (bookings.empty()) {
        cout << "No bookings found.\n";
        return;
    }
    cout << "--- My Bookings ---\n";
    for (const Booking& booking : bookings) {
        Event* event = Event::findEventByID(booking.getEventID());
        if (event) {
            cout << "Event Name: " << event->getName() << ", Location: " << event->getLocation()
                 << ", Reserved Seats: ";
            for (int seat : booking.getReservedSeats()) {
                cout << seat << " ";
            }
            cout << endl;
        }
    }
}

// Admin Menu function
void Admin::adminMenu() {
    int choice;
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Event\n";
        cout << "2. Delete Event\n";
        cout << "3. View Events\n";
        cout << "0. Return to Main Menu\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string eventName, eventType, eventDate, eventLocation;
                int seatCount;
                cout << "Enter event name: ";
                cin >> eventName;
                cout << "Enter event type: ";
                cin >> eventType;
                cout << "Enter event date (YYYY-MM-DD): ";
                cin >> eventDate;
                cout << "Enter event location: ";
                cin >> eventLocation;
                cout << "Enter seat count: ";
                cin >> seatCount;
                Event::addEvent(eventName, eventType, eventDate, eventLocation, seatCount);
                break;
            }
            case 2: {
                int eventID;
                cout << "Enter event ID to delete: ";
                cin >> eventID;
                Event::deleteEvent(eventID);
                break;
            }
            case 3:
                Event::viewEvents();
                break;
            case 0:
                return;
            default:
                cout << "Invalid choice.\n";
        }
    }
}

// User Menu function
void User::userMenu() {
    int choice;
    while (true) {
        cout << "\n--- User Menu ---\n";
        cout << "1. Choose Event\n";
        cout << "2. View Available Seats\n";
        cout << "3. Book Seats\n";
        cout << "4. View My Reservations\n";
        cout << "5. Modify My Booking\n";
        cout << "6. Cancel My Booking\n";
        cout << "0. Return to Main Menu\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Event::viewEvents();
                int eventID;
                cout << "Enter event ID to choose: ";
                cin >> eventID;
                Event* event = Event::findEventByID(eventID);
                if (event) {
                    cout << "Event '" << event->getName() << "' selected successfully.\n";
                } else {
                    cout << "Invalid event ID.\n";
                }
                break;
            }
            case 2: {
                int eventID;
                cout << "Enter event ID to view seats: ";
                cin >> eventID;
                Event* event = Event::findEventByID(eventID);
                if (event) {
                    event->displaySeats();
                } else {
                    cout << "Invalid event ID.\n";
                }
                break;
            }
            case 3: {
                int eventID;
                cout << "Enter event ID to book seats: ";
                cin >> eventID;
                Event* event = Event::findEventByID(eventID);
                if (!event) {
                    cout << "Invalid event ID.\n";
                    break;
                }
                vector<int> seatNumbers;
                int numSeats;
                cout << "Enter number of seats to book: ";
                cin >> numSeats;
                for (int i = 0; i < numSeats; ++i) {
                    int seatNumber;
                    cout << "Enter seat number to book: ";
                    cin >> seatNumber;
                    try {
                        event->reserveSeat(seatNumber);
                        seatNumbers.push_back(seatNumber);
                    } catch (const runtime_error& e) {
                        cout << e.what() << endl; // Handle seat reservation error
                    }
                }
                userBookings[getUserID()].emplace_back(Booking(getUserID(), eventID, seatNumbers));
                cout << numSeats << " seats successfully booked.\n";
                break;
            }
            case 4:
                Booking::viewBookings(getUserID());
                break;
            case 5: {
                int eventID;
                cout << "Enter event ID to modify booking: ";
                cin >> eventID;
                vector<int> newSeats;
                int newSeatCount;
                cout << "Enter number of new seats to book: ";
                cin >> newSeatCount;
                for (int i = 0; i < newSeatCount; ++i) {
                    int seatNumber;
                    cout << "Enter new seat number: ";
                    cin >> seatNumber;
                    newSeats.push_back(seatNumber);
                }
                Booking booking(getUserID(), eventID, newSeats);
                booking.modifyBooking(newSeats);
                break;
            }
            case 6: {
                int eventID;
                cout << "Enter event ID to cancel booking: ";
                cin >> eventID;
                Booking booking(getUserID(), eventID, {});
                try {
                    booking.cancelBooking();
                } catch (const runtime_error& e) {
                    cout << e.what() << endl; // Handle error if booking not found
                }
                break;
            }
            case 0:
                return;
            default:
                cout << "Invalid choice.\n";
        }
    }
}

// Function to create a new user
User User::createUser(int id) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    return User(username, password, id);
}

// Save user data to file
void User::saveUserToFile(const User& user) {
    ofstream outFile("users.txt", ios::app); // Append mode
    if (!outFile) {
        throw runtime_error("Unable to open file for writing.");
    }
    outFile << user.getUsername() << " " << user.getPassword() << endl;
    outFile.close();
}

int main() {
    vector<Person*> people;

    // Sample admin (for testing purposes)
    people.push_back(new Admin("admin", "admin123"));

    int mainChoice;

    while (true) {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Admin Login\n";
        cout << "2. User Login\n";
        cout << "3. Create User Account\n"; // Available only here
        cout << "0. Exit\n";
        cout << "Select an option: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1: {
                bool loggedIn = false;
                for (Person* person : people) {
                    if (dynamic_cast<Admin*>(person) && person->login()) {
                        loggedIn = true;
                        dynamic_cast<Admin*>(person)->adminMenu();
                        break;
                    }
                }
                if (!loggedIn) cout << "Invalid login.\n";
                break;
            }
            case 2: {
                bool loggedIn = false;
                for (Person* person : people) {
                    if (dynamic_cast<User*>(person) && person->login()) {
                        loggedIn = true;
                        dynamic_cast<User*>(person)->userMenu();
                        break;
                    }
                }
                if (!loggedIn) cout << "Invalid login.\n";
                break;
            }
            case 3: { // Create User Account
                static int nextUserID = 1; // Static variable for user ID
                User newUser = User::createUser(nextUserID++);
                User::saveUserToFile(newUser); // Save new user to file
                people.push_back(new User(newUser.getUsername(), newUser.getPassword(), newUser.getUserID()));
                cout << "Account created successfully.\n";
                break;
            }
            case 0:
                cout << "Exiting the system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
}
