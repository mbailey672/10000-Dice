#ifndef COMBINATION_H
#define COMBINATION_H

#include <vector>
#include <string>
#include <map>
#include <dice.h>

using namespace std;

class combination
{
    public:
        combination();
        virtual ~combination();

        int pointValue;          //Stores the amount of points for a given combination
        string sentence;         //Stores the sentence that should be outputted for a combination
        vector<dice> chosenDice; //Stores the values of the dice being chosen


        friend ostream& operator<< (ostream &output, combination &possibleMove);


    private:
};

#endif // COMBINATION_H
