#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "testpayment.h" // ÊÃßÏ ãä ÊÖãíä ãáİ ÇáÑÃÓ ÇáÎÇÕ ÈİÆÇÊ ÇáÏİÚ

using namespace std;

// ÇÎÊÈÇÑ ÇáÏİÚ ÈÇÓÊÎÏÇã ÈØÇŞÉ ÇáÇÆÊãÇä
TEST_CASE("CreditCard Payment") {
    CreditCard cc;
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf()); // ÊæÌíå ÇáÅÎÑÇÌ

    cc.pay(100.0); // ÏİÚ 100 ÏæáÇÑ

    cout.rdbuf(oldCout); // ÇÓÊÑÌÇÚ ÅÎÑÇÌ cout
    REQUIRE(buffer.str() == "Paid $100 using Credit Card.\n");
}

// ÇÎÊÈÇÑ ÇáÏİÚ ÈÇÓÊÎÏÇã ÈÇí ÈÇá
TEST_CASE("PayPal Payment") {
    PayPal pp;
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());

    pp.pay(50.0); // ÏİÚ 50 ÏæáÇÑ

    cout.rdbuf(oldCout);
    REQUIRE(buffer.str() == "Paid $50 using PayPal.\n");
}

// ÇÎÊÈÇÑ ÇáÏİÚ ÈÇÓÊÎÏÇã ÊÍæíá Èäßí
TEST_CASE("BankTransfer Payment") {
    BankTransfer bt;
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());

    bt.pay(201.0); // ÏİÚ 200 ÏæáÇÑ

    cout.rdbuf(oldCout);
    REQUIRE(buffer.str() == "Paid $200 using Bank Transfer.\n");
}


//
