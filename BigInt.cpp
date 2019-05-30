//
// Created by Максим on 04.04.2019.
//

#include "BigInt.h"
#include <string>
#include <iostream>
#include <utility>
#include <cmath>

using namespace std;

BigInt::BigInt() = default;

BigInt::BigInt(BigInt& bigInt) {
    setValue(bigInt);
}

BigInt::BigInt(const BigInt& bigInt) {
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
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
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
        cout << "Указаная строка содержит символ, не являющийся цифрой" << endl << s << endl;
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

void BigInt::setValue(const BigInt& bigInt) {
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

int* BigInt::copyValue(const int *value, int size, int copySize) {
    // Копирование value нового размера
    auto *newValue = new int[size];         // новый массив
    for (int i = 0; i < copySize; i++)
        newValue[i] = value[i];             // копирование старых значений
    return newValue;
}

void BigInt::appendValue(int number) {
    // Добавить к массиву value новую старшую ячейку со значением number
    if (number < 0)
        return;
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
    if(length == 1 && value[0] == 0)
        return;
    for(int i = size - 1; i >= 0 && value[i] == 0; i--)
        size--;
    int *newValue = copyValue(value, size);
    delete [] value;
    value = newValue;
    checkLength();                          // новая длина

    //if(length)
}

void BigInt::highMinusOne() {
    // Вычесть из самого старшего разряда единицу
    int high = value[size - 1];
    int hLength = 0;
    int hCopy = high;
    while (hCopy != 0) { hCopy /= 10; hLength++; }

    high -= pow(10, hLength - 1);
    value[size - 1] = high;
}

void BigInt::pushToBack(int number) {
    // Добавить цифру в конец числа
    if(number < 0)
        return;
    if(length == 1 && value[0] == 0)
        value[0] = number;
    else if(number > 9) { // TODO: Ускорить для добавления чисел большей длины
        pushToBack(number / 10);
        pushToBack(number % 10);
    } else {
        int* tempValue;
        if(size * 4 == length) {
            size++;
            tempValue = new int[size];
            tempValue[size - 1] = 0;
            for(int i = size - 2; i >= 0; i--) {
                tempValue[i + 1] += value[i] / 1000;
                tempValue[i] = (value[i] % 1000) * 10;
            }
            tempValue[0] += number;
        } else {
            tempValue = new int[size];
            for(int i = size - 1; i >= 1; i--) {
                tempValue[i] = (value[i] % 1000) * 10;
                tempValue[i] += value[i - 1] / 1000;
            }
            tempValue[0] = (value[0] % 1000) * 10 + number;
        }
        length++;
        delete[] value;
        value = tempValue;
    }
}

void BigInt::pushToBack(BigInt number) {
    // TODO: Переписать
    pushToBack(number.toInteger());
}

void BigInt::popFromBack() {
    // Удалить одну цифру с конца
    if(length > 1) {
        int* tempValue;
        if((size - 1) * 4 == --length) {
            size--;
            tempValue = new int[size];
            for(int i = size - 1; i >= 0; i--)
                tempValue[i] = value[i + 1] * 1000 + value[i] / 10;
        } else {
            tempValue = new int[size];
            for(int i = size - 2; i >= 0; i--)
                tempValue[i] = (value[i + 1] % 10) * 1000 + value[i] / 10;
            tempValue[size - 1] = value[size - 1] / 10;
        }
        delete [] value;
        value = tempValue;
    } else {
        value[0] = 0;
    }
}

// TODO: Метод для удаления нескольких цифр

int BigInt::getRatio(int a, int b) {
    // Вычислить коэффициент умножения
    if(b == 0)
        return -1;

    while(a < b) b /= 10;

    int aLength = 0;
    int aCopy = a;
    while (aCopy != 0) { aCopy /= 10; aLength++; }

    int bLength = 0;
    int bCopy = b;
    while (bCopy != 0) { bCopy /= 10; bLength++; }
    //while (bLength > 3) { b/= 10; bLength--;}

    while(aLength - bLength > 0 && a / b > 10)
    { a /= 10; aLength--; }

    return a / b;
}

// TODO: Вычисление коэффициента в BigInt


void BigInt::correctValue() {
    for(int k = 0; k < size; k++) {
        value[k + 1] += value[k] / 10000;
        value[k] = value[k] % 10000;
    }
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

int BigInt::toInteger() {
    // Вывод числа в виде int
    // TODO: Ускорить
    if(*this <= INT_MAX)
        return stoi(toString());
    return -1;
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
    int *newValue;
    if(size < right.size){
        newValue = new int[right.size]();

        for (int i = size; i < right.size; i++)
            newValue[i] = right.value[i];

        for(int i = 0; i < size; i++) {
            newValue[i] += value[i] + right.value[i];
            if (newValue[i] > 9999) {          // переполнение ячейки
                newValue[i + 1]++;
                newValue[i] -= 10000;
            }
        }
        size = right.size;
    } else if(size == right.size ) {
        if(size == right.size && value[size - 1] + right.value[size - 1] > 9999)
            newValue = new int[++size]();
        else
            newValue = new int[size]();

        for(int i = 0; i < right.size; i++) {
            newValue[i] += value[i] + right.value[i];
            if (newValue[i] > 9999) {          // переполнение ячейки
                newValue[i + 1]++;
                newValue[i] -= 10000;
            }
        }
    } else {
        newValue = new int[size]();

        for (int i = right.size; i < size; i++)
            newValue[i] = value[i];

        for(int i = 0; i < right.size; i++) {
            newValue[i] += value[i] + right.value[i];
            if (newValue[i] > 9999) {          // переполнение ячейки
                newValue[i + 1]++;
                newValue[i] -= 10000;
            }
        }
    }
    delete [] value;
    value = newValue;
    correctValue();
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
    else if(*this == right)
        *this = 0;
    else{
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

/* Старый -=
BigInt& BigInt::operator-=(const BigInt &right) {
    // TODO: Нестабильное поведение: иногда большее второе (правое) число считается меньшим
            // Исправлено?
    if(*this < right)
        throw "ERROR: Отрицательное число";
    else if(*this == right)
        *this = 0;
    else{
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
}*/

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

BigInt& BigInt::operator*=(const BigInt &right) {
    int *newValue = new int[size + right.size + 1]();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < right.size; j++) {
            int res = value[i] * right.value[j];
            newValue[i + j] += res % 10000;
            newValue[i + j + 1] += res / 10000;
        }
    }

    delete [] value;
    size = size + right.size + 1;
    value = newValue;
    correctValue();
    shrinkValue();
    return *this;
}

BigInt BigInt::operator*(const BigInt &right) {
    return BigInt(*this) *= right;
}

BigInt& BigInt::operator*=(string right) {
    return *this *= BigInt(right);
}

BigInt BigInt::operator*(string right) {
    return BigInt(*this) *= BigInt(right);
}

BigInt& BigInt::operator*=(int right) {
    return *this *= BigInt(right);
}

BigInt BigInt::operator*(int right) {
    return BigInt(*this) *= BigInt(right);
}

BigInt& BigInt::operator/=(const BigInt &right) {
    if(*this < right)
        setValue(0);
    else {
        BigInt result = BigInt(0);
        result = 0;
        while(*this >= right) {
            BigInt newRight = right;                // Сделать копию
            BigInt ratio = BigInt(getRatio((size == 1) ? value[size - 1] : value[size - 1] * 10000 + value[size - 2],
                    (right.size == 1) ? right.value[right.size - 1] : right.value[right.size - 1] * 10000 + right.value[right.size - 2]));
            // TODO: Переписать getRatio под BigInt
            BigInt ratioCopy = ratio;               // Промежуточный результат
            BigInt newRightCopy = newRight;
            newRight *= ratio;
            while (newRight.length != length) { newRight *= 10; ratioCopy *= 10; }    // TODO: Ускорить
            if (*this >= newRight)
                *this -= newRight;
            else {
                *this -= (newRight/10);
                ratioCopy /= 10;
                //*this -= newRightCopy * --ratio;
                //ratioCopy.highMinusOne();
            }
            result += ratioCopy;

        }
        *this = result;

    }

    return *this;
}

BigInt BigInt::operator/(const BigInt &right) {
    return BigInt(*this) /= right;
}

BigInt& BigInt::operator/=(string right) {
    return *this /= BigInt(right);
}

BigInt BigInt::operator/(string right) {
    return BigInt(*this) /= BigInt(right);
}

BigInt& BigInt::operator/=(int right) {
    return *this /= BigInt(right);
}

BigInt BigInt::operator/(int right) {
    return BigInt(*this) /= BigInt(right);
}

BigInt& BigInt::operator%=(const BigInt &right) {
    if(*this < right)
        return *this;
    else {
        while(*this >= right) {
            BigInt newRight = right;                // Сделать копию
            BigInt ratio = BigInt(getRatio((size == 1) ? value[size - 1] : value[size - 1] * 10000 + value[size - 2],
                    (right.size == 1) ? right.value[right.size - 1] : right.value[right.size - 1] * 10000 + right.value[right.size - 2]));
            // TODO: Переписать getRatio под BigInt
            BigInt newRightCopy = newRight;
            newRight *= ratio;
            while (newRight.length != length) { newRight *= 10; }    // TODO: Ускорить
            if (*this >= newRight)
                *this -= newRight;
            else
                *this -= (newRight/10);
        }
    }
    return *this;
}

BigInt BigInt::operator%(const BigInt &right) {
    return BigInt(*this) %= right;
}

BigInt& BigInt::operator%=(string right) {
    return *this %= BigInt(right);
}

BigInt BigInt::operator%(string right) {
    return BigInt(*this) %= BigInt(right);
}

BigInt& BigInt::operator%=(int right) {
    return *this %= BigInt(right);
}

BigInt BigInt::operator%(int right) {
    return BigInt(*this) %= BigInt(right);
}

BigInt BigInt::Power(BigInt left, BigInt right) {
    if(right == 0)
        return BigInt(1);
    // cout << left << " Степень: " << right << endl;
    if((right % 2) == 1)                    // TODO: Определить четность
        return Power(left, --right) * left;
    else
        return Power(left * left, right /= 2);
}

BigInt BigInt::Power(BigInt left, BigInt right, BigInt mod) {
    if(right == 0)
        return BigInt(1);
    if((right % 2) == 1)                    // TODO: Определить четность
        return (Power(left, --right, mod) * left) % mod;
    else
        return (Power((left * left) % mod, right /= 2, mod)) % mod;
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
    for (int i = size - 1; i >= 0; i--)
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
    for (int i = size - 1; i >= 0; i--)
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
    for (int i = size - 1; i >= 0; i--)
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
    for (int i = size - 1; i >= 0; i--)
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