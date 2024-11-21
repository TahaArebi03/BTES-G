// كلاس يمثل مقعدًا في حدث
class Seat {
    int seatNumber;   // رقم المقعد
    bool isReserved;  // حالة المقعد (محجوز أو متاح)

public:
    // المُنشئ لتهيئة رقم المقعد
    Seat(int number) : seatNumber(number), isReserved(false) {}

    // دالة للحصول على رقم المقعد
    int getSeatNumber() const { return seatNumber; }

    // دالة للتحقق مما إذا كان المقعد متاحًا
    bool checkAvailability() const { return !isReserved; }

    // دالة لحجز المقعد
    void reserve() {
        if (isReserved) throw runtime_error("Seat already reserved."); // إذا كان محجوزًا بالفعل
        isReserved = true; // تغيير الحالة إلى محجوز
    }
}; كلاس خاص بالseat
