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


void main()
{
    auto sw = StopWatch(AutoStart.no);
    BigInt prime1 = new BigInt("0x4977c970792f309cdafac771");
    BigInt prime2 = new BigInt("0x12fe5633ca465bac1527ab47");
    BigInt prime3 = new BigInt("0x90a7c97a79253f9cdbfacb9b");
    BigInt prime4 = new BigInt("0xb0125073b1283fabdbf96b47");


    sw.start();
    //auto number3 = number2 % 5;
    sw.stop();
    writeln(prime1.isPrime);
    writeln(prime2.isPrime);
    writeln(prime3.isPrime);
    writeln(prime4.isPrime);
    writeln("time = ", sw.peek.total!"nsecs", " ", sw.peek.total!"usecs" / 1_000_000f);
    sw.reset();
}
