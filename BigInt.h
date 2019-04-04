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
    BigInt(std::string);
    BigInt(int);
    static bool checkString(std::string);
    int getLength();
    int getSize();
    void setValue(std::string);
    void setValue(int);
    int* getValue();
    std::string toString();
};


#endif //RSA_BIGINT_H
