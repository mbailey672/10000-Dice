#include "roll.h"
#include "dice.h"
#include <iostream>
#include <sstream>
#include <windows.h>

using namespace std;

roll::roll() {
    dCollection = vector<dice>(6); //Nice constructor

    //I personally think this is lovely and should automatically be included in the C++ language
    alphaNums[1] = "One";
    alphaNums[2] = "Two";
    alphaNums[3] = "Three";
    alphaNums[4] = "Four";
    alphaNums[5] = "Five";
    alphaNums[6] = "Six";

    pointValue  = 0;     //Point value should be zero for new roll
    numDice     = 6;     //New rolls have six dice that need to be rolled
    keptDiceYet = false; //New rolls mean that a dice hasn't been kept yet
}

roll::~roll() {

}

void roll::newRoll() {
    //numDice is the number of dice rolled. It's subtracted from the total amount of dice being used, meaning if 4 dice are
    //rolled, dice 3-6 are rolled, while 1-2 remain the same
    for(int i = (dCollection.size() - numDice); i < dCollection.size(); i++) {
        dCollection[i].roll();
    }
}

void roll::sortRoll() {
    //Only sorts the dice that have been rolled and not the chosen ones.
    //I realize I've butchered the bubble sort algorithm here but sorting the latter half of vectors of objects
    //was causing me misery.
    if(numDice != 0) {
        for(int i = (dCollection.size() - numDice); i < dCollection.size()-1; i++) {
            //Last i elements are already in place
            for(int j = (dCollection.size() - numDice); j<dCollection.size()-1; j++) {
                if(dCollection[j].dValue > dCollection[j+1].dValue) {
                    swap(dCollection[j], dCollection[j+1]);
                }
            }
        }
    }

}

bool roll::findCombos() {

    possibleMoves.clear(); //Makes sure there's no sneaky old combos lying around

    {   //Hopefully these are self explanatory
        int numOnes = 0;
        int numFives = 0;

        //New library for me. Allows for Python style string concatenation without casing or goofiness.
        stringstream sentenceStream;

        //Most for loop iteration amounts are determined by the amount of dice. Theoretically I don't see why you couldn't play this game with 4 or 6 or 100 dice.
        for(int i = (dCollection.size() - numDice); i < dCollection.size(); i++) {
            //Remember that dCollection is a vector of type "dice", so I've overloaded the operator here.
            if(dCollection[i] == 1) {
                numOnes++;
            } else if (dCollection[i] == 5) {
                numFives++;
            }
        }

        //Number of ones/fives needs to be less than 3, since if it's 3 or greater, then they have a different point value
        if(numOnes < 3) {
            for(; numOnes > 0; numOnes--) {
                combination temp;
                temp.pointValue = numOnes * 100;
                //I added a nice/formatted sentence for every combination so they're easy to print out.
                //I think this is a little nicer then making the user do some math after spitting out random dice
                sentenceStream << alphaNums[numOnes] << " 1's equaling " << temp.pointValue << " points."; //I think these stringstreams are lovely
                temp.sentence = sentenceStream.str(); //Makes the sentence equal the contents of the stringstream

                //Adds the values of every chosen dice to a vector in the combination. Storing the dice amount
                for(int i = 0; i < numOnes; i++) {
                    temp.chosenDice.push_back(1);
                }
                possibleMoves.push_back(temp);
                sentenceStream = stringstream();
            }
        }
        if(numFives < 3) {
            //Essentially an identical loop to the previous one, except the ones are replaced with 5's
            //I could probably make one function to do both, but I could see that being a little illegible.
            for(; numFives > 0; numFives--) {
                combination temp;
                temp.pointValue = numFives * 50;
                sentenceStream << alphaNums[numFives] << " 5's equaling " << temp.pointValue << " points." ;
                temp.sentence = sentenceStream.str();
                //Adds the numbers to the combination structure. Useful when combining combinations.
                for(int i = 0; i < numFives; i++) {
                    temp.chosenDice.push_back(5);
                }
                possibleMoves.push_back(temp);
                sentenceStream = stringstream();
            }
        }
    }

    //This for loop finds all the combo's with multiple combinations of the same dice value
    {
        //Simple double nested for loop here. Iterates through each die, then iterates through every die after it, seeing if they're the same value
        int sameDice = 1; //For the purpose of this loop, there's always at least one die that's the same. Even if that doesn't make any sense.
        for(int i = (dCollection.size() - numDice); i < dCollection.size(); i++, sameDice = 1) {
            combination temp;
            temp.chosenDice.push_back(dCollection[i]);
            for(int j = i+1; j < dCollection.size(); j++) {
                if(dCollection[i] == dCollection[j]) {
                    sameDice++;
                    temp.chosenDice.push_back(dCollection[j]);
                }
            }
            stringstream sentenceStream;

            //If there are 3 or more dice that are the same, then a combination has been found
            if(sameDice >= 3) {
                //Basically just programming the if-statement tree equivalent of the rules
                if(sameDice == 3) {
                    if(dCollection[i] == 1) {
                        temp.pointValue = 1000;
                    } else {
                        temp.pointValue = dCollection[i] * 100;
                    }
                } else if (sameDice == 4) {
                    if(dCollection[i] == 1) {
                        temp.pointValue = 1100;
                    } else {
                        temp.pointValue = dCollection[i] * 100 + 100;
                    }
                } else if (sameDice == 5) {
                    temp.pointValue = 1500;
                } else if (sameDice == 6) {
                    temp.pointValue = 2500;
                }
                //You've seen this code before.
                sentenceStream << alphaNums[sameDice] << " " << dCollection[i].dValue << "'s, equaling " << temp.pointValue << " points.";
                temp.sentence = sentenceStream.str();
                possibleMoves.push_back(temp);
            }
        }
    }


    //These are only possible if 6 dice were rolled
    if(numDice == 6) {
        combination temp;
        stringstream sentenceStream;
        bool comboFound = false; //Stores whether a combo has been found or not
        //Don't see any better way of programming these then long if statements, which is always a shame.
        //Three pairs
        if(dCollection[0] == dCollection[1] && dCollection[2] == dCollection[3] && dCollection[4] == dCollection[5]) {
            temp.pointValue = 1200;
            sentenceStream << "Three pairs of " << dCollection[1].dValue << ", " << dCollection[3].dValue << ", and " << dCollection[5].dValue << " equaling " << temp.pointValue << " points.";
            comboFound = true;
        }
        //Two triplets
        else if((dCollection[0] == dCollection[1] && dCollection[1] == dCollection[2]) && (dCollection[3] == dCollection[4] && dCollection[4] == dCollection[5])) {
            temp.pointValue = 1200;
            sentenceStream << "Two triplets of " << dCollection[1].dValue << " and " << dCollection[5].dValue << " equaling " << temp.pointValue << " points.";
            comboFound = true;
        } else { //Straight

            int digitsChecked = 0;
            for(int i = 0; i+1 == dCollection[i] && i < 6; i++, digitsChecked++);

            if(digitsChecked == 6) {
                temp.pointValue = 2000;
                sentenceStream << "Straight equaling " << temp.pointValue << " points.";
                comboFound = true;
            }
        }

        //Could have copy and pasted the code in this if statement below into every if statement above it, but that would be more lines.
        if(comboFound) {
            temp.sentence = sentenceStream.str();
            for(int i = 0; i < numDice; i++) {
                temp.chosenDice.push_back(dCollection[i]);
            }
            possibleMoves.push_back(temp);
        }
    }

    //Only sorts the combos if there's more then 1. Not sure if that's necessary or not but I don't want to delete the code and figure out.
    //Returns false if no combos are found, letting the driver code know that there's no moves available.
    if(possibleMoves.size() > 1) {
        sortCombos();
        return true;
    } else if (possibleMoves.size() == 1) {
        return true;
    } else {
        return false;
    }
}

