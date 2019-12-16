#include <iostream>
#include <thread>
#include <clocale>

using std::cout;
using std::endl;
using std::thread;

void foo()
{
    cout << "Do some foo stuff." << endl;
}

void bar(int x)
{
    cout << "Do some bar: " << x << " stuff." << endl;
}

int main()
{
    //setlocale("")
    std::setlocale(LC_ALL, "es_ES.UTF-8");
//    std::locale::global(std::locale("ES"));
//    cout << "Locale: " << std::locale("ES").name().c_str()
//         << endl;
    thread first(foo);//inicia hilo que llama a foo()
    thread second(bar, 0); //inicia hilo que llama a bar(0)
    cout << "La ejecución concurrente de las funciones foo y bar va a comenzar " << endl;
    //sincronizar los hilos
    first.join();
    second.join();
    cout << "Ejecución finalizada" << endl;
    return 0;
}

/*
// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread

void foo()
{
    // do stuff...
    std::cout << "foo stuff" << std::endl;
}

void bar(int x)
{
    // do stuff...
    std::cout << "bar stuff" << std::endl;
}

int main()
{
    std::thread first (foo);     // spawn new thread that calls foo()
    std::thread second (bar, 0); // spawn new thread that calls bar(0)
    std::cout << "main, foo and bar now execute concurrently...\n";
    // synchronize threads:
    first.join();                // pauses until first finishes
    second.join();               // pauses until second finishes
    std::cout << "foo and bar completed.\n";
    return 0;
}
*/
