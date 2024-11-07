#include <fstream>
#include <vector>

// كلاس يمثل المستخدم، يرث من الكلاس Person
class User : public Person {
    int userID; // معرف فريد للمستخدم

public:
    // المُنشئ لتهيئة اسم المستخدم وكلمة المرور ومعرف المستخدم
    User(string user, string pass, int id) : Person(user, pass), userID(id) {}

    // دالة للحصول على معرف المستخدم
    int getUserID() const { return userID; }

    // تطبيق دالة تسجيل الدخول
    bool login() override {
        string enteredUsername, enteredPassword;
        cout << "Enter username: ";
        cin >> enteredUsername;
        cout << "Enter password: ";
        cin >> enteredPassword;

        // التحقق من بيانات الاعتماد من ملف المستخدمين
        ifstream inFile("users.txt");
        string user, pass;
        while (inFile >> user >> pass) {
            // إذا كانت بيانات الاعتماد صحيحة، تعيد true
            if (user == enteredUsername && pass == enteredPassword) {
                return true;
            }
        }
        return false; // بيانات اعتماد غير صحيحة
    }

    // دالة لعرض قائمة خيارات المستخدم
    void userMenu(); // يجب تنفيذها لاحقًا

    // دالة ثابتة لإنشاء مستخدم جديد
    static User createUser(int id);

    // دالة ثابتة لحفظ معلومات المستخدم في ملف
    static void saveUserToFile(const User& user);
}; كلاس user
