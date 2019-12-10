module bigint;

import std.stdio;
import std.conv : parse, text;
import std.math : ceil, abs, sgn;
import std.string : format;
import std.array : replace;
import std.random : uniform;

/// Класс BigInt - числа произвольной длины
class BigInt {
    private:
    static const ulong mask = ulong.max >> 32;
    static const ulong imask = ulong.max << 32;
    static const ulong dmask = mask + 1;

    static const ulong[] low_primes = [3, 5, 7, 11, 13, 17, 19, 23];

    ulong[] value;      /// Значение числа. Младшие элементы правее в числе
    ulong[] carry;      // for shifting

    size_t size;                    /// Количество элементов
    bool sign = false;              /// Знак числа: false - положительное, true - отрицательное

    public:
    static ulong mr_tests = 15;     /// Количество итераций теста Миллера-Рабина

    /// Пустой конструктор
    this() {
        size = 1;
        value = new ulong[size];
    }
    this(int num) {
        sign = sgn(num) < 0;
        this( cast(ulong) abs( num));
    }
    this(uint num) {
        this( cast(ulong) num);
    }
    this(long num) {
        sign = sgn(num) < 0;
        this( cast(ulong) abs( num));
    }
    this(ulong num) {
        this();
        value[0] = num;
        this.oneshift();
    }
    /// Конструктор числа BigInt из строковой записи
    this(string str) {
        ubyte base = 10;            // Основание числа в строке
        ubyte char_num = 10;        // Количество символов при разделении строки

        // Удаление вспомогательных символов
        str = str.replace("_", "").replace(" ", "").replace(",", "");

        if (str[0] == '-') {
            str = str[1..$];
            sign = true;
        } else if (str[1] == '+') {
            str = str[1..$];
            sign = false;
        }

        if (str[0..2] == "0x") {
            // Шестнадцатиричная запись
            str = str[2..$];
            base = 16;
            char_num = 8;
        } else if (str[0..2] == "0b") {
            // Двоичная запись
            str = str[2..$];
            base = 2;
            char_num = 32;
        }

        // Количество символов (без последней ячейки, если она не полная - меньше чем char_num)
        immutable auto fulls_len = str.length - str.length % char_num;

        size = cast(ulong)(ceil( str.length / cast(float)(char_num)));
        value = new ulong[size];

        string t;   // Временная переменная для перевода ячеек str в ulong

        // Ячейки по char_num символов переводятся в ulong
        for (auto i = 0; i < fulls_len; i += char_num) {
            t = str[$ - i - char_num..$ - i];
            value[i/char_num] = parse!ulong( t, base);
        }

        // Если была неполная ячейка
        if (str.length > fulls_len) {
            t = str[0..$ - fulls_len];
            value[$-1] = parse!ulong( t, base);
        }

        this.oneshift();    // Перенос переполненных ячеек
    }
    /// Конструктор на основе массива
    this(ulong[] arr) {
        size = arr.length;
        value = new ulong[size];
        value[] = arr[];
    }
    /// Копировать BigInt
    this(BigInt binum) {
        sign = binum.sign;
        this(binum.value);
    }

    /// Деструктор
    ~this() {
        destroy( value);
        destroy( carry);
    }

    /// Сгенерировать случайный BigInt из cells ячеек
    static BigInt randomCells(ulong cells) {
        BigInt res = new BigInt();
        res.size = cells;
        res.value.length = cells;
        foreach(ref item; res.value)
            item = uniform(0, mask);

        return res;
    }

    /// Перенос переполненных ячеек в старшие
    void shift() {
        carry[] = (value[0..$-1] & imask) / dmask;

        value[0..$-1] &= mask;
        value[1..$] += carry[];
    }
    /// Перенос переполненных ячеек в старшие массива arr
    void shift(ulong[] arr) {
        carry[] = (arr[0..$-1] & imask) / dmask;

        arr[0..$-1] &= mask;
        arr[1..$] += carry[];
    }
    /// Перенос переполненных ячеек в старшие с подготовкой вспомогательных массивов
    void pshift() {
        if (value[$-1] & imask) {
            size++;
            value.length++;
        }
        carry = new ulong[size - 1];

        shift();
    }
    /// Перенос переполненных ячеек в старшие с подготовкой и удалением вспомогательных массивов
    void oneshift() {
        pshift();
        destroy(carry);
    }

