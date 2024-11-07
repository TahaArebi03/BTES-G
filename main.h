int main() {
    vector<Person*> people;

    // عينة لمدير (لأغراض الاختبار)
    people.push_back(new Admin("admin", "admin123"));

    int mainChoice;

    while (true) {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Admin Login\n";
        cout << "2. User Login\n";
        cout << "3. Create User Account\n"; // متاح فقط هنا
        cout << "0. Exit\n";
        cout << "Select an option: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1: {
                bool loggedIn = false;
                for (Person* person : people) {
                    if (dynamic_cast<Admin*>(person) && person->login()) {
                        loggedIn = true;
                        dynamic_cast<Admin*>(person)->adminMenu(); // عرض قائمة المدير
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
                        dynamic_cast<User*>(person)->userMenu(); // عرض قائمة المستخدم
                        break;
                    }
                }
                if (!loggedIn) cout << "Invalid login.\n";
                break;
            }
            case 3: { // إنشاء حساب مستخدم
                static int nextUserID = 1; // متغير ثابت لمعرف المستخدم
                User newUser = User::createUser(nextUserID++);
                User::saveUserToFile(newUser); // حفظ المستخدم الجديد في الملف
                people.push_back(new User(newUser.getUsername(), newUser.getPassword(), newUser.getUserID())); // إضافة المستخدم إلى القائمة
                cout << "Account created successfully.\n";
                break;
            }
            case 0:
                cout << "Exiting the system. Goodbye!\n";
                return 0; // إنهاء البرنامج
            default:
                cout << "Invalid choice.\n"; // إدخال غير صالح
        }
    }
}
