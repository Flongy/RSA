module main;

import std.stdio;
import std.datetime.stopwatch;
import std.random;
import core.time;
import std.parallelism;
import std.conv;
import std.algorithm : min;
import std.math : ceil;
import core.stdc.stdlib : exit;
import std.array;
import std.string : isNumeric;

import bigint;


/// Вычислить остаток от деления на m числа обратного от x. (x^^-1) mod m
BigInt inverse(BigInt x, BigInt m) {
    BigInt a = new BigInt(0);
    BigInt b = new BigInt(m);
    BigInt u = new BigInt(1);
    BigInt q;
    while (x.isPositive) {
        q = b / x;  // integer division
        auto xc = new BigInt(x);
        auto ac = new BigInt(a);
        x = b % x;
        a = new BigInt(u);
        b = new BigInt(xc);
        u = ac - q * u;
    }
    if (b.isOne) return a % m;
    else return null;
}

void main()
{
    auto sw = StopWatch(AutoStart.no);
    BigInt e = new BigInt(65537);
    BigInt prime1 = new BigInt("0x4977c970792f309cdafac771");
    BigInt prime2 = new BigInt("0x12fe5633ca465bac1527ab47");
    BigInt prime3 = new BigInt("0x90a7c97a79253f9cdbfacb9b");
    BigInt prime4 = new BigInt("0xb0125073b1283fabdbf96b47");

    BigInt n1 = new BigInt(3557);
    BigInt n2 = new BigInt(2579);

    sw.start();
    auto N = prime1 * prime2;
    auto F = (prime1 - 1) * (prime2 - 1);
    auto d = inverse(e, F);
    sw.stop();
    writeln("d = ", d);
    writeln(prime1, " является простым: ", prime1.isPrime);
    writeln(prime2, " является простым: ", prime2.isPrime);
    writeln(prime3, " является простым: ", prime3.isPrime);
    writeln(prime4, " является простым: ", prime4.isPrime);
    writeln("time = ", sw.peek.total!"nsecs", " ", sw.peek.total!"usecs" / 1_000_000f);
    sw.reset();
}
