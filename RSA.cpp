#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "RSA.h"
#include "BigInt.h"

RSA::RSA() {
    // get keys
    input = "";
    N = 0;
    d = 0;
    mode = true;
}

RSA::RSA(std::string input, BigInt e, BigInt N)
{
    // encryption
    if (input == "")
        throw "Пустой текст!";
    else
	    this->input = input;

    this->e = e;
    this->N = N;

    this->mode = false;
}

RSA::RSA(BigInt N, BigInt d)
{
    // decryption

    this->N = N;
    this->d = d;

    this->mode = true;
}

std::string RSA::to_str() // N - то, что имеем
{
	std::string temp = "";
    // Создаём, длинную строку, состоящую из текста по таблице АСКИ
	for (unsigned int i = 0; i < input.size(); ++i)
	{
		int symbol = int(input[i]);
		temp += std::to_string(symbol);
	}
    return temp;
}

void RSA::divide(std::string text) {
    int length = N.getLength() - 1;
    for (int i = 0; i < text.size(); i += length)
    {
        std::string temp_output = "";
        for (int j = 0; j < length && i + j < text.size(); ++j)
        {
            temp_output += text[i + j];
        }

        textToProcess.push_back(temp_output);
    }
}

void RSA::setInput(std::string input) {
    if(d != 0)
        this->input = input;
}

std::string RSA::getOutput() {
    return output;
}

BigInt RSA::getE() {
    return e;
}

BigInt RSA::getN() {
    return N;
}

BigInt RSA::randPrime(std::string fileName, unsigned int fileLines)
{
	unsigned int lineNumber = rand() % (fileLines);
	BigInt result;
	std::string line;
    std::ifstream file (fileName, std::ios::in);
    if (file.is_open())
    {
        for(unsigned int i = 0; i < lineNumber; i++)
            getline(file, line);
        result = line;
        file.close();
    }
    return result;
}

BigInt RSA::randPrimeAsc() {
    return randPrime("prime_asc.txt", ascendingLength);
}

BigInt RSA::randPrimeDes() {
    return randPrime("prime_des.txt", descendingLength);
}

void RSA::encryption()
{
	auto iter = textToProcess.begin();
	while (iter != textToProcess.end())
	{
		BigInt temp = BigInt(*iter);
		BigInt result = BigInt::Power(temp, e, N);
		output.append(result.toString());
		++iter;
	}
}

void RSA::decryption()
{
	auto iter = textToProcess.begin();
	while (iter != textToProcess.end())
	{
		BigInt temp = BigInt(*iter);
		BigInt result = BigInt::Power(temp, d, N);
		output.append(result.toString());
		++iter;
	}
}

bool RSA::getPrimeFilesLines() {
    ascendingLength = 0;
    descendingLength = 0;
    std::string line;
    std::ifstream file ("prime_asc.txt", std::ios::in);
    if (file.is_open()) {
        while(getline(file, line))
            ascendingLength++;
        file.close();
    } else
        return false;

    file = std::ifstream("prime_des.txt", std::ios::in);
    if (file.is_open()) {
        while(getline(file, line))
            descendingLength++;
        file.close();
    } else
        return false;

    return true;
}

// That's true (check)
void RSA::sieve_of_eratosthenes(std::vector<bool> sieve)
{
    ascendingLength = 0;
    descendingLength = 0;
	sieve[0] = sieve[1] = false;
	unsigned long long length = sieve.size();
	for (unsigned long long i = 2; i <= length; ++i)
		if (sieve[i])
			for (unsigned long long j = i * i; j <= length; j += i)
				sieve[j] = false;
    std::ofstream ascendingPrimesFile ("prime_asc.txt", std::ios::out);
    if (ascendingPrimesFile.is_open())
    {
        for(unsigned long long i = 0; i < length/2; i++)
            if(sieve[i]) {
                ascendingPrimesFile << i << '\n';
                ascendingLength++;
            }
        ascendingPrimesFile.close();
    }
    std::ofstream descendingPrimesFile ("prime_des.txt", std::ios::out);
    if (descendingPrimesFile.is_open())
    {
        for(unsigned long long i = length; i >= length/2; i--)
            if(sieve[i]) {
                descendingPrimesFile << i << '\n';
                descendingLength++;
            }
        descendingPrimesFile.close();
    }
}

