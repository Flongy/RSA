#pragma once
#include "BigInt.h"
#include <string>
#include <vector>
#include <fstream>

class RSA
{
private:
    bool mode;                                  // Режим работы: false - шифровать, true - расшифровать
	BigInt p; // первое значение
	BigInt q; // второе значение
	BigInt N; // модуль RSA
	BigInt F; // Функция Эйлера
	BigInt e; // Степень шифрования
	BigInt d; // Степень расшифрования
	std::string input; // Шифруемый текст
    std::vector<std::string> textToProcess;     // Текст разделенный на подстроки для encryption/decryption
	std::string output; // Зашифрованный текст
    unsigned int ascendingLength;               // Количество строк в файле малых простых чисел
    unsigned int descendingLength;              // Количество строк в файле больших простых чисел

public:
	RSA();                                                  // коструктор для вычисления ключей
	RSA(std::string, BigInt, BigInt);               // коструктор для шифрования
	RSA(BigInt, BigInt);                                    // коструктор для расшифровки
	void start(); // запуск работы алгоритма
	void setInput(std::string);                             // Указать входной текст
	std::string getOutput();                                // Получить результат работы
    BigInt getE();
    BigInt getN();
    static BigInt euklidAlgorithm(BigInt, BigInt);                 // Обратный элемент в кольце по модулю
private:
    std::string to_str();
    void divide(std::string);
	BigInt randPrime(std::string, unsigned int);            // Получить случайное простое число из файла
	BigInt randPrimeAsc();                                  // Получить случайное малое простое число из файла
	BigInt randPrimeDes();                                  // Получить случайное большое простое число из файла
    void encryption();
    void decryption();
    bool getPrimeFilesLines();                              // Посчитать количество строк в файлах с простыми числами
	void sieve_of_eratosthenes(std::vector<bool> sieve);    // инициализация решета
};