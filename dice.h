#ifndef DICE_H
#define DICE_H

using namespace std;



class dice
{
    //Honestly, this doesn't need to be a class. I made it a class before most of the other methods were started, and now I'm too lazy just to include this code in the "roll"  object
    public:
        dice();
        dice(int);
        virtual ~dice();

        int dValue;  //stores the dice value
        void roll(); //rolls the dice value. Random number generator

        //Can't believe you have to do this. Ridiculous
        friend bool operator== (int, dice &one);
        friend bool operator== (dice &one, int);

        friend bool operator== (dice &one, dice &two);
        friend int operator* (dice &one, int num);
};

#endif // DICE_H
