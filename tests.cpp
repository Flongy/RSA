//
// Created by Максим on 04.04.2019.
//

#include <iostream>
#include "BigInt.h"
#include "tests.h"

using namespace std;

void test_BigInt() {
    // test checkString()
    if (BigInt::checkString("123456") != true ||
            BigInt::checkString("abc") != false ||
            BigInt::checkString("123abc") != false)
        cout<<"BigInt::checkString() не прошел тесты"<<endl;

    //test setValue()
    BigInt testBI;
    testBI.setValue("1234");
    if(testBI.toString().compare("1234") != 0) {
        cout<<"BigInt::setValue() не прошел тест"<<endl;
    }
    testBI.setValue("123456789123456789123456789");
    if(testBI.toString().compare("123456789123456789123456789") != 0) {
        cout<<"BigInt::setValue() не прошел тест"<<endl;
    }
}