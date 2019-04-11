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
    if(testBI != "0")
        cout << "BigInt::setValue() или toString() не прошел тест на значении 0" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("1234");
    if(testBI != "1234")
        cout << "BigInt::setValue() или toString() не прошел тест на значении 1234" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("123456789123456789123456789");
    if(testBI != "123456789123456789123456789")
        cout << "BigInt::setValue() или toString() не прошел тест на значении 123456789123456789123456789" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("10000200003000040000");
    if(testBI != "10000200003000040000")
        cout << "BigInt::setValue() или toString() не прошел тест на значении 10000200003000040000" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("10000");
    if(testBI != "10000")
        cout << "BigInt::setValue() или toString() не прошел тест на значении 10000" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("10100");
    if(testBI != "10100")
        cout << "BigInt::setValue() или toString() не прошел тест на значении 10100" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("5 0000");
    if(testBI != "50000")
        cout << "BigInt::setValue() или toString() не прошел тест на значении 5 0000" << endl << "BigInt = "<< testBI.toString() << endl;

    testBI.setValue("5,000");
    if(testBI != "5000")
        cout << "BigInt::setValue() или toString() не прошел тест на значении 5,000" << endl << "BigInt = "<< testBI.toString() << endl;

    /* TODO: сделать знаковый BigInt?
     * testBI.setValue("-10");
    if(testBI.toString().compare("-10") != 0) {
        cout<<"BigInt::setValue() не прошел тест"<<endl;
    }*/


    //test appendValue()
    testBI.setValue(100);
    testBI.appendValue(1);
    if(testBI != "10100")
        cout << "BigInt::appendValue() не прошел тест на значениях 100, 1" << endl << "BigInt = " << testBI.toString() << endl;

    testBI.setValue(1000);
    testBI.appendValue(101000);
    if(testBI != "1010001000")
        cout << "BigInt::appendValue() не прошел тест на значениях 1000, 101000" << endl << "BigInt = " << testBI.toString() << endl;


    //test operator+=
    testBI.setValue(100);
    testBIRight.setValue(100);
    testBI += testBIRight;
    if(testBI != "200")
        cout << "BigInt::operator+=() не прошел тест на значениях 100 += 100" << endl << "BigInt = " << testBI.toString() << endl;

    testBI.setValue(9000);
    testBIRight.setValue(1000);
    testBI += testBIRight;
    if(testBI != "10000" || testBI.getSize() != 2)
        cout << "BigInt::operator+=() не прошел тест на значениях 9000 += 1000" << endl << "BigInt = " << testBI.toString() << endl;

    testBI.setValue(99999999);
    testBIRight.setValue(99999999);
    testBI += testBIRight;
    if(testBI != "199999998" || testBI.getSize() != 3)
        cout << "BigInt::operator+=() не прошел тест на значениях 99999999 += 99999999" << endl << "BigInt = " << testBI.toString() << endl;

    testBI.setValue(50000);
    testBIRight.setValue(6700);
    testBI += testBIRight;
    if(testBI != "56700" || testBI.getSize() != 2)
        cout << "BigInt::operator+=() не прошел тест на значениях 50000 += 6700" << endl << "BigInt = " << testBI.toString() << endl;


    //test operator+
    testBI.setValue(120);
    testBIRight.setValue(640);
    if((testBI + testBIRight) != "760" || testBI != "120" || testBIRight != "640")
        cout << "BigInt::operator+() не прошел тест на значениях 120 + 640" << endl << testBI << " + " << testBIRight <<  " = " << (testBI + testBIRight).toString() << endl;

    testBI.setValue(15000);
    testBIRight.setValue(5000);
    if((testBI + testBIRight) != 20000 || (testBI + testBIRight).getSize() != 2 || (testBI + testBIRight).getLength() != 5 || testBI != "15000" || testBIRight != "5000")
        cout << "BigInt::operator+() не прошел тест на значениях 15000 + 5000" << endl << testBI << " + " << testBIRight <<  " = " << (testBI + testBIRight).toString() << endl;

    testBI.setValue(5000);
    testBIRight.setValue(5000);
    if((testBI + testBIRight) != 10000 || (testBI + testBIRight).getSize() != 2 || (testBI + testBIRight).getLength() != 5 || testBI != "5000" || testBIRight != "5000")
        cout << "BigInt::operator+() не прошел тест на значениях 5000 + 5000" << endl << testBI << " + " << testBIRight <<  " = " << (testBI + testBIRight).toString() << endl;

    testBI.setValue(50);
    if((testBI + "50") != "100" || testBI != "50")
        cout << "BigInt::operator+() не прошел тест на значениях 50 + (string) 50" << endl << testBI << " + " << "50" <<  " = " << (testBI + "50").toString() << endl;

    testBI.setValue(50);
    if((testBI + 50) != 100 || testBI != "50")
        cout << "BigInt::operator+() не прошел тест на значениях 50 + (int) 50" << endl << testBI << " + " << 50 <<  " = " << (testBI + 50).toString() << endl;


    //test operator++
    testBI = 10;
    testBI++;
    if(testBI != 11)
        cout<< "BigInt::operator++(int) не прошел тест" << endl << "BigInt = " << testBI << endl;

    testBI = 10;
    ++testBI;
    if(testBI != 11)
        cout<< "BigInt::operator++() не прошел тест" << endl << "BigInt = " << testBI << endl;


    //test operator-=
    testBI = 100;
    testBIRight = 150;
    try {
        testBI -= testBIRight;
        cout << "BigInt::operator-=() выдал отрицательное число" << endl << testBI << endl;
    } catch (const char *str) {
        //cout << str << endl;
    }
    if (testBI != 100)
        cout << "BigInt::operator-=() не прошел тест на значениях 100 -= 150" << endl << "BigInt = " << testBI << endl;

    testBI = 10000;
    testBIRight = 15000;
    try {
        //cout<< "T: " << testBI << ' ' << testBIRight << endl;
        testBI -= testBIRight;
        cout << "BigInt::operator-=() выдал отрицательное число" << endl << testBI << endl;
    } catch (const char *str) {
        //cout << str << endl;
    }
    if (testBI != 10000)
        cout << "BigInt::operator-=() не прошел тест на значениях 10000 -= 15000" << endl << "BigInt = " << testBI << endl;

    testBI = 100;
    testBIRight = 50;
    testBI -= testBIRight;
    if (testBI != 50 || testBIRight != 50 || testBI.getLength() != 2)
        cout << "BigInt::operator-=() не прошел тест на значениях 100 -= 50" << endl << "BigInt = " << testBI << endl;

    testBI = 10000;
    testBIRight = 1000;
    testBI -= testBIRight;
    if (testBI != 9000 || testBIRight != 1000 || testBI.getLength() != 4 || testBI.getSize() != 1)
        cout << "BigInt::operator-=() не прошел тест на значениях 10000 -= 1000" << endl << "BigInt = " << testBI << endl;

    testBI = 10000;
    testBIRight = 9500;
    testBI -= testBIRight;
    if (testBI != 500 || testBIRight != 9500 || testBI.getLength() != 3 || testBI.getSize() != 1)
        cout << "BigInt::operator-=() не прошел тест на значениях 10000 -= 9500" << endl << "BigInt = " << testBI << endl;

    testBI = 10000;
    testBI -= "5000";
    if (testBI != 5000 || testBI.getLength() != 4 || testBI.getSize() != 1)
        cout << "BigInt::operator-=() не прошел тест на значениях 10000 -= (string) 5000" << endl << "BigInt = " << testBI << endl;

    testBI = 10000;
    testBI -= 5000;
    if (testBI != 5000 || testBI.getLength() != 4 || testBI.getSize() != 1)
        cout << "BigInt::operator-=() не прошел тест на значениях 10000 -= (int) 5000" << endl << "BigInt = " << testBI << endl;


    //test operator-
    testBI = 200;
    testBIRight = 250;
    try {
        testBI = testBI - testBIRight;
        cout << "BigInt::operator-() выдал отрицательное число" << endl << testBI << endl;
    } catch (const char *str) {
        //cout << str << endl;
    }

    testBI = 10000;
    testBIRight = 15000;
    try {
        testBI = testBI - testBIRight;
        cout << "BigInt::operator-() выдал отрицательное число" << endl << testBI << endl;
    } catch (const char *str) {
        //cout << str << endl;
    }

    testBI = 10000;
    testBIRight = 20000;
    try {
        testBI = testBI - testBIRight;
        cout << "BigInt::operator-() выдал отрицательное число" << endl << testBI << endl;
    } catch (const char *str) {
        //cout << str << endl;
    }


    //test operator--
    testBI = 10;
    testBI--;
    if(testBI != 9)
        cout<< "BigInt::operator++(int) не прошел тест" << endl << "BigInt = " << testBI << endl;

    testBI = 10;
    --testBI;
    if(testBI != 9)
        cout<< "BigInt::operator++() не прошел тест" << endl << "BigInt = " << testBI << endl;

    testBI = 0;
    try {
        testBI--;
        cout << "BigInt::operator-() выдал отрицательное число" << endl << testBI << endl;
    } catch (const char *str) {
        //cout << str << endl;
    }
    if(testBI != 0)
        cout<< "BigInt::operator++() не прошел тест" << endl << "BigInt = " << testBI << endl;


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

    testBI.setValue(10000);
    if(!(testBI == "10000"))
        cout << "BigInt::operator==() не прошел тест на значениях 10000 == (string) 10000" << endl << "Результат: " << (testBI == "10000") << endl;

    testBI.setValue(10000);
    if(!(testBI == 10000))
        cout << "BigInt::operator==() не прошел тест на значениях 10000 == (int) 10000" << endl << "Результат: " << (testBI == 10000) << endl;


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

    testBI.setValue(10000);
    if(!(testBI != "9000"))
        cout << "BigInt::operator!=() не прошел тест на значениях 10000 != (string) 9000" << endl << "Результат: " << (testBI != "9000") << endl;

    testBI.setValue(10000);
    if(!(testBI != 9000))
        cout << "BigInt::operator!=() не прошел тест на значениях 10000 != (int) 9000" << endl << "Результат: " << (testBI != 9000) << endl;


    //test operator>
    testBI.setValue(100);
    testBIRight.setValue(101);
    if((testBI > testBIRight))
        cout << "BigInt::operator>() не прошел тест на значениях 100 > 100" << endl << "Результат: " << (testBI > testBIRight) << endl;

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

    testBI.setValue(10000);
    if(!(testBI > 9000))
        cout << "BigInt::operator>() не прошел тест на значениях 10000 > (int) 9000" << endl << "Результат: " << (testBI > 9000) << endl;


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

    testBI.setValue(9000);
    if(!(testBI < "10000"))
        cout << "BigInt::operator<() не прошел тест на значениях 9000 < (string) 10000" << endl << "Результат: " << (testBI < "10000") << endl;

    testBI.setValue(9000);
    if(!(testBI < 10000))
        cout << "BigInt::operator<() не прошел тест на значениях 9000 < (int) 10000" << endl << "Результат: " << (testBI < 10000) << endl;

    testBI.setValue(12000);
    if(!(testBI < 15000))
        cout << "BigInt::operator<() не прошел тест на значениях 12000 < (int) 15000" << endl << "Результат: " << (testBI < 15000) << endl;


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

    testBI.setValue(10000);
    if(!(testBI >= "10000"))
        cout << "BigInt::operator>=() не прошел тест на значениях 10000 >= (string) 10000" << endl << "Результат: " << (testBI >= "10000") << endl;

    testBI.setValue(10000);
    if(!(testBI >= 10000))
        cout << "BigInt::operator>=() не прошел тест на значениях 10000 >= (int) 10000" << endl << "Результат: " << (testBI >= 10000) << endl;


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

    testBI.setValue(10000);
    if(!(testBI <= "10000"))
        cout << "BigInt::operator<=() не прошел тест на значениях 10000 <= (string) 10000" << endl << "Результат: " << (testBI <= "10000") << endl;

    testBI.setValue(10000);
    if(!(testBI <= 10000))
        cout << "BigInt::operator<=() не прошел тест на значениях 10000 <= (int) 10000" << endl << "Результат: " << (testBI <= 10000) << endl;

    // test operator=
    testBI = BigInt("1234");
    if(testBI != 1234)
        cout << "BigInt::operator=() не прошел тест на значении (BigInt) 1234" << endl << testBI << endl;

    testBI = "1234";
    if(testBI != 1234)
        cout << "BigInt::operator=() не прошел тест на значении (string) 1234" << endl << testBI << endl;

    testBI = 1234;
    if(testBI != 1234)
        cout << "BigInt::operator=() не прошел тест на значении (int) 1234" << endl << testBI << endl;
}