#include <iostream>
#include <string>

using namespace std;

// كلاس يمثل شخصًا (مدير أو مستخدم)
class Person {
protected:
    string username;  // اسم المستخدم
    string password;  // كلمة المرور

public:
    // المُنشئ لتهيئة اسم المستخدم وكلمة المرور
    Person(string user, string pass) : username(user), password(pass) {}

    // دالة افتراضية نقية لتسجيل الدخول، يجب تنفيذها في الكلاسات المشتقة
    virtual bool login() = 0;

    // دالة للحصول على اسم المستخدم
    string getUsername() const { return username; }
    
    // دالة للحصول على كلمة المرور
    string getPassword() const { return password; }
};
/*
هذا الكلاس يمثل شخصًا (إما مدير أو مستخدم) ويحتوي على الخصائص الأساسية مثل اسم المستخدم وكلمة المرور.

الخصائص:
username: اسم المستخدم.
password: كلمة المرور.
الوظائف:
المُنشئ: يقوم بتهيئة username و password.
login(): دالة افتراضية نقية، يجب تنفيذها في الكلاسات المشتقة.
getUsername(): تعيد اسم المستخدم.
getPassword(): تعيد كلمة المرور.
  */
