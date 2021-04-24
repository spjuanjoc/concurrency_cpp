//
// Created by juan.castellanos on 24/01/20.
//

#ifndef MUTEXEXAMPLE_UNPROTECTEDMUTEX_H
#define MUTEXEXAMPLE_UNPROTECTEDMUTEX_H

#include <mutex>
#include <string>

class some_data
{
    int         a;
    std::string b;

public:
    void do_something(){};
};
class data_wrapper
{
private:
    some_data  data;
    std::mutex m;

public:
    template<typename Function>
    void process_data( Function func )
    {
        std::lock_guard<std::mutex> l( m );
        func( data );  // Pass “protected” data to user-supplied function
    }
};

some_data* unprotected;

void malicious_function( some_data& protected_data )
{
    unprotected = &protected_data;
}

data_wrapper x;

void foo()
{
    x.process_data( malicious_function );  //Pass in a malicious function
    unprotected->do_something();           //Unprotected access to protected data
}
#endif  //MUTEXEXAMPLE_UNPROTECTEDMUTEX_H
