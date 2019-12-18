module bigint;

import std.stdio;
import std.conv : parse, text, emplace;
import std.math : ceil, abs, sgn;
import std.string : format;
import std.array : replace;
import std.random : uniform;
import std.file : readText, write, isFile, remove, FileException;

/// Класс BigInt - числа произвольной длины
class BigInt {
    private:
    static const ulong mask = ulong.max >> 32;
    static const ulong imask = ulong.max << 32;
    static const ulong dmask = mask + 1;

    static const ulong[] low_primes = [3, 5, 7, 11, 13, 17, 19, 23];

    ulong[] value;   /// Значение числа. Младшие элементы правее в числе
    ulong[] carry;      // for shifting

    ulong size = 1;                /// Количество элементов
    bool sign = false;              /// Знак числа: false - положительное, true - отрицательное

    public:
    static ulong mr_tests = 20;     /// Количество итераций теста Миллера-Рабина

    /// Пустой конструктор
    this() {
        sign = false;
        size = 1;
        value = new ulong[size];
    }
    this(const long num) {
        this();
        sign = sgn(num) < 0;
        value[0] = abs(num);
        this.oneshift();
    }
    /// Конструктор числа BigInt из строковой записи
    this(const string str) {
        ubyte base = 10;            // Основание числа в строке
        ubyte char_num = 10;        // Количество символов при разделении строки

        // Удаление вспомогательных символов
        auto snum = str.replace("_", "").replace(" ", "").replace(",", "").replace("\n", "");

        if (snum[0] == '-') {
            snum = snum[1..$];
            sign = true;
        } else if (snum[1] == '+') {
            snum = snum[1..$];
            sign = false;
        }

        if (snum[0..2] == "0x") {
            // Шестнадцатиричная запись
            snum = snum[2..$];
            base = 16;
            char_num = 8;
        } else if (snum[0..2] == "0b") {
            // Двоичная запись
            snum = snum[2..$];
            base = 2;
            char_num = 32;
        }

        // Количество символов (без последней ячейки, если она не полная - меньше чем char_num)
        immutable auto fulls_len = snum.length - snum.length % char_num;

        size = cast(ulong)(ceil( snum.length / cast(float)(char_num)));  // Количество ячеек, включая неполную
        value = new ulong[size];

        string t;   // Временная переменная для перевода ячеек snum в ulong

        // Ячейки по char_num символов переводятся в ulong
        for (auto i = 0; i < fulls_len; i += char_num) {
            t = snum[$ - i - char_num..$ - i];
            value[i/char_num] = parse!ulong( t, base);
        }

        // Если была неполная ячейка
        if (snum.length > fulls_len) {
            t = snum[0..$ - fulls_len];
            value[$-1] = parse!ulong(t, base);
        }

        this.oneshift();    // Перенос переполненных ячеек
    }
    /// Конструктор на основе массива
    this(const ulong[] arr) {
        size = arr.length;
        value = new ulong[size];
        value[] = arr[];
    }
    /// Копировать BigInt
    this(const BigInt binum) {
        sign = binum.sign;
        this(binum.value);
    }

    /// Деструктор
    ~this() {
        destroy(value);
        destroy(carry);
    }

