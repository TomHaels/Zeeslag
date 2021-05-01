#include "ships.h"
#include <iostream>
#include <strings.h>

using namespace std;

ships::ships(int size)
{
    setlength(size);
};

int ships::getlenght()
{
    return length;
}

void ships::setlength(int l)
{
    length = l;
}


