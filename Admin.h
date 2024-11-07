// كلاس يمثل المدير، يرث من الكلاس Person
class Admin : public Person {
public:
    // المُنشئ لتهيئة اسم المستخدم وكلمة المرور
    Admin(string user, string pass) : Person(user, pass) {}

    // تطبيق دالة تسجيل الدخول
    bool login() override {
        string enteredUsername, enteredPassword;
        cout << "Enter admin username: ";
        cin >> enteredUsername;
        cout << "Enter admin password: ";
        cin >> enteredPassword;

        // مقارنة بيانات الاعتماد المدخلة مع بيانات الاعتماد المخزنة
        return (enteredUsername == username && enteredPassword == password);
    }

    // دالة لعرض قائمة خيارات المدير
    void adminMenu();
};
كلاس admin
