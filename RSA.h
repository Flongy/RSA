#pragma once
#include "BigInt.h"
#include <string>
#include <vector>

class RSA
{
	private BigInt p; // ������ ��������
	private BigInt q; // ������ ��������
	private BigInt N;
	private BigInt f;
	private BigInt e;
	private std::string input; // ��������� �����
	private std::string output; // ������������� �����
	private std::vector<bool> sieve(1000000, true); // ������ 

	public RSA(std::string text = "", std::string p = "", std::string q = ""); // ���������� �� ���������

	private BigInt rand_prime(); // ������������� �
	private void sieve_of_eratosthenes(); // ������������� ������ 

	public start(); // ������ ������ ���������
};