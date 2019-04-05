#pragma once
#include "BigInt.h"
#include <string>
#include <vector>

class RSA
{
	private BigInt p; // первое значение
	private BigInt q; // второе значение
	private BigInt N;
	private BigInt f;
	private BigInt e;
	private std::string input; // шифруемый текст
	private std::string output; // зашифрованный текст
	private std::vector<bool> sieve(1000000, true); // решето 

	public RSA(std::string text = "", std::string p = "", std::string q = ""); // коструктор по умочланию

	private BigInt rand_prime(); // инициализация Е
	private void sieve_of_eratosthenes(); // инициализация решета 

	public start(); // запуск работы алгоритма
};