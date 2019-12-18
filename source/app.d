module main;

import std.stdio;
import std.datetime.stopwatch;
import std.file;

import bigint;
import rsa;


/// Вывести пользователю подсказку
void unknown() {
    writeln("Неизвестная команда. Используйте:\n\tgenerate num1 num2 - Создание новых ключей, где num1 и num2 - простые числа.\n\tencrypt pk text - Зашифровать текст text по открытому ключу pk.\n\tdecrypt pk sk ciphertext - Расшифровать данные ciphertext, pk - открытый ключ, sk - закрытый ключ.\nДобавьте ключ -f, если указаны имена файлов.");
}

void main(string[] argv)
{
    auto sw = StopWatch(AutoStart.no);

    // Проверка команды: создать ключи, зашифровать, расшифровать
    switch(argv.length > 1 ? argv[1] : "") {
        case "generate":
        if(argv.length < 4) {
            unknown();
            return;
        }
        writeln("Создание новых ключей");
        sw.start();
        BigInt p;
        BigInt q;
        RSA rsa;
        if(argv.length == 5 && argv[4] == "-f") {
            p = BigInt.fromFile(argv[2]);
            q = BigInt.fromFile(argv[3]);
        } else {
            p = new BigInt(argv[2]);
            q = new BigInt(argv[3]);
        }

        rsa = RSA.generate(p, q);
        rsa.sk.toFile("sk.key");
        rsa.pk.toFile("pk.key");
        sw.stop();
        writeln("Ключи созданы. Закрытый ключ сохранен в sk.key, открытый - в pk.key");
        writeln("Закрытый ключ: ", rsa.sk);
        writeln("Открытый ключ: ", rsa.pk);
        break;


        case "encrypt":
        if(argv.length < 4) {
            unknown();
            break;
        }
        writeln("Зашифровка текста");
        sw.start();

        RSA rsa;
        BigText text;

        if(argv.length == 5 && argv[4] == "-f") {
            rsa = new RSA(BigInt.fromFile(argv[2]));
            text = new BigText(readText(argv[3]));
        } else {
            rsa = new RSA(new BigInt(argv[2]));
            text = new BigText(argv[3]);
        }

        BigInt ciphertext = rsa.encrypt(text);
        ciphertext.toFile("ciphered.txt");
        sw.stop();
        writeln("Текст был зашифрован. Шифротекст сохранен в ciphered.txt");
        writeln("Результат: ", ciphertext);
        break;


        case "decrypt":
        if(argv.length < 5) {
            unknown();
            break;
        }
        writeln("Расшифровка текста");
        sw.start();
        RSA rsa;
        BigInt ciphertext;
        if(argv.length == 6 && argv[5] == "-f") {
            rsa = new RSA(BigInt.fromFile(argv[2]), BigInt.fromFile(argv[3]));
            ciphertext = BigInt.fromFile(argv[4]);
        } else {
            rsa = new RSA(new BigInt(argv[2]), new BigInt(argv[3]));
            ciphertext = new BigInt(argv[4]);
        }

        BigText text = rsa.decryptText(ciphertext);
        text.calcLength();
        std.file.write("deciphered.txt", text.toString);
        sw.stop();
        writeln("Шифротекст расшифрован. Текст сохранен в deciphered.txt");
        writeln("Текст: ", text);
        break;


        default:
        unknown();
        return;
    }
    writeln("Время выполнения: ", sw.peek.total!"nsecs", " наносекунд; ", sw.peek.total!"usecs" / 1_000_000f, " секунд");
}
