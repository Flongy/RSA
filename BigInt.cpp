//
// Created by Максим on 04.04.2019.
//

#include "BigInt.h"
#include <string>
#include <iostream>
#include <utility>

using namespace std;

BigInt::BigInt() = default;

BigInt::BigInt(BigInt& bigInt) {
    setValue(bigInt);
}

BigInt::BigInt(string s) {
    setValue(std::move(s));
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

void BigInt::checkLength() {
    // Запись новой длины
    int number = value[size - 1];
    length = (size - 1) * 4;
    while (number != 0) { number /= 10; length++; }

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
    trimString(s);
    if (!checkString(s)) {
        cout<<"Указаная строка содержит символ, не являющийся цифрой"<<endl;
    } else {
        length = (int) s.length();
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

void BigInt::setValue(BigInt& bigInt) {
    // Записать в value новое значение (BigInt)
    length = bigInt.length;
    size = bigInt.size;
    delete [] value;
    value = copyValue(bigInt.value, size);
}

int* BigInt::getValue() {
    // Вывести массив value
    return value;
}

int* BigInt::copyValue(const int *value, int size) {
    // Копирование value
    auto *newValue = new int[size];         // новый массив
    for (int i = 0; i < size; i++)
        newValue[i] = value[i];             // копирование старых значений
    return newValue;
}

void BigInt::appendValue(int number) {
    // Добавить к массиву value новую ячейку со значением number
    if(number > 9999) {                         // Если number имеет больше четырех цифр (не помещается в ячейку)
        appendValue(number % 10000);
        appendValue(number / 10000);
    } else {
        size++;
        auto *newValue = new int[size];         // новый массив
        for (int i = 0; i < size - 1; i++)
            newValue[i] = value[i];             // копирование старых значений
        newValue[size - 1] = number;            // добавление новой ячейки
        delete[] value;
        value = newValue;                       // замена
        // Запись новой длины
        length = (size - 1) * 4;
        while (number != 0) { number /= 10; length++; }
    }
}

void BigInt::shrinkValue() {
    // Удалить пустые старшие ячейки value
    for(int i = size - 1; i >= 0 && value[i] == 0; i--)
        size--;
    int *newValue = copyValue(value, size);
    delete [] value;
    value = newValue;
    checkLength();                          // новая длина
}

string BigInt::toString() {
    // Вывод числа в виде string
    string result;
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

BigInt& BigInt::operator=(BigInt right) {
    setValue(right);
    return *this;
}

BigInt& BigInt::operator=(const std::string right) {
    setValue(right);
    return *this;
}

BigInt& BigInt::operator=(const int right) {
    setValue(right);
    return *this;
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
    checkLength();
    return *this;
}

BigInt BigInt::operator+(const BigInt &right) {
    return BigInt(*this) += right;
}

BigInt& BigInt::operator+=(string right) {
    return *this += BigInt(std::move(right));
}

BigInt BigInt::operator+(string right) {
    return BigInt(*this) += BigInt(std::move(right));
}

BigInt& BigInt::operator+=(int right) {
    return *this += BigInt(right);
}

BigInt BigInt::operator+(int right) {
    return BigInt(*this) += BigInt(right);
}

BigInt& BigInt::operator++() {
    return *this += 1;
}

const BigInt BigInt::operator++(int) {
    auto temp = BigInt(*this);
    ++*this;
    return temp;
}

BigInt& BigInt::operator-=(const BigInt &right) {
    // TODO: Нестабильное поведение: иногда большее второе (правое) число считается меньшим
            // Исправлено?
    if(*this < right)
        throw "ERROR: Отрицательное число";
    else {
        for (int i = 0; i < right.size; i++) {
            value[i] -= right.value[i];
            if (value[i] < 0) {
                value[i + 1]--;
                value[i] += 10000;
            }
        }
        shrinkValue();
    }
    return *this;
}

BigInt BigInt::operator-(const BigInt &right) {
    return BigInt(*this) -= right;
}

BigInt& BigInt::operator-=(string right) {
    return *this -= BigInt(right);
}

BigInt BigInt::operator-(string right) {
    return BigInt(*this) -= BigInt(right);
}

BigInt& BigInt::operator-=(int right) {
    return *this -= BigInt(right);
}

BigInt BigInt::operator-(int right) {
    return BigInt(*this) -= BigInt(right);
}

BigInt& BigInt::operator--() {
    return *this -= 1;
}

const BigInt BigInt::operator--(int) {
    auto temp = BigInt(*this);
    --*this;
    return temp;
}

bool BigInt::operator==(const BigInt &right) {
    if (length != right.length)
        return false;
    for (int i = 0; i < size; i++)
        if (value[i] != right.value[i])
            return false;
    return true;
}

bool BigInt::operator==(string right) {
    return *this == BigInt(std::move(right));
}

bool BigInt::operator==(int right) {
    return *this == BigInt(right);
}

bool BigInt::operator!=(const BigInt &right) {
    if (length != right.length)
        return true;
    for (int i = 0; i < size; i++)
        if (value[i] != right.value[i])
            return true;
    return false;
}

bool BigInt::operator!=(string right) {
    return *this != BigInt(std::move(right));
}

bool BigInt::operator!=(int right) {
    return *this != BigInt(right);
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

bool BigInt::operator> (string right) {
    return *this > BigInt(std::move(right));
}

bool BigInt::operator> (int right) {
    return *this > BigInt(right);
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

bool BigInt::operator< (string right) {
    return *this < BigInt(std::move(right));
}

bool BigInt::operator< (int right) {
    return *this < BigInt(right);
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

bool BigInt::operator>=(string right) {
    return *this >= BigInt(std::move(right));
}

bool BigInt::operator>=(int right) {
    return *this >= BigInt(right);
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

bool BigInt::operator<=(string right) {
    return *this <= BigInt(std::move(right));
}

bool BigInt::operator<=(int right) {
    return *this <= BigInt(right);
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