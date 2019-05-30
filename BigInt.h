//
// Created by Максим on 04.04.2019.
//

#ifndef RSA_BIGINT_H
#define RSA_BIGINT_H

#include <string>
#include <vector>

class BigInt {
private:
    int length = 0;                 // количество цифр
    int size = 0;                   // размер value
    int *value = nullptr;           // число
    // TODO: bool sign = false;              // знак числа
    /* Пример: BigInt("1 2345 6789") => value[2]:1 value[1]:2345 value[0]:6789 */

public:
    BigInt();
    BigInt(BigInt&);
    BigInt(const BigInt&);
    explicit BigInt(std::string);
    explicit BigInt(int);

    static void trimString(std::string&);                   // Удалить символы "удобной записи"
    static bool checkString(std::string);                   // Проверить строку на пригодность (является ли числом)
    void checkLength();                                     // Изменить длину числа

    int getLength();
    int getSize();
    int* getValue();

    void setValue(std::string);                             // Записать через string
    void setValue(int);                                     // Записать через int
    void setValue(BigInt&);                                 // Записать через BigInt
    void setValue(const BigInt&);                           // Записать через BigInt

    static int* copyValue(const int*, int);
    static int* copyValue(const int*, int, int);
    void appendValue(int);                                  // Добавить к массиву value новую старшую ячейку со значением number
    void shrinkValue();                                     // Удалить пустые старшие ячейки value
    void highMinusOne();                                    // Вычесть из самого старшего разряда единицу
    void pushToBack(int);                                   // Добавить цифру в конец числа
    void pushToBack(BigInt);                                // Добавить BigInt цифру в конец числа
    void popFromBack();
    static int getRatio(int, int);                          // Вычислить коэфициент умножения
    static BigInt getRatio(BigInt, BigInt);                 // TODO: Вычислить коэфициент умножения BigInt
    void correctValue();

    std::string toString();
    int toInteger();

    BigInt& operator=(BigInt right);                        // Оператор присваивания из BigInt
    BigInt& operator=(std::string right);                   // Оператор присваивания из string
    BigInt& operator=(int right);                           // Оператор присваивания из int

    BigInt& operator+=(const BigInt& right);                // Прибавить к текущему другой BigInt
    BigInt operator+(const BigInt& right);                  // Сумма двух BigInt
    BigInt& operator+=(std::string right);                  // Прибавить к текущему другой BigInt (через string)
    BigInt operator+(std::string right);                    // Сумма BigInt и (string -> BigInt)
    BigInt& operator+=(int right);                          // Прибавить к текущему другой BigInt (через int)
    BigInt operator+(int right);                            // Сумма BigInt и (int -> Bigint)
    BigInt& operator++();                                   // Префиксный инкремент
    const BigInt operator++(int);                           // Постфиксный инкремент

    BigInt& operator-=(const BigInt& right);                // Вычесть из текущего другой BigInt
    BigInt operator-(const BigInt& right);                  // Разность двух BigInt
    BigInt& operator-=(std::string right);                  // Вычесть из текущего другой BigInt (через string)
    BigInt operator-(std::string right);                    // Разность BigInt и (string -> BigInt)
    BigInt& operator-=(int right);                          // Вычесть из текущего другой BigInt (через int)
    BigInt operator-(int right);                            // Разность BigInt и (int -> BigInt)
    BigInt& operator--();                                   // Префиксный инкремент
    const BigInt operator--(int);                           // Постфиксный инкремент

    BigInt& operator*=(const BigInt& right);                // Умножить на BigInt
    BigInt operator*(const BigInt& right);                  // Произведение двух BigInt
    BigInt& operator*=(std::string right);                  // Умножить на BigInt (через string)
    BigInt operator*(std::string right);                    // Произведение BigInt и (string -> BigInt)
    BigInt& operator*=(int right);                          // Умножить на BigInt (через int)
    BigInt operator*(int right);                            // Произведение BigInt и (int -> BigInt)

    BigInt& operator/=(const BigInt& right);                // Разделить на BigInt
    BigInt operator/(const BigInt& right);                  // Частное двух BigInt
    BigInt& operator/=(std::string right);                  // Разделить на BigInt (через string)
    BigInt operator/(std::string right);                    // Частное BigInt и (string -> BigInt)
    BigInt& operator/=(int right);                          // Разделить на BigInt (через int)
    BigInt operator/(int right);                            // Частное BigInt и (int -> BigInt)

    BigInt& operator%=(const BigInt& right);                // Записать остаток от деления на BigInt
    BigInt operator%(const BigInt& right);                  // Остаток от деления BigInt
    BigInt& operator%=(std::string right);                  // Записать остаток от деления на BigInt (через string)
    BigInt operator%(std::string right);                    // Остаток от деления BigInt и (string -> BigInt)
    BigInt& operator%=(int right);                          // Записать остаток от деления на BigInt (через int)
    BigInt operator%(int right);                            // Остаток от деления BigInt и (int -> BigInt)

    static BigInt Power(BigInt, BigInt);
    static BigInt Power(BigInt, BigInt, BigInt);
    // BigInt Power(BigInt);
    // BigInt Power(BigInt, BigInt);

    // Операторы сравнения с объектами класса
    bool operator==(const BigInt& right);
    bool operator!=(const BigInt& right);
    bool operator> (const BigInt& right);
    bool operator< (const BigInt& right);
    bool operator>=(const BigInt& right);
    bool operator<=(const BigInt& right);

    // Операторы сравнения со string
    bool operator==(std::string right);
    bool operator!=(std::string right);
    bool operator> (std::string right);
    bool operator< (std::string right);
    bool operator>=(std::string right);
    bool operator<=(std::string right);

    // Операторы сравнения с int
    bool operator==(int right);
    bool operator!=(int right);
    bool operator> (int right);
    bool operator< (int right);
    bool operator>=(int right);
    bool operator<=(int right);

    // Операторы работы с потоками
    friend std::ostream& operator<<(std::ostream&, BigInt& bigInt);
    friend std::istream& operator>>(std::istream&, BigInt& bigInt);


};


#endif //RSA_BIGINT_H
