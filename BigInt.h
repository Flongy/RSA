//
// Created by Максим on 04.04.2019.
//

#ifndef RSA_BIGINT_H
#define RSA_BIGINT_H

#include <string>

class BigInt {
private:
    int length = 0;                 // количество цифр
    int size = 0;                   // размер value
    int *value = nullptr;           // число
    /* Пример: BigInt("123456789") => value[2]:1 value[1]:2345 value[0]:6789 */

public:
    explicit BigInt();
    explicit BigInt(BigInt&);
    explicit BigInt(std::string);
    explicit BigInt(int);

    static void trimString(std::string&);                   // Удалить символы "удобной записи"
    static bool checkString(std::string);                   // Проверить строку на пригодность (является ли числом)

    int getLength();
    int getSize();
    int* getValue();

    void setValue(std::string);                             // Записать через string
    void setValue(int);                                     // Записать через int
    void setValue(BigInt&);                                  // Записать через BigInt

    static int* copyValue(const int*, int);
    void appendValue(int);
    //void removeValue(int);

    std::string toString();

    BigInt& operator=(BigInt right);                        // Оператор присваивания из BigInt
    BigInt& operator=(std::string right);                   // Оператор присваивания из string
    BigInt& operator=(int right);                           // Оператор присваивания из int

    BigInt& operator+=(const BigInt& right);                // Прибавить к текущему другой BigInt
    BigInt operator+(const BigInt& right);                  // Сложение двух BigInt
    BigInt& operator+=(std::string right);                  // Прибавить к текущему другой BigInt (через string)
    BigInt operator+(std::string right);                    // Сложение BigInt и string -> BigInt
    BigInt& operator+=(int right);                          // Прибавить к текущему другой BigInt (через int)
    BigInt operator+(int right);                            // Сложение BigInt и int -> Bigint
    BigInt& operator++();                                   // Префиксный инкремент
    BigInt operator++(int);                                 // Постфиксный инкремент

    /* TODO: Вычитание
    BigInt& operator-=(const BigInt& right);
    BigInt operator-(const BigInt& right);
    BigInt& operator-=(const std::string right);
    BigInt operator-(const std::string right);
    BigInt& operator-=(const int right);
    BigInt operator-(const int right);
    BigInt operator--();*/

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