void roll::sortCombos() {
    //Three cheers for unbutchered bubble sort code
    for(int i=0; i<possibleMoves.size()-1; i++) {
        //Last i elements are already in place
        for(int j=0; j<possibleMoves.size()-i-1; j++) {
            if(possibleMoves[j].pointValue > possibleMoves[j+1].pointValue) {
                swap(possibleMoves[j], possibleMoves[j+1]);
            }
        }
    }
}

//Overloaded for the combination object. Simple for loop otherwise.
void roll::printMoves() {
    for(int i = 0; i < possibleMoves.size(); i++) {
        cout << i+1 << ". " << possibleMoves[i];
    }
}

//Function for ending the turn
void roll::pickMove(int playerChoice) {
    vector<dice> newDiceSet; //Stores the updated dice set

    //Adds the chosen dice to the new set
    for(int i = 0; i < possibleMoves[playerChoice].chosenDice.size(); i++) {
        newDiceSet.push_back(possibleMoves[playerChoice].chosenDice[i]);
    }

    //i is the variable that iterates through every dice in the old dice set. j is the variable that counts the stored dice encountered
    //Basically, if there's a 1 chosen, and dCollection[i] is equal to newDiceSet[j], then instead of adding dCollection[i] to the newDiceSet,
    //since it's already stored, it adds 1 to j. This continues, adding dice that aren't already accounted for while skipping dice that are
    for(int i = 0, j = 0; i < 6; i++) {
        if(dCollection[i] == newDiceSet[j] && j < possibleMoves[playerChoice].chosenDice.size()) {
            j++;
        } else {
            newDiceSet.push_back(dCollection[i]);
        }
    }


    dCollection = newDiceSet; //Overwrites the old dice set with the new one. Note that the dice values haven't changed, just the order has
    pointValue  += possibleMoves[playerChoice].pointValue; //Increases the point value of the roll
    keptDiceYet = true; //Marks that there have been dice saved

    //Decreases the number of dice that can be rolled by the dice that have been chosen. If you pick two 1's, then consider those dice "locked",
    //as only the remaining 4 dice can be rolled.
    numDice -= possibleMoves[playerChoice].chosenDice.size();

    //If there's no dice left to roll, then you can roll all 6.
    if(numDice == 0) {
        cout << "You've picked all 6 dice, so you get to roll them all again!" << endl;
        numDice = 6;
    }

    possibleMoves.clear(); //Clears any of the previous possible moves as they likely are invalid after locking some of the dice
}

ostream& operator<< (ostream &output, roll &playerRoll) {
    //Cute little color code here. Red for locked dice, green for those that can be picked
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for(int i = 0; i < playerRoll.dCollection.size(); i++) {
        if(i < (6-playerRoll.numDice)) {
            SetConsoleTextAttribute(hConsole, 12);
        } else {
            SetConsoleTextAttribute(hConsole, 10);
        }
        output << playerRoll.dCollection[i].dValue << endl;
    }
    output << endl;
    SetConsoleTextAttribute(hConsole, 15);
}
