//
// Created by Максим on 04.04.2019.
//

#include "BigInt.h"
#include <string>
#include <iostream>

using namespace std;

BigInt::BigInt() {}

BigInt::BigInt(BigInt& bigInt) {
    length = bigInt.length;
    size = bigInt.size;
    value = bigInt.value;
}

BigInt::BigInt(string s) {
    setValue(s);
}

BigInt::BigInt(int s) {
    setValue(s);
}

void BigInt::trimString(string& s) {
    // Убрать из строки символы "удобной записи": ' ', ','
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    s.erase(remove(s.begin(), s.end(), ','), s.end());
}

bool BigInt::checkString(string s) {
    // Проверка строки на содержание символов, которые не являются цифрами
    if (s[0] == '-')
        return false;           // TODO: сделать знаковый BigInt?
    for(char c:s)
        if (!isdigit(c))
            return false;       // вернуть false, если в строке есть нецифровой символ
    return true;                // вернуть true, если в строке задано число
}

int BigInt::getLength() {
    // Вывести количество цифр
    return length;
}

int BigInt::getSize() {
    // Вывести количество ячеек (четверок)
    return size;
}

void BigInt::setValue(string s) {
    // Записать в value новое значение
    delete [] value;                                                            // Удалить старый указатель
    trimString(s);
    if (!checkString(s)) {
        cout<<"Указаная строка содержит символ, не являющийся цифрой"<<endl;
    } else {
        length = s.length();
        size = length % 4 == 0 ? length/4 : length/4 + 1;                       // Расчет количества ячеек для value так, чтобы число поместилось целиком
        value = new int[size];
        for(int i = 0; i < size; i++) {
            int currentIndex = length - (i + 1) * 4;                            // С какой позиции берется следующая четверка
            if (currentIndex < 0)                                               // Если количество цифр в ячейке меньше четырех
                value[i] = stoi(s.substr(0, 4 + currentIndex));
            else
                value[i] = stoi(s.substr(currentIndex, 4));
        }
    }
}

void BigInt::setValue(int s) {
    // Записать в value новое значение (int)
    setValue(to_string(s));
}

int* BigInt::getValue() {
    // Вывести массив value
    return value;
}

void BigInt::appendValue(int number) {
    // Добавить к массиву value новую ячейку со значением number
    if(number > 9999) {                         // Если number имеет больше четырех цифр (не помещается в ячейку)
        appendValue(number % 10000);
        appendValue(number / 10000);
    } else {
        size++;
        int *newValue = new int[size];          // новый массив
        for (int i = 0; i < size - 1; i++)
            newValue[i] = value[i];             // копирование старых значений
        newValue[size - 1] = number;            // добавление новой ячейки
        delete[] value;
        value = newValue;                       // замена
    }
}

string BigInt::toString() {
    // Вывод числа в виде string
    string result = "";
    for (int i = 0; i < size; i++) {
        // Проверка, если в ячейке число имеет меньше четырех цифр,
        // добавление дополнительных нулей в string запись при необходимости
        if (value[i] == 0 && size > i + 1)
            result = "0000" + result;
        else if (value[i] < 10 && size > i + 1)
            result = "000" + to_string(value[i]) + result;
        else if (value[i] < 100 && size > i + 1)
            result = "00" + to_string(value[i]) + result;
        else if (value[i] < 1000 && size > i + 1)
            result = "0" + to_string(value[i]) + result;
        else
            result = to_string(value[i]) + result;
    }
    return result;
}

BigInt& BigInt::operator+=(const BigInt& right) {
    for(int i = 0; i < min(size, right.size); i++) {
        value[i] += right.value[i];
        if (value[i] > 9999) {          // переполнение ячейки
            if (size < i + 2)
                appendValue(1);
            else
                value[i + 1]++;
            value[i] -= 10000;
        }
    }
    return *this;
}

BigInt BigInt::operator+(const BigInt &right) {
    return BigInt(*this) += right;
}

bool BigInt::operator==(const BigInt &right) {
    if (length != right.length)
        return false;
    for (int i = 0; i < size; i++)
        if (value[i] != right.value[i])
            return false;
    return true;
}

bool BigInt::operator!=(const BigInt &right) {
    return !(*this == right);
}

bool BigInt::operator> (const BigInt &right) {
    if (length > right.length)
        return true;
    if(length < right.length)
        return false;
    for (int i = size; i >= 0; i--)
        if(value[i] > right.value[i])
            return true;
        else if(value[i] < right.value[i])
            return false;
    return false;
}

bool BigInt::operator< (const BigInt &right) {
    if (length < right.length)
        return true;
    if(length > right.length)
        return false;
    for (int i = size; i >= 0; i--)
        if(value[i] < right.value[i])
            return true;
        else if(value[i] > right.value[i])
            return false;
    return false;
}

bool BigInt::operator>=(const BigInt &right) {
    if (length > right.length)
        return true;
    if(length < right.length)
        return false;
    for (int i = size; i >= 0; i--)
        if(value[i] > right.value[i])
            return true;
        else if(value[i] < right.value[i])
            return false;
    return true;
}

bool BigInt::operator<=(const BigInt &right) {
    if (length < right.length)
        return true;
    if(length > right.length)
        return false;
    for (int i = size; i >= 0; i--)
        if(value[i] < right.value[i])
            return true;
        else if(value[i] > right.value[i])
            return false;
    return true;
}

ostream& operator<<(ostream& os, BigInt& bigInt) {
    // Вывод BigInt в поток
    return os << bigInt.toString();
}

istream& operator>>(istream& is, BigInt& bigInt) {
    // Ввод нового BigInt из потока
    string s;
    is >> s;
    bigInt.setValue(s);
    return is;
}