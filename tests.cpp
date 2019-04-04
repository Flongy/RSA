//
// Created by Максим on 04.04.2019.
//

#include <iostream>
#include "BigInt.h"
#include "tests.h"

using namespace std;

void test_BigInt() {
    BigInt testBI;
    BigInt testBIRight;


    // test checkString()
    if (BigInt::checkString("123456") != true)
        cout << "BigInt::checkString() не прошел тест на значении 123456" << endl << "Результат checkString: " << BigInt::checkString("123456") << endl;
    if (BigInt::checkString("abc") != false)
        cout << "BigInt::checkString() не прошел тест на значении abc" << endl << "Результат checkString: " << BigInt::checkString("abc") << endl;
    if (BigInt::checkString("123abc") != false)
        cout << "BigInt::checkString() не прошел тест на значении 123abc" << endl << "Результат checkString: " << BigInt::checkString("123abc") << endl;
    // TODO: сделать знаковый BigInt?
    if (BigInt::checkString("-10") != false)
        cout << "BigInt::checkString() не прошел тест на значении -10" << endl << "Результат checkString: " << BigInt::checkString("-10") << endl;


    //test setValue() и toString()
    testBI.setValue("0");
    if(testBI.toString().compare("0") != 0)
        cout << "BigInt::setValue() или toString() не прошел тест на значении 0" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("1234");
    if(testBI.toString().compare("1234") != 0)
        cout << "BigInt::setValue() или toString() не прошел тест на значении 1234" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("123456789123456789123456789");
    if(testBI.toString().compare("123456789123456789123456789") != 0)
        cout << "BigInt::setValue() или toString() не прошел тест на значении 123456789123456789123456789" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("10000200003000040000");
    if(testBI.toString().compare("10000200003000040000") != 0)
        cout << "BigInt::setValue() или toString() не прошел тест на значении 10000200003000040000" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("10000");
    if(testBI.toString().compare("10000") != 0)
        cout << "BigInt::setValue() или toString() не прошел тест на значении 10000" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("10100");
    if(testBI.toString().compare("10100") != 0)
        cout << "BigInt::setValue() или toString() не прошел тест на значении 10100" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("5 0000");
    if(testBI.toString().compare("50000") != 0)
        cout << "BigInt::setValue() или toString() не прошел тест на значении 5 0000" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("5,000");
    if(testBI.toString().compare("5000") != 0)
        cout << "BigInt::setValue() или toString() не прошел тест на значении 5,000" << endl << "BigInt = "<< testBI.toString() << endl;

    /* TODO: сделать знаковый BigInt?
     * testBI.setValue("-10");
    if(testBI.toString().compare("-10") != 0) {
        cout<<"BigInt::setValue() не прошел тест"<<endl;
    }*/


    //test append()
    testBI.setValue(100);
    testBI.appendValue(1);
    if(testBI.toString().compare("10100") != 0)
        cout << "BigInt::appendValue() не прошел тест на значениях 100, 1" << endl << "BigInt = " << testBI.toString() << endl;

    testBI.setValue(1000);
    testBI.appendValue(101000);
    if(testBI.toString().compare("1010001000") != 0)
        cout << "BigInt::appendValue() не прошел тест на значениях 1000, 101000" << endl << "BigInt = " << testBI.toString() << endl;


    //test operator+=
    testBI.setValue(100);
    testBIRight.setValue(100);
    testBI += testBIRight;
    if(testBI.toString().compare("200") != 0)
        cout << "BigInt::operator+=() не прошел тест на значениях 100 += 100" << endl << "BigInt = " << testBI.toString() << endl;

    testBI.setValue(9000);
    testBIRight.setValue(1000);
    testBI += testBIRight;
    if(testBI.toString().compare("10000") != 0 || testBI.getSize() != 2)
        cout << "BigInt::operator+=() не прошел тест на значениях 9000 += 1000" << endl << "BigInt = " << testBI.toString() << endl;

    testBI.setValue(99999999);
    testBIRight.setValue(99999999);
    testBI += testBIRight;
    if(testBI.toString().compare("199999998") != 0 || testBI.getSize() != 3)
        cout << "BigInt::operator+=() не прошел тест на значениях 99999999 += 99999999" << endl << "BigInt = " << testBI.toString() << endl;

    testBI.setValue(50000);
    testBIRight.setValue(6700);
    testBI += testBIRight;
    if(testBI.toString().compare("56700") != 0 || testBI.getSize() != 2)
        cout << "BigInt::operator+=() не прошел тест на значениях 50000 += 6700" << endl << "BigInt = " << testBI.toString() << endl;


    //test operator+
    testBI.setValue(120);
    testBIRight.setValue(640);
    if((testBI + testBIRight).toString().compare("760") != 0 && testBI.toString().compare("120") && testBIRight.toString().compare("640"))
        cout << "BigInt::operator+() не прошел тест на значениях 120 + 640" << endl << "BigInt = " << (testBI + testBIRight).toString() << endl;


    //test operator==
    testBI.setValue(100);
    testBIRight.setValue(100);
    if(!(testBI == testBIRight))
        cout << "BigInt::operator==() не прошел тест на значениях 100 == 100" << endl << "Результат: " << (testBI == testBIRight) << endl;

    testBI.setValue(100);
    testBIRight.setValue(200);
    if((testBI == testBIRight))
        cout << "BigInt::operator==() не прошел тест на значениях 100 == 200" << endl << "Результат: " << (testBI == testBIRight) << endl;

    testBI.setValue(200);
    testBIRight.setValue(100);
    if((testBI == testBIRight))
        cout << "BigInt::operator==() не прошел тест на значениях 200 == 100" << endl << "Результат: " << (testBI == testBIRight) << endl;

    testBI.setValue(9000);
    testBIRight.setValue(10000);
    if((testBI == testBIRight))
        cout << "BigInt::operator==() не прошел тест на значениях 9000 == 10000" << endl << "Результат: " << (testBI == testBIRight) << endl;

    testBI.setValue(10000);
    testBIRight.setValue(9000);
    if((testBI == testBIRight))
        cout << "BigInt::operator==() не прошел тест на значениях 10000 == 9000" << endl << "Результат: " << (testBI == testBIRight) << endl;


    //test operator!=
    testBI.setValue(100);
    testBIRight.setValue(100);
    if((testBI != testBIRight))
        cout << "BigInt::operator!=() не прошел тест на значениях 100 != 100" << endl << "Результат: " << (testBI != testBIRight) << endl;

    testBI.setValue(100);
    testBIRight.setValue(200);
    if(!(testBI != testBIRight))
        cout << "BigInt::operator!=() не прошел тест на значениях 100 != 200" << endl << "Результат: " << (testBI != testBIRight) << endl;

    testBI.setValue(200);
    testBIRight.setValue(100);
    if(!(testBI != testBIRight))
        cout << "BigInt::operator!=() не прошел тест на значениях 200 != 100" << endl << "Результат: " << (testBI != testBIRight) << endl;

    testBI.setValue(9000);
    testBIRight.setValue(10000);
    if(!(testBI != testBIRight))
        cout << "BigInt::operator!=() не прошел тест на значениях 9000 != 10000" << endl << "Результат: " << (testBI != testBIRight) << endl;

    testBI.setValue(10000);
    testBIRight.setValue(9000);
    if(!(testBI != testBIRight))
        cout << "BigInt::operator!=() не прошел тест на значениях 10000 != 9000" << endl << "Результат: " << (testBI != testBIRight) << endl;


    //test operator>
    testBI.setValue(100);
    testBIRight.setValue(100);
    if((testBI > testBIRight))
        cout << "BigInt::operator>() не прошел тест на значениях 100 > 100" << endl << "Результат: " << (testBI > testBIRight) << endl;

    testBI.setValue(100);
    testBIRight.setValue(200);
    if((testBI > testBIRight))
        cout << "BigInt::operator>() не прошел тест на значениях 100 > 200" << endl << "Результат: " << (testBI > testBIRight) << endl;

    testBI.setValue(200);
    testBIRight.setValue(100);
    if(!(testBI > testBIRight))
        cout << "BigInt::operator>() не прошел тест на значениях 200 > 100" << endl << "Результат: " << (testBI > testBIRight) << endl;

    testBI.setValue(9000);
    testBIRight.setValue(10000);
    if((testBI > testBIRight))
        cout << "BigInt::operator>() не прошел тест на значениях 9000 > 10000" << endl << "Результат: " << (testBI > testBIRight) << endl;

    testBI.setValue(10000);
    testBIRight.setValue(9000);
    if(!(testBI > testBIRight))
        cout << "BigInt::operator>() не прошел тест на значениях 10000 > 9000" << endl << "Результат: " << (testBI > testBIRight) << endl;


    //test operator<
    testBI.setValue(100);
    testBIRight.setValue(100);
    if((testBI < testBIRight))
        cout << "BigInt::operator<() не прошел тест на значениях 100 < 100" << endl << "Результат: " << (testBI < testBIRight) << endl;

    testBI.setValue(100);
    testBIRight.setValue(200);
    if(!(testBI < testBIRight))
        cout << "BigInt::operator<() не прошел тест на значениях 100 < 200" << endl << "Результат: " << (testBI < testBIRight) << endl;

    testBI.setValue(200);
    testBIRight.setValue(100);
    if((testBI < testBIRight))
        cout << "BigInt::operator<() не прошел тест на значениях 200 < 100" << endl << "Результат: " << (testBI < testBIRight) << endl;

    testBI.setValue(9000);
    testBIRight.setValue(10000);
    if(!(testBI < testBIRight))
        cout << "BigInt::operator<() не прошел тест на значениях 9000 < 10000" << endl << "Результат: " << (testBI < testBIRight) << endl;

    testBI.setValue(10000);
    testBIRight.setValue(9000);
    if((testBI < testBIRight))
        cout << "BigInt::operator<() не прошел тест на значениях 10000 < 9000" << endl << "Результат: " << (testBI < testBIRight) << endl;


    //test operator>=
    testBI.setValue(100);
    testBIRight.setValue(100);
    if(!(testBI >= testBIRight))
        cout << "BigInt::operator>=() не прошел тест на значениях 100 >= 100" << endl << "Результат: " << (testBI >= testBIRight) << endl;

    testBI.setValue(100);
    testBIRight.setValue(200);
    if((testBI >= testBIRight))
        cout << "BigInt::operator>=() не прошел тест на значениях 100 >= 200" << endl << "Результат: " << (testBI >= testBIRight) << endl;

    testBI.setValue(200);
    testBIRight.setValue(100);
    if(!(testBI >= testBIRight))
        cout << "BigInt::operator>=() не прошел тест на значениях 200 >= 100" << endl << "Результат: " << (testBI >= testBIRight) << endl;

    testBI.setValue(9000);
    testBIRight.setValue(10000);
    if((testBI >= testBIRight))
        cout << "BigInt::operator>=() не прошел тест на значениях 9000 >= 10000" << endl << "Результат: " << (testBI >= testBIRight) << endl;

    testBI.setValue(10000);
    testBIRight.setValue(9000);
    if(!(testBI >= testBIRight))
        cout << "BigInt::operator>=() не прошел тест на значениях 10000 >= 9000" << endl << "Результат: " << (testBI >= testBIRight) << endl;


    //test operator<=
    testBI.setValue(100);
    testBIRight.setValue(100);
    if(!(testBI <= testBIRight))
        cout << "BigInt::operator<=() не прошел тест на значениях 100 <= 100" << endl << "Результат: " << (testBI <= testBIRight) << endl;

    testBI.setValue(100);
    testBIRight.setValue(200);
    if(!(testBI <= testBIRight))
        cout << "BigInt::operator<=() не прошел тест на значениях 100 <= 200" << endl << "Результат: " << (testBI <= testBIRight) << endl;

    testBI.setValue(200);
    testBIRight.setValue(100);
    if((testBI <= testBIRight))
        cout << "BigInt::operator<=() не прошел тест на значениях 200 <= 100" << endl << "Результат: " << (testBI <= testBIRight) << endl;

    testBI.setValue(9000);
    testBIRight.setValue(10000);
    if(!(testBI <= testBIRight))
        cout << "BigInt::operator<=() не прошел тест на значениях 9000 <= 10000" << endl << "Результат: " << (testBI <= testBIRight) << endl;

    testBI.setValue(10000);
    testBIRight.setValue(9000);
    if((testBI <= testBIRight))
        cout << "BigInt::operator<=() не прошел тест на значениях 10000 <= 9000" << endl << "Результат: " << (testBI <= testBIRight) << endl;
}