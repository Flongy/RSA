#include <iostream>
#include <fstream>
#include "BigInt.h"
#include "RSA.h"
#include "tests.h"

using namespace std;

int main() {
    srand(time(0));

    BigInt test1 = BigInt("519204090418487100");
    BigInt test2 = BigInt("218886299");
    BigInt test3 = BigInt("2226134185912430299");
    // test_BigInt();

     cout << ((test1 * test2) % test3).toString() << endl;
    // RSA::euklidAlgorithm(test1,test2);
/*
    RSA decrypt;
    decrypt.start();

    cout << "Decryption: e = " << decrypt.getE().toString() << "; N = " << decrypt.getN().toString() << endl;

    RSA encrypt("Hello world!", decrypt.getE(), decrypt.getN());

    encrypt.start();

    cout << "Encrypted text: " << encrypt.getOutput() << endl;

    decrypt.setInput(encrypt.getOutput());
    decrypt.start();


    cout << "Decrypted text: " << decrypt.getOutput() << endl;

*/


    //cout << rand() << endl;

    //cin >> test;
    //cout << test << endl;

    /*string line;
    ifstream myfile ("primals.txt", ios::in);
    if (myfile.is_open())
    {
        while (getline (myfile, line))
        {
            if(rand() % 5 == 0) {
                test = line;
                break;
            }
            test = line;
        }
        myfile.close();
    }

    else cout << "Unable to open file";
    cout << test << endl;*/
    /*for(int i = 0; i < k; i++)
        cout << test[i] << endl;*/

    /*BigInt Vasya;
    BigInt Johny;
    Vasya = "3";
    // "59 4226 9176 3146 5592 0301 8078 2336";
    Johny = "100";
    BigInt result = BigInt::Power(Vasya, Johny);

    for(int i = result.getSize() - 1; i >= 0; i--)
        cout << result.getValue()[i] << " ";
    cout << endl;
    cout << "Result = " << result << endl;*/
    return 0;
}