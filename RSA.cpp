#include <string>
#include <vector>
#include "RSA.h"
#include "BigInt.h"

RSA::RSA(std::string text = "", std::string p = "", std::string q = "")
{
	if text == ""
		throw "Пустой текст!"
	else
		this.text = text;

	if BigInt.checkString(p)
		this->p = new BigInt(p);

	if BigInt.checkString(q)
		this->q = new BigInt(q);
}

BigInt RSA::rand_prime()
{
	int temp = 10 + rand() % 1000000;
	if !sieve[temp]
		rand_prime()
	return BigInt(temp);
}

void RSA::sieve_of_eratosthenes()
{
	sieve[0] = sieve[1] = false;
	for (int i = 2; i <= length; ++i)
		if (sieve[i])
			if (i * 1ll * i <= length)
				for (int j = i * i; j <= n; j += i)
					sieve[j] = false;
}

RSA::start()
{
	N = p * q;
	F = (p - 1)*(q - 1);
	e = rand_prime();
}
