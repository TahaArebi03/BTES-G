#include <map>
#include <stdexcept>
#include <vector>

// كلاس يمثل حدثًا ويحتوي على معلومات حوله
class Event {
    int eventID;                // معرف الحدث
    string name;                // اسم الحدث
    string type;                // نوع الحدث
    string date;                // تاريخ الحدث
    string location;            // موقع الحدث
    vector<Seat> seats;         // قائمة المقاعد المرتبطة بالحدث
    static vector<Event> events; // قائمة جميع الأحداث
    static int nextEventID;     // معرف الحدث التالي

public:
    // المُنشئ لتهيئة جميع الخصائص بناءً على المدخلات
    Event(const string& eventName, const string& eventType, const string& eventDate, const string& eventLocation, int seatCount);

    // دالة static لإضافة حدث جديد
    static void addEvent(const string& eventName, const string& eventType, const string& eventDate, const string& eventLocation, int seatCount);

    // دالة static لحذف حدث بناءً على معرفه
    static void deleteEvent(int eventID);

    // دالة static لعرض جميع الأحداث
    static void viewEvents();

    // دالة static للحصول على قائمة الأحداث
    static const vector<Event>& getEvents();

    // دالة static للبحث عن حدث بناءً على معرفه
    static Event* findEventByID(int eventID);
    
    // دالة للحصول على معرف الحدث
    int getEventID() const { return eventID; }

    // دالة للحصول على اسم الحدث
    string getName() const;

    // دالة للحصول على موقع الحدث
    string getLocation() const { return location; }

    // دالة للحصول على عدد المقاعد المتاحة
    int getAvailableSeats() const;

    // دالة لحجز مقعد معين
    void reserveSeat(int seatNumber);

    // دالة لعرض حالة المقاعد
    void displaySeats() const;

    // دالة static لحفظ معلومات الأحداث في ملف
    static void saveToFile(const string& filename);
};

// تعريف المتغيرات static
vector<Event> Event::events;
int Event::nextEventID = 1;

// تعريف المُنشئ
Event::Event(const string& eventName, const string& eventType, const string& eventDate, const string& eventLocation, int seatCount)
    : eventID(nextEventID++), name(eventName), type(eventType), date(eventDate), location(eventLocation) {
    for (int i = 1; i <= seatCount; ++i) {
        seats.push_back(Seat(i)); // إضافة المقاعد
    }
}

// باقي الدوال الخاصة بكلاس Event يجب تعريفها
