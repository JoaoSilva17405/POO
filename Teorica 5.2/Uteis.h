#ifndef UTEIS_H_INCLUDED
#define UTEIS_H_INCLUDED

#include <iostream>
#include <time.h>
using namespace std;

class Uteis
{
public:
    static int Aleatorio(int min, int max)
    {
        return min + rand() % (max - min + 1);
    }
    static void wait ( int mlseconds )
    {
        clock_t endwait;
        endwait = clock () + mlseconds;
        while (clock() < endwait);
    }
    static void wait_segundos ( int seconds )
    {
        wait(seconds * CLOCKS_PER_SEC);
    }
};



#endif // UTEIS_H_INCLUDED
