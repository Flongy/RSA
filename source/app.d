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
import rsa;



void main()
{
    auto sw = StopWatch(AutoStart.no);
    const BigInt prime1 = new BigInt("0x4977c970792f309cdafac771");
    const BigInt prime2 = new BigInt("0x12fe5633ca465bac1527ab47");
    const BigInt prime3 = new BigInt("0x90a7c97a79253f9cdbfacb9b");
    const BigInt prime4 = new BigInt("0xb0125073b1283fabdbf96b47");


    string s = "Hello, world!";
    writefln("%d", s[0]);
    sw.start();
    //writeln(RSA.generate(prime1, prime2));
    sw.stop();
    writeln(prime1, " является простым: ", prime1.isPrime);
    writeln(prime2, " является простым: ", prime2.isPrime);
    writeln(prime3, " является простым: ", prime3.isPrime);
    writeln(prime4, " является простым: ", prime4.isPrime);
    writeln("time = ", sw.peek.total!"nsecs", " ", sw.peek.total!"usecs" / 1_000_000f);
    sw.reset();
}