    /// Удаление нулевых ячеек в старших элементах
    void correct_size() {
        for (auto i = size; i > 1 && value[i-1] == 0; --i)
            --value.length;
        size = value.length;
    }

    /// Проверка равенства
    override bool opEquals(Object other) const {
        return this.opEquals( cast(BigInt)(other));
    }
    /// BigInt == BigInt
    bool opEquals(BigInt other) const {
        return this.sign == other.sign && this.size == other.size && this.value == other.value;
    }
    ///// BigInt == ulong
    //bool opEquals(ulong other) const {
    //    if (other > mask)
    //        return this == new BigInt( other);
    //    return size > 1 || value[0] == other;
    //}
    /// BigInt == long
    bool opEquals(long other) const {
        bool osign = (sgn(other) < 0);
        other = abs(other);
        if (other > mask)
            return this == new BigInt( other);
        return size > 1 || (value[0] == other && sign == osign);
    }
    /// BigInt == string
    bool opEquals(string other) const {
        return this == new BigInt(other);
    }
    /// Равенство двух BigInt
    static bool opEquals(BigInt a, BigInt b) {
        return a.sign == b.sign && a.value == b.value;
    }

    override int opCmp(Object other) const {
        return this.opCmp( cast(BigInt)(other));
    }
    /// Сравнение чисел BigInt - BigInt
    int opCmp(BigInt other) const {
        if (this == other) return 0;
        else {
            if (this.size == other.size) {
                for (ulong i = size; i > 0; ++i) {
                    if (value[i - 1] > other.value[i - 1])
                    // this > other
                        return sign ? -1 : 1;
                    else
                    // this < other
                        return sign ? 1 : -1;
                }
            } else if (this.sign < other.sign || (this.size > other.size && !sign) || (this.size < other.size && sign)) return 1;
            else return -1;

        }
        assert(0);
    }
    /// Сравнение чисел BigInt - ulong
    //int opCmp(ulong other) const {
    //    if (other > mask)
    //        return this.opCmp( new BigInt( other));
    //    if (this == other && !sign) return 0;
    //    else if (size > 1 || (value[0] > other && !sign))   return 1;   // BigInt > num
    //    else return -1;  // BigInt < num
    //}
    /// Сравнение чисел BigInt - long
    int opCmp(long other) const {
        if (other > mask)
            return this.opCmp( new BigInt( other));

        bool osign = (sgn(other) < 0);
        other = abs(other);

        if (this == other && sign == osign) return 0;
        else if (size > 1 || (value[0] > other && sign == osign))   return 1;   // BigInt > num
        else return -1;  // BigInt < num
    }
    /// Сравнение чисел BigInt - string
    int opCmp(string other) const {
        return this.opCmp(new BigInt(other));
    }


    /// Унарные операции
    BigInt opUnary(string op)() {
        if (op == "++") {
            // Инкремент
            return this += 1;
        } else if (op == "--") {
            // Декремент
            return this -= 1;
        } else assert(0, "Operator "~op~" not implemented");
    }

    /// Бинарные операции с присваиванием
    BigInt opOpAssign(string op)(BigInt other) {
        if (op == "+") return addAssign( other);     // Сложение
        else if (op == "-") return subAssign( other);     // Сложение
        else if (op == "*") return mulAssign( other);     // Умножение
        //else if (op == "^^") return powAssign(other);     // Возведение в степень
        else assert(0, "Operator "~op~" not implemented");
    }

    /// Бинарные операции с присваиванием для ulong
    BigInt opOpAssign(string op)(ulong other) {
        if (op == "+") return addAssign( other);                // Сложение
        else if (op == "-") return subAssign( other);           // Сложение
        else if (op == "*") return mulAssign( other);           // Умножение
        else if (op == ">>") return rBitShiftAssign( other);    // Сдвиг вправо
        else if (op == "<<") return lBitShiftAssign( other);    // Сдвиг влево
        else assert(0, "Operator "~op~" not implemented");
    }