BigInt RSA::euklidAlgorithm(BigInt num, BigInt mod) {
    BigInt u1 = BigInt(1);
    BigInt v1 = BigInt(0);
    BigInt u2 = BigInt(0);
    BigInt v2 = BigInt(1);
    bool signs[] = {false, false, false, false, false, false};        // false - знак '+'; true - знак '-'
            // 0 - u1, 1 - v1, 2 - u2, 3 - v2, 4 - temp1, 5 - temp2
    /*int u1 = 1;
    int u2 = 0;
    int v1 = 0;
    int v2 = 1;*/

    BigInt q;
    BigInt r;

    while(mod > 0) {
        q = num / mod;
        r = num % mod;

        std::cout << "num = " << num << " = " << u1 << "*F + " << v1 << "*e" << std::endl;
        std::cout << "mod = " << mod << " = " << u2 << "*F + " << v2 << "*e" << std::endl;

        BigInt temp1 = q * u2;

        //std::cout << "q * u2 = " << temp1 <<  << std::endl;
        if(!signs[0] && signs[2]) {
            // из положительного вычесть отрицательное
            temp1 += u1;
            signs[4] = false;
        }else if(!signs[0] && !signs[2]) {
            // из положительного вычесть положительное
            if(temp1 >= u1) {
                // правая часть больше левой
                temp1 -= u1;
                signs[4] = true;
            } else {
                // левая часть больше правой
                temp1 = u1 - temp1;
                signs[4] = false;
            }
        } else if(signs[0] && !signs[2]) {
            // из отрицательного вычесть положительное
            temp1 += u1;
            signs[4] = true;
        } else {
            // из отрицательного вычесть отрицательное
            if(temp1 >= u1) {
                // правая часть больше левой
                temp1 -= u1;
                signs[4] = false;
            } else {
                // левая часть больше правой
                temp1 = u1 - temp1;
                signs[4] = true;
            }
        }
       // std::cout << "u1 - q * u2" << temp1 << std::endl;


        BigInt temp2 = q * v2;
        if(!signs[1] && signs[3]) {
            // из положительного вычесть отрицательное
            temp2 += v1;
            signs[5] = false;
        }else if(!signs[1] && !signs[3]) {
            // из положительного вычесть положительное
            if(temp2 >= v1) {
                // правая часть больше левой
                temp2 -= v1;
                signs[5] = true;
            } else {
                // левая часть больше правой
                temp2 = v1 - temp2;
                signs[5] = false;
            }
        } else if(signs[1] && !signs[3]) {
            // из отрицательного вычесть положительное
            temp2 += v1;
            signs[5] = true;
        } else {
            // из отрицательного вычесть отрицательное
            if(temp2 >= v1) {
                // правая часть больше левой
                temp2 -= v1;
                signs[5] = false;
            } else {
                // левая часть больше правой
                temp2 = v1 - temp2;
                signs[5] = true;
            }
        }


        num = mod;
        mod = r;

        u1 = u2;
        signs[0] = signs[2];

        u2 = temp1;
        signs[2] = signs[4];

        v1 = v2;
        signs[1] = signs[3];

        v2 = temp2;
        signs[3] = signs[5];

       // std::cout << "u2 = " << u2 << "; " << u2.getLength() << "; " << u2.getSize() << std::endl;
    }

    return v2;
}

void RSA::start()
{
    if(mode) {
        // descendingLength = 24491667;
        // ascendingLength = 26355867;
        if(N == 0 || d == 0) {
            if (!getPrimeFilesLines())                                              // Посчитать количество строк в файлах, если они есть
                sieve_of_eratosthenes(std::vector<bool>(1000000000, true));
            p = randPrimeDes();
            q = randPrimeDes();

            N = p * q;

            F = (p - 1) * (q - 1);
            e = randPrimeAsc();
            std::cout << "N = " << N << "; F = " << F << "; e = " << e << std::endl;
            d = euklidAlgorithm(F, e);

            std::cout << "d = " << d << "; " << d.getLength() << "; " << d.getSize() << std::endl;
        } else {
            divide(input);
            decryption();
        }
    } else {
        /*if ((descendingLength)&&(ascendingLength))
            if (!getPrimeFilesLines())
                sieve_of_eratosthenes(std::vector<bool> (1000000000, true));*/

        //std::cout << "Большой файл" << descendingLength<< std::endl;
        //std::cout << "Малый файл" << ascendingLength << std::endl;
        std::cout << "Base txt: " << to_str() << std::endl;
        divide(to_str());
        encryption();

        // 1 5000 0000 0000.
        // 2 0000.
        //std::cout << "Зашифрованный текст: " << output << std::endl;
    }
}