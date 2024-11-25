#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

// --- Base Class: Person ---
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
    void viewUserRatings(int eventID);
    void deleteUserComment(int eventID, int commentIndex);
};

// --- User Class ---
class User : public Person {
    int userID;

public:
    User(string user, string pass, int id) : Person(user, pass), userID(id) {}

    int getUserID() const { return userID; }

    bool login() override {
        string enteredUsername, enteredPassword;
        cout << "Enter username: ";
        cin >> enteredUsername;
        cout << "Enter password: ";
        cin >> enteredPassword;

        ifstream inFile("users.txt");
        string user, pass;
        while (inFile >> user >> pass) {
            if (user == enteredUsername && pass == enteredPassword) {
                return true;
            }
        }
        return false; // Invalid credentials
    }

    static User createUser(int id);
    static void saveUserToFile(const User &user);
    void userMenu();
    void viewBookings() const; // œ«·… ÃœÌœ… ·⁄—÷ «·ÕÃÊ“« 
};

// --- Static Function to Create a New User ---
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

    User newUser(username, password, id);
    saveUserToFile(newUser);
    cout << "User created successfully!\n";
    return newUser;
}

// --- Save User to File ---
void User::saveUserToFile(const User &user) {
    ofstream outFile("users.txt", ios::app);
    if (!outFile) {
        throw runtime_error("Error opening file for saving user.");
    }
    outFile << user.getUsername() << " " << user.getPassword() << endl;
    outFile.close();
}

class Rating {
private:
    struct RatingComment {
        int rating;
        string comment;
        string username; // «”„ «·„” Œœ„
    };

    vector<RatingComment> ratings; //  ﬁÌÌ„«  „⁄ «· ⁄·Ìﬁ« 

public:
    void addRating(int rating, const string& comment, const string& username) {
        if (rating < 1 || rating > 5) {
            throw invalid_argument("Rating must be between 1 and 5.");
        }
        ratings.push_back({rating, comment, username});
    }

    void displayRatingsAndComments() const {
        if (ratings.empty()) {
            cout << "No ratings or comments yet.\n";
        } else {
            cout << "Ratings and Comments:\n";
            for (size_t i = 0; i < ratings.size(); ++i) {
                const auto& rc = ratings[i];
                cout << i + 1 << ". " << rc.username << " rated: " << rc.rating
                     << " | Comment: " << rc.comment << endl;
            }
        }
    }

    void deleteRatingComment(int index) {
        if (index < 1 || index > static_cast<int>(ratings.size())) {
            throw invalid_argument("Invalid index.");
        }
        ratings.erase(ratings.begin() + (index - 1));
    }