    /// Операция сложения с присваиванием
    BigInt addAssign(BigInt other) {
        if (this.sign == other.sign) {
            if (other.size > size) {
                size = other.size;
                value.length = size;
                value[] += other.value[];
            } else {
                value[0..other.size] += other.value[];
            }
            oneshift();
            //correct_size();
        } else {
            this -= other.inv;
        }
        return this;
    }
    /// Операция сложения с присваиванием ulong
    BigInt addAssign(long other) {
        if (other > mask)
            return this += new BigInt( other);

        const bool osign = (sgn(other) < 0);
        other = abs(other);
        if (osign == this.sign) {
            value[0] += other;
            oneshift();
            //correct_size();
        } else {
            this -= other;
        }
        return this;
    }

    /// Операция вычитания с присваиванием
    BigInt subAssign(BigInt other) {
        if (other.isZero) return this;

        if (this == other) {
            setZero();
        } else if (this.sign == other.sign) {
            ulong j;
            if((sign && this < other) || (!sign && this > other)) {
                //writeln((sign && this < other), (!sign && this > other));
                //writeln(this.sign ? "-" : "", this.value, other.sign ? " -" : " ", other.value);
                for(int i = 0; i < other.size; ++i) {
                    if(value[i] < other.value[i]) {
                        value[i] += dmask;
                        for (j = 1; value[i + j] == 0; ++j)
                            value[i + j] += dmask;
                        value[i+1..i+j+1] -= 1;
                    }
                    value[i] -= other.value[i];
                }
            } else if((sign && this > other) || (!sign && this < other)) {
                sign = !sign;
                if (size < other.size) {
                    this.value.length = other.size;
                }
                for(int i = 0; i < other.size; ++i) {
                    if(value[i] > other.value[i]) {
                        writeln(value, " ", other.value);
                        other.value[i] += dmask;
                        for (j = 1; other.value[i + j] == 0; ++j)
                            other.value[i + j] += dmask;
                        other.value[i+1..i+j+1] -= 1;
                    }
                    value[i] = other.value[i] - value[i];
                }
                if (size < other.size) {
                    value[this.size..$] = other.value[this.size..$];
                    this.size = other.size;
                }
            } else {
                assert(0, "HELP");
            }
            correct_size();
            oneshift();
        } else if (this.sign != other.sign) {
            this += other.inv;
        } else if (!other.isZero) {
            ulong j;
            foreach (i, item; other.value) {
                if (item > value[i]) {
                    value[i] += dmask;
                    for (j = 1; value[i + j] == 0; ++j)
                        value[i + j] += dmask;
                    value[i+1..i+j+1] -= 1;
                }
                value[i] -= item;
            }
            //oneshift();
            correct_size();
        }
        return this;
    }
    /// Операция вычитания с присваиванием long
    BigInt subAssign(long other) {
        if (other > mask)
            return this -= new BigInt( other);
        const bool osign = (sgn(other) < 0);
        other = abs(other);

        if (size == 1 && value[0] <= other && sign == osign) {
            sign = !sign;
            value[0] = other - value[0];
        } else if (sign != osign) {
            this += other;
        } else if (other > 0) {
            if (value[0] == 0) {
                ulong j = 1;
                value[0] += dmask;
                for (; value[j] == 0; ++j)
                // Занять из старших разрядов, пока не будет найден ненулевой разряд
                    value[j] += dmask;
                value[1..j+1] -= 1;
            }
            value[0] -= other;
            correct_size();
            //oneshift();
            correct_size();
        }
        return this;
    }

