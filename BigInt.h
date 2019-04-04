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
    int *value = NULL;              // число

public:
    BigInt();
    BigInt(BigInt&);
    BigInt(std::string);
    BigInt(int);

    static void trimString(std::string&);
    static bool checkString(std::string);

    int getLength();
    int getSize();
    int* getValue();

    void setValue(std::string);
    void setValue(int);

    void appendValue(int);

    std::string toString();

    BigInt& operator+=(const BigInt& right);
    BigInt operator+(const BigInt& right);

    bool operator==(const BigInt& right);
    bool operator!=(const BigInt& right);
    bool operator> (const BigInt& right);
    bool operator< (const BigInt& right);
    bool operator>=(const BigInt& right);
    bool operator<=(const BigInt& right);

    friend std::ostream& operator<<(std::ostream&, BigInt& bigInt);
    friend std::istream& operator>>(std::istream&, BigInt& bigInt);
};


#endif //RSA_BIGINT_H
