#include "dice.h"
#include <time.h>
#include <iostream>
#include <cstdlib>

using namespace std;

dice::dice()
{
    dValue = 0;
}

//Dunno if I ever use this constructor or not
dice::dice(int diceValue) {
    dValue = diceValue;
}

dice::~dice()
{
    //dtor
}

void dice::roll() {
    dValue = (rand()%6)+1;
}

bool operator== (dice &one, int diceValue) {
    if(diceValue == one.dValue) {
        return true;
    } else {
        return false;
    }
}

bool operator== (int diceValue, dice &one) {
    if(diceValue == one.dValue) {
        return true;
    } else {
        return false;
    }
}

bool operator== (dice &one, dice &two) {
    if(one.dValue == two.dValue) {
        return true;
    } else {
        return false;
    }
}

int operator* (dice &one, int num) {
    return one.dValue * num;
}