    /// Операция умножения с присваиванием
    BigInt mulAssign(BigInt other) {
        if (other.isZero) {
            setZero();
        } else {
            auto new_size = size + other.size;
            auto result = new ulong[new_size];

            carry = new ulong[new_size - 1];

            foreach (i, cell; other.value) {
                result[i..i + size] += value[] * cell;
                shift(result);
            }

            destroy(carry);

            sign = this.sign ^ other.sign;
            size = new_size;
            value = result;

            oneshift();
            correct_size();
        }
        return this;
    }
    /// Операция умножения с присваиванием ulong
    BigInt mulAssign(long other) {
        if (other == 0)
            setZero();
        else {
            sign = sign ^ (sgn(other) < 0);
            other = abs(other);

            value[0..size] *= other;
            oneshift();
        }
        return this;
    }

    /// Битовый сдвиг вправо
    BigInt rBitShiftAssign(ulong other) {
        // TODO: Сдвиг > 32 бит
        assert (other <= 32, "Сдвиг на "~text( other)~" не входит в интервал '0..32'");
        if (other == 32) {
            if (size > 1) {
                --size;
                value[0..$-1] = value[1..$];
                --value.length;
            } else
                value[0] = 0;
        } else if (other > 0) {
            const ulong shift_mask = (1 << other) - 1;
            carry = new ulong[size - 1];
            carry[] = value[1..$] & shift_mask;
            value[] /= 2 ^^ other;
            value[0..$-1] += carry[] * 2 ^^ (32 - other);
            destroy( carry);
            correct_size();
        }
        return this;
    }
    /// Битовый сдвиг влево
    BigInt lBitShiftAssign(ulong other) {
        value[] *= 2 ^^ other;
        oneshift();
        return this;
    }

    /// Получение результата деления на other
    BigInt div(BigInt other) {
        assert (!other.isZero, "Деление на 0");
        if(other.isOne) {
            auto temp = new BigInt(this);
            return temp;
        }
        switch(this.opCmp(other)) {
            case 1:
                BigInt quotient = new BigInt();
                BigInt remainder = new BigInt();

                quotient.sign = this.sign ^ other.sign;

                for (long i = size * 32 - 1; i >= 0; --i) {
                    remainder <<= 1;
                    remainder.setLowBit( this.getBit( i));
                    if (remainder >= other) {
                        remainder -= other;
                        quotient.setBit( i, true);
                    }
                }
                return quotient;
            case 0:
                auto temp = new BigInt( this);
                temp.sign = this.sign ^ other.sign;
                return temp;
            case -1:
                return new BigInt();
            default:
                assert(0);
        }
    }

    /// Получение остатка от деления на other
    BigInt mod(BigInt other) {
        assert (!other.isZero, "Деление на 0");
        if (this > other || this.sign) {
            BigInt remainder = new BigInt();
            for (long i = size * 32 - 1; i >= 0; --i) {
                remainder <<= 1;
                remainder.setLowBit( this.getBit( i));
                if (remainder >= other)
                    remainder -= other;
            }
            return sign ? other - remainder : remainder;
        }
        return new BigInt( this);
    }

    /// Остаток от деления на ulong
    BigInt mod(ulong other) {
        assert (other > 0, "Деление на 0");
        return this % new BigInt(other);
    }

    /// Возведение числа в степень
    static BigInt pow(BigInt left, BigInt right) {
        assert(!right.sign);
        if (right.isZero)
            return new BigInt( 1);
        if (right.isOne)
            return left;

        if (right.isOdd)
            return (left ^^ (right - 1)) * left;
        else
            return (left * left) ^^ (right >> 1);
    }

    /// Возведение в степень по модулю
    static BigInt powMod(BigInt left, BigInt right, BigInt modulo) {
        if (right.isZero)
            return new BigInt( 1);
        if (right.isOne)
            return left;

        if (right.isOdd)
            return (powMod( left, (right - 1), modulo) * left) % modulo;
        else
            return (powMod( (left * left) % modulo, right >> 1, modulo)) % modulo;
    }

    /// Бинарные операции BigInt - BigInt
    BigInt opBinary(string op)(BigInt other) {
        if (op == "+") return new BigInt( this) += other;       // Сложение
        else if (op == "-") return new BigInt( this) -= other;  // Сложение
        else if (op == "*") return new BigInt( this) *= other;  // Умножение
        else if (op == "/") return div(other);                  // Деление
        else if (op == "%") return mod(other);                  // Остаток от деления
        else if (op == "^^") return pow( this, other);          // Возведение в степень
        else assert(0, "Operator "~op~" not implemented");
    }

