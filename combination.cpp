#include "combination.h"
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

combination::combination() {
    pointValue = 0;
    sentence   = "";
}

combination::~combination() {
    //dtor
}

ostream& operator<< (ostream &output, combination &possibleMove) {
    //Outputs the point value before the sentence because it looked nice to me
    output << possibleMove.pointValue << " - " <<possibleMove.sentence << endl;
}
