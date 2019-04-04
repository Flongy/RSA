//
// Created by Максим on 04.04.2019.
//

#include "BigInt.h"
#include <string>
#include <iostream>

using namespace std;

BigInt::BigInt() {}

BigInt::BigInt(string s) {
    setValue(s);
}

BigInt::BigInt(int s) {
    setValue(s);
}

bool BigInt::checkString(string s) {
    // Проверка строки на содержание символов, которые не являются цифрами
    for(char c:s) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

int BigInt::getLength() {
    return length;
}

int BigInt::getSize() {
    return size;
}

void BigInt::setValue(string s) {
    if (!checkString(s)) {
        cout<<"Указаная строка содержит символ, не являющийся цифрой"<<endl;
    } else {
        length = s.length();
        size = length % 4 == 0 ? length/4 : length/4 + 1;
        value = new int[size];
        for(int i = 0; i < size; i++) {
            int currentIndex = length - (i + 1) * 4;
            if (currentIndex < 0)
                value[i] = stoi(s.substr(0, 4 + currentIndex));
            else
                value[i] = stoi(s.substr(currentIndex, 4));
        }
    }
}

void BigInt::setValue(int s) {
    setValue(to_string(s));
}

int* BigInt::getValue() {
    return value;
}

string BigInt::toString() {
    string result = "";
    for (int i = 0; i < size; i++) {
        result = to_string(value[i]) + result;
    }
    return result;
}