    /// Бинарные операции BigInt - ulong
    BigInt opBinary(string op)(ulong other) {
        if (op == "+") return new BigInt( this) += other;           // Сложение
        else if (op == "-") return new BigInt( this) -= other;      // Сложение
        else if (op == "*") return new BigInt( this) *= other;      // Умножение
        else if (op == "%") return mod(other);                      // Остаток от деления
        else if (op == ">>") return new BigInt( this) >>= other;    // Сдвиг вправо
        else if (op == "<<") return new BigInt( this) <<= other;    // Сдвиг влево
        else assert(0, "Operator "~op~" not implemented");
    }

    /// Число с обратным знаком
    BigInt inv() {
        BigInt cp = new BigInt(this);
        cp.sign = !sign;
        return cp;
    }

    /// Является ли число нечетным
    bool isOdd() const {
        return value[0] & 1;
    }

    bool isOne() const {
        /* Является ли число единицей */
        return !sign && size == 1 && value[0] == 1;
    }
    BigInt setOne() {
        /* Присвоить числу единицу */
        sign = false;
        size = 1;
        value = [ 1];
        return this;
    }
    bool isZero() const {
        /* Является ли число нулем */
        return size == 1 && value[0] == 0;
    }
    BigInt setZero() {
        /* Обнулить число */
        sign = false;
        size = 1;
        value = new ulong[size];
        return this;
    }

    bool getBit(ulong num) const {
        /* Получить значение бита в разряде num в числе */
        const auto cell = num / 32;
        assert (cell < size, "Диапазон возможных бит: [0.."~text( size * 32 - 1)~"], передано: "~text( num));
        return (value[cell] >> (num % 32)) & 1;
    }

    void setBit(ulong num, bool bit) {
        /* Установить бит в разряде num в значение bit */
        const auto cell = num / 32;
        const auto bit_mask = 1 << (num % 32) & mask;
        if (cell >= size && bit) {
            size = cell + 1;
            value.length = size;
            value[cell] |=  bit_mask;
        } else if (bit)
            value[cell] |=  bit_mask;
        else if (!bit && cell < size)
                value[cell] &= ~bit_mask;
    }

    void setLowBit(bool bit) {
        /* Установить младший бит */
        if(bit)
            value[0] |= bit;
        else
            value[0] &= ulong.max - 1;
    }

    /// Проверка, является ли число простым
    bool isPrime(ulong tests) {
        if (!isOdd)
            return false;

        foreach(item; low_primes) {
            const auto t = (this % item);
            if(t.isZero)
                return false;
        }
        return miller_rabin(tests);
    }

    /// Проверка, является ли число простым
    bool isPrime() {
        return isPrime(mr_tests);
    }

    /// Тест Миллера-Рабина на простоту числа. k - точность (количество итераций)
    bool miller_rabin(ulong k) {
        const BigInt oneLess = new BigInt(this) - 1;
        BigInt d = this - 1;
        while ((d % 2).isZero)
            d >>= 1;

        for(ulong i = 0; i < k; ++i) {
            BigInt a;
            if (size == 1) {
                a = BigInt.randomCells(1);
                while(a > this)
                    a = BigInt.randomCells(1);
            } else {
                a = BigInt.randomCells(uniform(1, size));
                while(a > this)
                    a = BigInt.randomCells(uniform(1, size));
            }

            BigInt x = powMod(a, d, this);

            if (x.isOne || x == oneLess)
                return true;
            while (d != oneLess)
            {
                x = (x * x) % this;
                d <<= 1;

                if (x.isOne)      return false;
                if (x == oneLess)    return true;
            }
        }
        return false;
    }

    /// Поиск следующего простого числа
    BigInt nextPrime(ulong tests) {
        if (isPrime(tests))
            return this;
        else {
            BigInt next;
            if(isOdd)
                next = new BigInt(this);
            else
                next = this + 1;

            while(true)
                if(next.isPrime(tests))
                    return next;
                else
                    next += 2;
        }
    }

