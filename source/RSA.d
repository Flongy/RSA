module rsa;

import std.stdio;
import bigint;

class RSA {
    private:
    const BigInt sk = null;  /// Закрытый ключ
    public:
    const BigInt pk = null;  /// Открытый ключ
    static const BigInt e = new BigInt(65_537);

    /// Конструктор нового класса RSA. pk - открытый ключ, sk - закрытый ключ
    this(const BigInt pk, const BigInt sk) {
        this.pk = new BigInt(pk);
        this.sk = new BigInt(sk);
    }

    /// Конструктор нового класса RSA только для зашифровывания сообщения
    this(const BigInt pk) {
        this.pk = new BigInt(pk);
    }

    /// Деструктор
    ~this() {
        destroy(pk);
        destroy(sk);
    }

    /// Вычислить остаток от деления на m числа обратного от x. (x^^-1) mod m
    static BigInt inverse(const BigInt x, const BigInt m) {
        BigInt a = new BigInt(0);
        BigInt b = new BigInt(m);
        BigInt x2 = new BigInt(x);
        BigInt u = new BigInt(1);
        BigInt q;
        while (x2.isPositive) {
            q = b / x2;  // integer division
            auto x2_copy = new BigInt(x2);
            auto a_copy = new BigInt(a);
            x2 = b % x2;
            a = new BigInt(u);
            b = new BigInt(x2_copy);
            u = a_copy - q * u;
        }
        if (b.isOne) return a % m;
        else return null;
    }

    /// Шифрование данных по открытому ключу
    BigInt encrypt(const BigInt data) const {
        assert(data < this.pk, "Слишком много данных для шифрования");
        return BigInt.powMod(data, e, pk);
    }

    /// Расшифровка шифротекста
    BigInt decrypt(const BigInt data) const {
        assert(sk !is null, "Закрытый ключ не был получен");
        return BigInt.powMod(data, sk, pk);
    }


    /// Создать новую пару ключей по двум простым числам p и q
    static RSA generate(const BigInt p, const BigInt q) {
        BigInt N = p * q;
        BigInt F = (p - 1) * (q - 1);
        BigInt d = RSA.inverse(RSA.e, F);
        assert(d !is null);
        return new RSA(N, d);
    }



    /*
    override size_t toHash() {
        // todo
    }


    override string toString() {
        // todo: make a useful toString()
        return "RSA";
    }
    */
}


unittest {
    RSA cipher1 = RSA.generate(new BigInt("0x4977c970792f309cdafac771"), new BigInt("0x12fe5633ca465bac1527ab47"));
    RSA cipher2 = new RSA(cipher1.pk);

    //BigInt random_data = new BigInt(0x6bc89abc);
    BigInt random_data = BigInt.randomCells(3);
    BigInt cipher_text = cipher2.encrypt(random_data);
    assert(cipher1.decrypt(cipher_text) == random_data);
}