    const vector<RatingComment>& getRatings() const {
        return ratings;
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
    Rating eventRatings; // Â–« «·„ €Ì— Œ«’
    static vector<Event> events;
    static int nextEventID;

public:
    Event(const string &eventName, const string &eventType, const string &eventDate, const string &eventLocation)
        : eventID(nextEventID++), name(eventName), type(eventType), date(eventDate), location(eventLocation) {}

    // --- Nested Booking Class ---
    class Booking {
    private:
        int bookingID;
        int userID;
        int eventID; // ≈÷«›… „⁄—› «·ÕœÀ
        static vector<Booking> bookings;
        static int nextBookingID;

    public:
        Booking(int user, int event) : bookingID(nextBookingID++), userID(user), eventID(event) {}

        int getUserID() const { return userID; } // œ«·… «·Ê’Ê· ·‹ userID
        int getBookingID() const { return bookingID; } // œ«·… «·Ê’Ê· ·‹ bookingID
        int getEventID() const { return eventID; } // œ«·… «·Ê’Ê· ·‹ eventID

        static void bookEvent(int userID, Event &event) {
            bookings.push_back(Booking(userID, event.eventID)); // ≈÷«›… „⁄—› «·ÕœÀ
            cout << "Booking successful for event: " << event.name << endl;
        }

        static void modifyBooking(int bookingID, int newEventID) {
            auto it = find_if(bookings.begin(), bookings.end(), [bookingID](const Booking &b) {
                return b.bookingID == bookingID;
            });
            if (it != bookings.end()) {
                cout << "Booking modified successfully to event ID: " << newEventID << endl;
            } else {
                cout << "Booking not found.\n";
            }
        }

        static void cancelBooking(int bookingID) {
            auto it = remove_if(bookings.begin(), bookings.end(), [bookingID](const Booking &b) {
                return b.bookingID == bookingID;
            });
            if (it != bookings.end()) {
                bookings.erase(it, bookings.end());
                cout << "Booking canceled successfully.\n";
            } else {
                cout << "Booking not found.\n";
            }
        }

        static bool hasBookedEvent(int userID) {
            return any_of(bookings.begin(), bookings.end(),
                          [userID](const Booking &b) {
                              return b.userID == userID;
                          });
        }

        static const vector<Booking>& getAllBookings() {
            return bookings; // œ«·… ·≈—Ã«⁄ Ã„Ì⁄ «·ÕÃÊ“« 
        }
    };

    static void addEvent(const string &eventName, const string &eventType, const string &eventDate, const string &eventLocation) {
        events.push_back(Event(eventName, eventType, eventDate, eventLocation));
        cout << "Event added successfully.\n";
    }

    static void deleteEvent(int eventID) {
        auto it = remove_if(events.begin(), events.end(), [eventID](Event &e) { return e.eventID == eventID; });
        if (it != events.end()) {
            events.erase(it, events.end());
            cout << "Event deleted successfully.\n";
        } else {
            cout << "Event not found.\n";
        }
    }

    static void viewEvents() {
        cout << "--- Events ---\n";
        for (const Event &event : events) {
            cout << "ID: " << event.eventID << ", Name: " << event.name
                 << ", Type: " << event.type << ", Date: " << event.date
                 << ", Location: " << event.location << endl;
        }
    }

    static Event* findEventByID(int eventID) {
        auto it = find_if(events.begin(), events.end(), [eventID](Event &e) { return e.eventID == eventID; });
        return (it != events.end()) ? &(*it) : nullptr;
    }

    void rateAndComment(int userID, int rating, const string &comment) {
        try {
            if (!Booking::hasBookedEvent(userID)) {
                throw invalid_argument("You must book this event before rating or commenting.");
            }
            eventRatings.addRating(rating, comment, getUsernameByID(userID)); // Add username
            cout << "Your rating and comment have been successfully added!\n";
        } catch (const invalid_argument &e) {
            cout << "Error: " << e.what() << endl; // Print error message
        } catch (const exception &e) {
            cout << "An unexpected error occurred: " << e.what() << endl;
        }
    }

    void displayComments() {
        eventRatings.displayRatingsAndComments();
    }

    static string getUsernameByID(int userID) {
        return "User" + to_string(userID); // «” »œ«· Â–« »«·„‰ÿﬁ «·›⁄·Ì ··Õ’Ê· ⁄·Ï «”„ «·„” Œœ„
    }

    Rating& getEventRatings() {
        return eventRatings; // œ«·… ··Ê’Ê· ≈·Ï  ﬁÌÌ„«  Ê ⁄·Ìﬁ«  «·„” Œœ„Ì‰
    }

    // œÊ«· «·Ê’Ê· ·„⁄·Ê„«  «·ÕœÀ
    string getName() const { return name; }
    string getType() const { return type; }
    string getDate() const { return date; }
    string getLocation() const { return location; }

    static void saveEventsToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            throw runtime_error("Error opening file for saving events.");
        }

        for (const Event& event : events) {
            outFile << event.eventID << "," << event.name << "," << event.type << ","
                    << event.date << "," << event.location << endl;
        }
        outFile.close();
        cout << "Events saved successfully to " << filename << ".\n";
    }

    static void loadEventsFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            ofstream outFile(filename);
            if (!outFile) {
                throw runtime_error("Error creating new file for events.");
            }
            cout << "No events found. A new file has been created: " << filename << ".\n";
            outFile.close();
            return;
        }

        events.clear(); // Clear current events
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string name, type, date, location;
            int id;

            getline(ss, line, ',');
            id = stoi(line);
            getline(ss, name, ',');
            getline(ss, type, ',');
            getline(ss, date, ',');
            getline(ss, location, ',');

            Event newEvent(name, type, date, location);
            newEvent.eventID = id; // Assign event ID
            events.push_back(newEvent);
        }
        inFile.close();
        cout << "Events loaded successfully from " << filename << ".\n";
    }
};

vector<Event> Event::events;
int Event::nextEventID = 1;
vector<Event::Booking> Event::Booking::bookings;
int Event::Booking::nextBookingID = 1;

// --- Admin Menu ---
void Admin::adminMenu() {
    int choice;
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Event\n";
        cout << "2. Delete Event\n";
        cout << "3. View Events\n";
        cout << "4. View User Ratings and Comments\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, type, date, location;
                cout << "Enter event name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter event type: ";
                getline(cin, type);
                cout << "Enter event date: ";
                getline(cin, date);
                cout << "Enter event location: ";
                getline(cin, location);
                Event::addEvent(name, type, date, location);
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
            case 4: {
                int eventID;
                cout << "Enter event ID to view ratings and comments: ";
                cin >> eventID;
                viewUserRatings(eventID);
                break;
            }
            case 0:
                return;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}