    /// Поиск следующего простого числа
    BigInt nextPrime() {
        return nextPrime(mr_tests);
    }

    /// Число положительное
    bool isPositive() const {
        return !(isZero || sign);
    }

    /// Число отрицательное
    bool isNegative() const {
        return !isZero && sign;
    }

    /// Знак числа: false - положительное ('+'), true - отрицательное ('-')
    bool getSign() const {
        return sign;
    }

    /// Количество ячеек в числе
    ulong getSize() const {
        return size;
    }


    /*override size_t toHash() {
        // todo
    }*/

    /// Шетнадцатиричное представление числа
    string hex() {
        string res = sign ? "-0x" : "0x";
        for (auto i = size; i > 0; --i)
            res ~= format!"%08x "( value[i-1]);
        return res;
    }

    /// Двоичное представление числа
    string bin() {
        string res = sign ? "-0b" : "0b";
        for (auto i = size; i > 0; --i)
            res ~= format!"%032b "( value[i-1]);
        return res;
    }

    override string toString() {
        return hex();
    }

    string toString(bool in_hex) {
        if (in_hex)
            return hex();
        else
            return bin();
    }

}

/// Проверка класса BigInt с одной ячейкой
unittest {
    BigInt num = new BigInt();
    assert(num.isZero);

    num = new BigInt(10);
    assert(num == 10);

    num = new BigInt("0xFF");
    assert(num == 0xFF);

    num = new BigInt("0b11110000");
    assert(num == 0b11110000);

    num = new BigInt(128);
    ++num;
    assert(num == 129);
    --num;
    assert(num == 128);

    num <<= 3;
    assert(num == 1024);
    num >>= 2;
    assert(num == 256);

    num.setLowBit(true);
    assert(num == 257);

    num.setBit(3, true);
    assert(num == 265);

    num.setLowBit(false);
    assert(num == 264);

    num.setBit(8, false);
    assert(num == 8);

    num += 3;
    assert(num == 11);

    num -= 5;
    assert(num == 6);

    num *= 10;
    assert(num == 60);

    assert(num % 9 == 6);



    BigInt number1 = new BigInt(31);
    BigInt number2 = new BigInt(5);
    BigInt number3 = new BigInt(-14);

    assert(number1 == 31);
    assert(number2 == 5);

    assert((number1 + number2) == 36);
    assert((number1 + number2.inv) == 26);
    assert(number1 + -20 == 11);
    assert((number1 - number2) == 26);
    assert((number2 - number1) == -26);
    assert(((number2 - number1) - number2) == -31);
    assert((number2 - (number2 - number1)) == 31);
    assert((number2 - 15) == -10);
    assert((number2 - -15) == 20);
    assert((number1 * number2) == 155);
    assert((number1 / number2) == 6);
    assert((number1 % number2) == 1);
    assert((number3 % number1) == 17);
    assert((number3 % number2) == 1);
    assert((number1 ^^ number2) == 28_629_151);
    assert(BigInt.powMod(number1, number2, number2) == 1);
}

/// Проверка класса BigInt с несколькими ячейками
unittest {
    BigInt num = BigInt.randomCells(8);
    assert(num.size == 8);

    BigInt number1 = new BigInt("0x9010 fa00 23ab");
    BigInt number2 = new BigInt(0x3BB5_B0C5);

    assert(number1 > number2);
    assert(number2 < number1);
    assert(number1.inv < number2.inv);
    assert(num.inv < number1.inv);

    assert(number1 == "0x9010 fa00 23ab");
    assert(number2 == 0x3BB5_B0C5);

    assert((number1 + number2) == 0x9011_35B5_D470);
    assert((number1 - number2) == 0x9010_BE4A_72E6);
    assert((number1 * number2) == "0x219a 2919 5342 1b88 0297");
    assert((number1 / number2) == 0x269AB);
    assert((number1 % number2) == 0x3098_4314);
    assert(BigInt.powMod(number1, number2, number2) == 0x3579_5215);
}

