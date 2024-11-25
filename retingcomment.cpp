#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*
 * كلاس التقييم:
 * يُستخدم هذا الكلاس لإدارة التقييمات والتعليقات التي يقدمها المستخدمون.
 * يحتوي على قائمة من التقييمات، كل منها مرتبط باسم المستخدم والتعليق.
 */
class Rating {
private:
    // هيكل لتخزين بيانات التقييم
    struct RatingComment {
        int rating;       // التقييم (من 1 إلى 5)
        string comment;   // التعليق الذي يقدمه المستخدم
        string username;  // اسم المستخدم الذي قدم التقييم
    };

    vector<RatingComment> ratings; // قائمة التقييمات والتعليقات

public:
    /*
     * إضافة تقييم جديد:
     * - username: اسم المستخدم
     * - rating: التقييم (رقم بين 1 و 5)
     * - comment: تعليق المستخدم
     */
    void addRating(const string& username, int rating, const string& comment) {
        if (rating < 1 || rating > 5) { // التأكد من أن التقييم ضمن النطاق الصحيح
            cout << "Error: Rating must be between 1 and 5.\n"; // رسالة خطأ باللغة الإنجليزية
            return;
        }
        // إضافة التقييم الجديد إلى القائمة
        ratings.push_back({rating, comment, username});
        cout << "Rating added successfully!\n"; // رسالة نجاح باللغة الإنجليزية
    }

    /*
     * عرض جميع التقييمات والتعليقات:
     * يعرض اسم المستخدم، التقييم، والتعليق لكل إدخال في القائمة.
     */
    void displayRatingsAndComments() const {
        if (ratings.empty()) { // إذا كانت القائمة فارغة
            cout << "There are no ratings currently.\n"; // رسالة باللغة الإنجليزية إذا كانت القائمة فارغة
            return;
        }

        cout << "List of ratings and comments:\n"; // عنوان القائمة باللغة الإنجليزية
        for (const auto& entry : ratings) {
            cout << "Username: " << entry.username
                 << ", Rating: " << entry.rating
                 << ", Comment: " << entry.comment << endl;
        }
    }

    /*
     * حذف التقييم باستخدام اسم المستخدم:
     * - إذا تم العثور على التقييم باستخدام اسم المستخدم، سيتم حذفه.
     * - إذا لم يتم العثور على التقييم، سيتم عرض رسالة خطأ.
     */
    void deleteRating(const string& username) {
        // البحث عن التقييم باستخدام اسم المستخدم
        auto it = find_if(ratings.begin(), ratings.end(),
                          [&username](const RatingComment& entry) {
                              return entry.username == username;
                          });

        if (it != ratings.end()) { // إذا تم العثور على التقييم
            ratings.erase(it); // حذف التقييم
            cout << "Rating deleted successfully!\n"; // رسالة نجاح باللغة الإنجليزية
        } else { // إذا لم يتم العثور على التقييم
            cout << "No rating found for the username: " << username << ".\n"; // رسالة خطأ باللغة الإنجليزية
        }
    }
};