    /// Сгенерировать случайный BigInt из cells ячеек
    static BigInt randomCells(const ulong cells) {
        BigInt res = new BigInt();
        res.size = cells;
        res.value.length = cells;
        foreach(ref item; res.value)
            item = uniform(0, mask);
        res.correct_size();
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
    override bool opEquals(const Object other) const {
        return this.opEquals( cast(BigInt)(other));
    }
    /// BigInt == BigInt
    bool opEquals(const BigInt other) const {
        return this.sign == other.sign && this.size == other.size && this.value == other.value;
    }
    /// BigInt == long
    bool opEquals(const long other) const {
        const ulong num = abs(other);           /// Модуль второго числа
        if (num > mask)   /// Число больше максимального значения ячейки => занимает две ячейки => перевести в BigInt
            return this == new BigInt(other);
        const bool osign = (sgn(other) < 0);    /// Знак второго числа: false - "+", true - "-"
        return size == 1 && value[0] == num && sign == osign;
    }
    /// BigInt == string
    bool opEquals(const string other) const {
        return this == new typeof(this)(other);
    }
    /// Равенство двух BigInt
    static bool opEquals(BigInt a, BigInt b) {
        return a.sign == b.sign && a.value == b.value;
    }

    override int opCmp(const Object other) const {
        return this.opCmp( cast(BigInt)(other));
    }
    /// Сравнение чисел BigInt - BigInt
    int opCmp(const BigInt other) const {
        if (this == other) return 0;
        else {
            if (this.size == other.size) {
                for (long i = size - 1; i >= 0; --i) {
                    if (value[i] > other.value[i])      // this > other
                        return sign ? -1 : 1;
                    else if (value[i] < other.value[i]) // this < other
                        return sign ? 1 : -1;
                    // Если значения старших ячеек равны, то перейти к следующей
                }
            } else if (this.sign < other.sign || (this.size > other.size && !sign) || (this.size < other.size && sign)) return 1;
            else return -1;

        }
        assert(0);
    }
    /// Сравнение чисел BigInt - long
    int opCmp(const long other) const {
        const ulong num = abs(other);
        if (num > mask)
            return this.opCmp( new BigInt( other));

        const bool osign = (sgn(other) < 0);

        if (this == num && sign == osign) return 0;
        else if (size > 1 || (value[0] > num && sign == osign))   return 1;   // BigInt > num
        else return -1;  // BigInt < num
    }
    /// Сравнение чисел BigInt - string
    int opCmp(const string other) const {
        return this.opCmp(new typeof(this)(other));
    }


    /// Унарные операции
    BigInt opUnary(const string op)() {
        if (op == "++") {
            // Инкремент
            return this += 1;
        } else if (op == "--") {
            // Декремент
            return this -= 1;
        } else assert(0, "Operator "~op~" not implemented");
    }

    /// Бинарные операции с присваиванием
    BigInt opOpAssign(const string op)(const BigInt other) {
        if (op == "+") return addAssign(other);     // Сложение
        else if (op == "-") return subAssign(other);     // Сложение
        else if (op == "*") return mulAssign(other);     // Умножение
        //else if (op == "^^") return powAssign(other);     // Возведение в степень
        else assert(0, "Operator "~op~" not implemented");
    }

    /// Бинарные операции с присваиванием для ulong
    BigInt opOpAssign(const string op)(const ulong other) {
        if (op == "+") return addAssign( other);                // Сложение
        else if (op == "-") return subAssign( other);           // Сложение
        else if (op == "*") return mulAssign( other);           // Умножение
        else if (op == ">>") return rBitShiftAssign( other);    // Сдвиг вправо
        else if (op == "<<") return lBitShiftAssign( other);    // Сдвиг влево
        else assert(0, "Operator "~op~" not implemented");
    }

    /// Операция сложения с присваиванием
    BigInt addAssign(const BigInt other) {
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
    BigInt addAssign(const long other) {
        const ulong num = abs(other);
        if (num > mask)
            return this += new BigInt(other);
        const bool osign = (sgn(other) < 0);

        if (osign == this.sign) {
            value[0] += num;
            oneshift();
            //correct_size();
        } else {
            this -= num;
        }
        return this;
    }

    /// Операция вычитания с присваиванием
    BigInt subAssign(const BigInt other) {
        if (other.isZero) return this;

        if (this == other) {
            setZero();
        } else if (this.sign == other.getSign) {
            ulong j;
            if((sign && this < other) || (!sign && this > other)) {
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
                ulong[] ovalue = new ulong[other.size];
                ovalue[] = other.value[];
                for(ulong i = 0; i < size; ++i) {
                    if(value[i] > ovalue[i]) {
                        ovalue[i] += dmask;
                        for (j = 1; ovalue[i + j] == 0; ++j)
                            ovalue[i + j] += dmask;
                        ovalue[i+1..i+j+1] -= 1;
                    }
                    ovalue[i] -= value[i];
                }
                size = other.size;
                value = ovalue;
            } else {
                assert(0);
            }
            correct_size();
            oneshift();
        } else {
            this += other.inv;
        }
        return this;
    }
    /// Операция вычитания с присваиванием long
    BigInt subAssign(const long other) {
        const ulong num = abs(other);
        if (num > mask)
            return this -= new BigInt(other);
        const bool osign = (sgn(other) < 0);

        if (size == 1 && value[0] <= num && sign == osign) {
            sign = !sign;
            value[0] = num - value[0];
        } else if (sign != osign) {
            this += num;
        } else if (num > 0) {
            if (value[0] < num) {
                ulong j = 1;
                value[0] += dmask;

                // Занять из старших разрядов, пока не будет найден ненулевой разряд
                for (; value[j] == 0; ++j)
                    value[j] += dmask;

                value[1..j+1] -= 1;
            }
            value[0] -= num;
            //oneshift();
            correct_size();
        }
        return this;
    }

    /// Операция умножения с присваиванием
    BigInt mulAssign(const BigInt other) {
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
    /// Операция умножения с присваиванием long
    BigInt mulAssign(const long other) {
        if (other == 0)
            setZero();
        else {
            sign = sign ^ (sgn(other) < 0);
            value[0..size] *= abs(other);
            oneshift();
        }
        return this;
    }

    /// Битовый сдвиг вправо
    BigInt rBitShiftAssign(const ulong other) {
        // TODO: Сдвиг > 32 бит
        assert (other <= 32, "Сдвиг на "~text(other)~" не входит в интервал '0..32'");
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
            destroy(carry);
            correct_size();
        }
        return this;
    }
    /// Битовый сдвиг влево
    BigInt lBitShiftAssign(const ulong other) {
        value[] *= 2 ^^ other;
        oneshift();
        return this;
    }

    /// Получение результата деления на other
    BigInt div(const BigInt other) const {
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
                auto temp = new BigInt(this);
                temp.sign = this.sign ^ other.sign;
                return temp;
            case -1:
                return new BigInt();
            default:
                assert(0);
        }
    }

    /// Получение остатка от деления на other
    BigInt mod(const BigInt other) const {
        assert (!other.isZero, "Деление на 0");
        assert(other.isPositive, "Получение остатка от деления на отрицательное число не реализовано"); // TODO
        if (this == other || other.isOne) {
            return new BigInt(0);
        } else if (!sign && this < other) {
            return new BigInt( this);
        } else if (sign && this.inv < other) {
            return other + this;
        } else {
            BigInt remainder = new BigInt();
            for (long i = size * 32 - 1; i >= 0; --i) {
                remainder <<= 1;
                remainder.setLowBit(this.getBit(i));
                if (remainder >= other)
                    remainder -= other;
            }
            return sign ? other - remainder : remainder;
        }
        assert(0);
    }

    /// Остаток от деления на ulong
    BigInt mod(const long other) const {
        assert (other > 0, "Деление на 0");
        return this % new BigInt(other);
    }

    /// Возведение числа в степень
    static BigInt pow(const BigInt left, const BigInt right) {
        assert(!right.sign);
        if (right.isZero)
            return new BigInt( 1);
        if (right.isOne)
            return new BigInt(left);

        if (right.isOdd)
            return (left ^^ (right - 1)) * left;
        else
            return (left * left) ^^ (right >> 1);
    }

    /// Возведение в степень по модулю
    static BigInt powMod(const BigInt left, const BigInt right, const BigInt modulo) {
        if (right.isZero)
            return new BigInt(1);
        if (right.isOne)
            return new BigInt(left) % modulo;

        if (right.isOdd)
            return (powMod(left, (right - 1), modulo) * left) % modulo;
        else
            return powMod((left * left) % modulo, right >> 1, modulo) % modulo;
    }

    /// Бинарные операции BigInt - BigInt
    BigInt opBinary(const string op)(const BigInt other) const {
        if (op == "+") return new BigInt( this) += other;       // Сложение
        else if (op == "-") return new BigInt( this) -= other;  // Сложение
        else if (op == "*") return new BigInt( this) *= other;  // Умножение
        else if (op == "/") return div(other);                  // Деление
        else if (op == "%") return mod(other);                  // Остаток от деления
        else if (op == "^^") return pow(this, other);          // Возведение в степень
        else assert(0, "Operator "~op~" not implemented");
    }

    /// Бинарные операции BigInt - ulong
    BigInt opBinary(const string op)(const long other) const {
        if (op == "+") return new BigInt(this) += other;           // Сложение
        else if (op == "-") return new BigInt(this) -= other;      // Сложение
        else if (op == "*") return new BigInt(this) *= other;      // Умножение
        else if (op == "%") return mod(other);                     // Остаток от деления
        else if (op == ">>") return new BigInt(this) >>= other;    // Сдвиг вправо
        else if (op == "<<") return new BigInt(this) <<= other;    // Сдвиг влево
        else assert(0, "Operator "~op~" not implemented");
    }

    /// Число с обратным знаком
    BigInt inv() const {
        BigInt cp = new BigInt(this);
        cp.sign = !sign;
        return cp;
    }

    /// Является ли число нечетным
    bool isOdd() const {
        return value[0] & 1;
    }

    /// Является ли число единицей
    bool isOne() const {
        return !sign && size == 1 && value[0] == 1;
    }
    /// Присвоить числу единицу
    BigInt setOne() {
        sign = false;
        size = 1;
        value = [ 1];
        return this;
    }
    /// Является ли число нулем
    bool isZero() const {
        return size == 1 && value[0] == 0;
    }
    /// Обнулить число
    BigInt setZero() {
        sign = false;
        size = 1;
        value = new ulong[size];
        return this;
    }

    /// Получить значение бита в разряде num в числе
    bool getBit(const ulong num) const {
        const auto cell = num / 32;
        assert (cell < size, "Диапазон возможных бит: [0.."~text( size * 32 - 1)~"], передано: "~text( num));
        return (value[cell] >> (num % 32)) & 1;
    }

    /// Установить бит в разряде num в значение bit
    void setBit(const ulong num, const bool bit) {
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

    /// Установить младший бит
    void setLowBit(const bool bit) {
        if(bit)
            value[0] |= bit;
        else
            value[0] &= ulong.max - 1;
    }

    /// Проверка, является ли число простым
    bool isPrime(const ulong tests) const {
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
    bool isPrime() const {
        return isPrime(mr_tests);
    }

    /// Тест Миллера-Рабина на простоту числа. k - точность (количество итераций)
    bool miller_rabin(const ulong k) const {
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
    BigInt nextPrime(const ulong tests) const {
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

    /// Поиск следующего простого числа
    BigInt nextPrime() const {
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

    /// Загрузить число из файла
    static BigInt fromFile(const string fileName) {
        return new BigInt(fileName.readText);
    }

    /// Сохранить число в файле
    void toFile(const string fileName) const {
        fileName.write(hex(false));
    }

    /// Шетнадцатиричное представление числа
    string hex() const {
        string res = sign ? "-0x" : "0x";
        for (ulong i = size; i > 0; --i)
            res ~= format!"%08x "( value[i-1]);
        return res;
    }

    /// Шетнадцатиричное представление числа. divide - разделять ли ячейки пробелами
    string hex(bool divide) const {
        string res = sign ? "-0x" : "0x";
        if(divide)
            for (ulong i = size; i > 0; --i)
                res ~= format!"%08x "( value[i-1]);
        else
            for (ulong i = size; i > 0; --i)
                res ~= format!"%08x"( value[i-1]);
        return res;
    }

    /// Двоичное представление числа
    string bin() const {
        string res = sign ? "-0b" : "0b";
        for (ulong i = size; i > 0; --i)
            res ~= format!"%032b "( value[i-1]);
        return res;
    }

    override string toString() const {
        return hex();
    }

    string toString(bool in_hex) const {
        if (in_hex)
            return hex();
        else
            return bin();
    }

}

class BigText : BigInt {
    private:
    static const ulong char_mask = (1 << 8) - 1;    /// Маска для одного (однобайтового) символа

    ulong length = 0;   /// Количество символов (подъячеек)

    public:
    /// Создать BigInt из пользовательского текста
    this(const string str) {
        length = str.length;
        size = calcSize(length);  // Количество ячеек, включая неполную (по четыре символа в ячейке)
        value = new ulong[size];

        ulong fulls = length / 4;
        foreach(i, c; str) {
            value[i >> 2] += cast(ulong)(c) << (8 * (i % 4));
        }
    }

    this(const string str, bool isBigInt) {
        if(isBigInt)
            super(str);
        else
            this(str);
    }

    this(const BigInt binum) {
        super(binum);
    }

    static ulong calcSize(ulong n) {
        return cast(ulong)(ceil(n / 4f));
    }

    ulong calcLength() {
        length = size * 4;
        for(long i = length - 1; i >= 0; --i) {
            if (!(value[i>>2] & (char_mask << 8 * (i%4))))
                --length;
            else
                break;
        }
        return length;
    }

    string toString() {
        calcLength();
        char[] res = new char[length];
        foreach(i, ref c; res) {
            c = (value[i>>2] >> 8*(i%4)) & char_mask;
        }
        return cast(string)(res);
    }

    override string toString() const {
        char[] res = new char[length];
        foreach(i, ref c; res) {
            c = (value[i>>2] >> 8*(i%4)) & char_mask;
        }
        return cast(string)(res);
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
    assert((number1 + -20) == 11);
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
    if(num.size < 8)
        writeln("Неверный размер при randomCells: ", num);
    assert(num.size == 8);

    string f = "BigInt";
    while(true)
        try {
            (f~".test").isFile;
            f ~= uniform!"[]"('A', 'Z');
        } catch (FileException e) {
            break;
        } finally {
            f ~= ".test";
        }

    num.toFile(f);
    assert(num == BigInt.fromFile(f));
    f.remove;


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

    assert(new BigInt("0x0005818e 136961ca 09976068") % new BigInt("0x00000559 65d2e6dd") == 0x6bc89abc);
    assert(new BigInt("0x00000559 80c50d8c") > new BigInt("0x00000559 65d2e6dd"));
}