// --- View User Ratings and Comments ---
void Admin::viewUserRatings(int eventID) {
    Event* event = Event::findEventByID(eventID);
    if (event) {
        const Rating& ratings = event->getEventRatings(); // «” Œœ«„ «·œ«·… «·⁄«„…
        ratings.displayRatingsAndComments();
        int commentIndex;
        cout << "Enter index to delete (0 to skip): ";
        cin >> commentIndex;
        if (commentIndex > 0) {
            deleteUserComment(eventID, commentIndex);
        }
    } else {
        cout << "Event not found.\n";
    }
}

// --- Delete User Comment ---
void Admin::deleteUserComment(int eventID, int commentIndex) {
    Event* event = Event::findEventByID(eventID);
    if (event) {
        try {
            event->getEventRatings().deleteRatingComment(commentIndex); // «” Œœ«„ «·œ«·… «·⁄«„…
            cout << "Comment deleted successfully.\n";
        } catch (const invalid_argument &e) {
            cout << "Error: " << e.what() << endl;
        }
    } else {
        cout << "Event not found.\n";
    }
}

// --- User Menu ---
void User::userMenu() {
    int choice;
    while (true) {
        cout << "\n--- User Menu ---\n";
        cout << "1. View Events\n";
        cout << "2. Book Event\n";
        cout << "3. View Your Bookings\n"; // ≈÷«›… ŒÌ«— ⁄—÷ «·ÕÃÊ“« 
        cout << "4. Modify Booking\n";
        cout << "5. Cancel Booking\n";
        cout << "6. Rate and Comment on Event\n";
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
                    Event::Booking::bookEvent(getUserID(), *event);
                } else {
                    cout << "Event not found.\n";
                }
                break;
            }
            case 3:
                viewBookings(); // «” œ⁄«¡ œ«·… ⁄—÷ «·ÕÃÊ“« 
                break;
            case 4: {
                int bookingID, newEventID;
                cout << "Enter booking ID to modify: ";
                cin >> bookingID;
                cout << "Enter new event ID: ";
                cin >> newEventID;
                Event::Booking::modifyBooking(bookingID, newEventID);
                break;
            }
            case 5: {
                int bookingID;
                cout << "Enter booking ID to cancel: ";
                cin >> bookingID;
                Event::Booking::cancelBooking(bookingID);
                break;
            }
            case 6: {
                int eventID, rating;
                string comment;
                cout << "Enter event ID to rate: ";
                cin >> eventID;
                cout << "Enter your rating (1-5): ";
                cin >> rating;
                cin.ignore();
                cout << "Enter your comment (optional): ";
                getline(cin, comment);

                Event* event = Event::findEventByID(eventID);
                if (event) {
                    event->rateAndComment(getUserID(), rating, comment);
                } else {
                    cout << "Event not found.\n";
                }
                break;
            }
            case 0:
                return;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}

// --- ⁄—÷ «·ÕÃÊ“«  ---
void User::viewBookings() const {
    cout << "--- Your Bookings ---\n";
    for (const auto& booking : Event::Booking::getAllBookings()) {
        if (booking.getUserID() == userID) { // «” Œœ«„ œ«·… «·Ê’Ê·
            Event* event = Event::findEventByID(booking.getEventID());
            if (event) {
                cout << "Booking ID: " << booking.getBookingID() // «” Œœ«„ œ«·… «·Ê’Ê·
                     << ", Event ID: " << booking.getEventID() // „⁄—› «·ÕœÀ
                     << ", Event Name: " << event->getName() // «”„ «·ÕœÀ
                     << ", Event Type: " << event->getType() // ‰Ê⁄ «·ÕœÀ
                     << ", Event Date: " << event->getDate() //  «—ÌŒ «·ÕœÀ
                     << ", Event Location: " << event->getLocation() << endl; // „Êﬁ⁄ «·ÕœÀ
            } else {
                cout << "Event not found for booking ID: " << booking.getBookingID() << endl;
            }
        }
    }
}

// --- Main Function ---
int main() {
    Admin admin("admin", "1234");
    int choice;

    // Load events from file at startup
    try {
        Event::loadEventsFromFile("events.txt");
    } catch (const exception &e) {
        cout << "Error loading events: " << e.what() << endl;
    }

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
                static int userID = 1; // Example user ID
                User user("placeholder", "placeholder", userID);
                if (user.login()) {
                    cout << "User logged in successfully!\n";
                    user.userMenu();
                } else {
                    cout << "Invalid user credentials.\n";
                    }
                break;
            }
            case 3: {
                static int userID = 1; // Example user ID
                try {
                    User::createUser(userID++);
                } catch (const exception &e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 0:
                cout << "Exiting the program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }

    // Save events to file on exit
    try {
        Event::saveEventsToFile("events.txt");
    } catch (const exception &e) {
        cout << "Error saving events: " << e.what() << endl;
    }

    return 0;
}
