#ifndef ROLL_H
#define ROLL_H
#include "dice.h"
#include "combination.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

class roll
{
    public:
        roll();
        virtual ~roll();

        vector<dice> dCollection; //Stores 6 dice. For rolls
        vector<combination> possibleMoves; //Stores all the possible combinations

        int pointValue; //Stores how many points have been accumulated
        int numDice;    //Stores how many dice should be rolled.
        bool keptDiceYet; //Stores whether a dice has been kept or not, which is important to know whether someone can end there turn or not.
        map<int, string> alphaNums; //My favorite library makes a return. Essentially useless object expect for pretty sentences

        void newRoll(); //Rolls dice
        void sortRoll(); //Sorts dice values
        bool findCombos(); //Finds dice combinations
        void sortCombos(); //Sorts dice combinations by point value
        void  pickMove(int); //Picks a combination, adds points, reduces dice number, etc.

        void printMoves();   //Prints moves
        //For printing dice values because lazy
        friend ostream& operator<< (ostream &output, roll &playerRoll);


};

#endif // ROLL_H
