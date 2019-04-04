#include <iostream>
#include "BigInt.h"
#include "tests.h"

using namespace std;

int main() {
    BigInt t("123456789123456789123456789");
    cout << t.toString() << "; " << t.getLength() << "; " << t.getSize() << endl;
    test_BigInt();
    return 0;
}