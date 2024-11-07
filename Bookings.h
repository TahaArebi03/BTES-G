#include <vector>
#include <map>

// كلاس يمثل حجزًا قام به مستخدم لحدث معين
class Booking {
private:
    int userID;                // معرف المستخدم الذي قام بالحجز
    int eventID;               // معرف الحدث الذي تم حجزه
    vector<int> reservedSeats;  // قائمة المقاعد المحجوزة

public:
    // المُنشئ لتهيئة جميع الخصائص
    Booking(int userID, int eventID, const vector<int>& seats)
        : userID(userID), eventID(eventID), reservedSeats(seats) {}

    // دالة للحصول على معرف المستخدم
    int getUserID() const { return userID; }

    // دالة للحصول على معرف الحدث
    int getEventID() const { return eventID; }

    // دالة للحصول على قائمة المقاعد المحجوزة
    const vector<int>& getReservedSeats() const { return reservedSeats; }

    // دالة لإلغاء الحجز
    void cancelBooking();

    // دالة لتعديل الحجز
    void modifyBooking(const vector<int>& newSeats);

    // دالة static لعرض جميع الحجوزات للمستخدم
    static void viewBookings(int userID);
};

// خريطة لتخزين حجوزات كل مستخدم
map<int, vector<Booking>> userBookings;

// تعريف دالة إلغاء الحجز
void Booking::cancelBooking() {
    auto& bookings = userBookings[userID];
    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        if (it->getEventID() == eventID) {
            bookings.erase(it); // حذف الحجز
            cout << "Booking cancelled successfully for event ID: " << eventID << endl;
            return;
        }
    }
    throw runtime_error("No booking found to cancel."); // إذا لم يتم العثور على حجز
}

