#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "testpayment.h" // ���� �� ����� ��� ����� ����� ����� �����

using namespace std;

// ������ ����� �������� ����� ��������
TEST_CASE("CreditCard Payment") {
    CreditCard cc;
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf()); // ����� �������

    cc.pay(100.0); // ��� 100 �����

    cout.rdbuf(oldCout); // ������� ����� cout
    REQUIRE(buffer.str() == "Paid $100 using Credit Card.\n");
}

// ������ ����� �������� ��� ���
TEST_CASE("PayPal Payment") {
    PayPal pp;
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());

    pp.pay(50.0); // ��� 50 �����

    cout.rdbuf(oldCout);
    REQUIRE(buffer.str() == "Paid $50 using PayPal.\n");
}

// ������ ����� �������� ����� ����
TEST_CASE("BankTransfer Payment") {
    BankTransfer bt;
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());

    bt.pay(201.0); // ��� 200 �����

    cout.rdbuf(oldCout);
    REQUIRE(buffer.str() == "Paid $200 using Bank Transfer.\n");
}


//
