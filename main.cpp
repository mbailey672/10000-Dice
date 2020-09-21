#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include "dice.h"
#include "roll.h"
#include "combination.h"

using namespace std;


void runTurn(roll&, vector<int>&, int);
bool canPickCombo(int, int);
int  getPlayersMove(roll&, int);
bool isValidMove(roll&, int);

int main() {

    srand (time(NULL)); //Seeds random number generator. Somehow works for every file in this project which is interesting.
    int playerCount = 0; //Stores number of players

    roll playerRoll;

    bool hasWon = false;

    cout << "Welcome to Dice 10000!" << endl;

    cout << "Please enter how many people will be playing the game (1-10): ";
    cin  >> playerCount;

    //Holds the points for all the players. I learned that vectors have a constructor, which makes initialization really easy.
    vector<int> points(playerCount, 0);

    //This for loop runs the game by iterating through the players
    for(int currentPlayer = 0; !hasWon; currentPlayer++) {
        //if the current player is the last player, then this resets the current player back to the first one
        if(currentPlayer == playerCount) {
            currentPlayer = 0;
        }
        //this runs the turn
        runTurn(playerRoll, points, currentPlayer);

        //Checks to see if someone has won
        if(points[currentPlayer] == 10000) {
            //advances the player once
            currentPlayer++;

            //lets everyone get their last turn
            for(int j = 0; j < playerCount-1; j++, currentPlayer++) {
                //if the current player is the last player, then this resets the current player back to the first one
                if(currentPlayer == playerCount) {
                    currentPlayer = 0;
                }
                runTurn(playerRoll, points, currentPlayer);
            }
            //marks that the game has won
            hasWon = true;
        }
    }

    //Essentially me fooling around to have a nice winning sentence at the end.
    stringstream sentenceStream;
    string winningSentence = "";
    sentenceStream << "Player ";

    //This adds all the players that reached a score of 10000
    for(int i = 0; i < playerCount; i++) {
        if(points[i] == 10000) {
            sentenceStream << i << ", ";
        }
    }
    winningSentence = sentenceStream.str();
    //Gets rid of the ", " at the end of the string
    winningSentence = winningSentence.substr(0, (winningSentence.length()-2));
    sentenceStream = stringstream();
    sentenceStream << winningSentence << " has won the game!";
    cout << sentenceStream.str() << endl;

    return 0;
}
/*
    This function is called once every time the for loop iterates. It runs the turn of one player.
    The roll object is passed by reference as parts of the previous players roll could be passed on.
    The points vector is also passed by reference so the changes remain.
    playerNum indicates the player that is having their turn.
*/
void runTurn(roll& playerRoll, vector<int>& points, int playerNum) {
    int playerChoice = 0;  //Used in various spots around the function for menu selection
    bool doneTurn = false; //Determines when the game is done

    cout << "It is Player " << playerNum+1 << "'s turn. You have " << points[playerNum] << " points." << endl;
    //If the current roll object has had dice selected/chosen by any player, then the current player needs to decide
    //whether they want to reroll all 6 dice or roll on the existing dice. If there isn't any kept dice then all 6 are rolled automatically
    if(playerRoll.keptDiceYet == false) {
        playerRoll.newRoll();
        playerRoll.sortRoll();
    } else {
        //This prints out the 6 dice numbers
        cout << playerRoll;

        cout << "Currently, your roll is worth " << playerRoll.pointValue << " points." << endl;
        cout << "You can either reroll all 6 dice, or roll the remaining " << playerRoll.numDice << " dice." << endl;
        cout << "1. Reroll dice" << endl;
        cout << "2. Roll remaining " << playerRoll.numDice << " dice" << endl;
        cout << "What do you want to do: ";
        cin  >> playerChoice;

        while(playerChoice != 1 && playerChoice != 2) {
            cout << "P(i.e. it's already stored)lease enter a valid choice: ";
            cin  >> playerChoice;
        }

        //This resets the whole object by calling the constructor. Might not be as efficient as reseting the important parts, but it's less lines of code.
        if(playerChoice == 1) {
            playerRoll = roll();
        }
        playerRoll.newRoll();
        playerRoll.sortRoll();
    }


    cout << playerRoll;

    //findCombos acts as a boolean function that determines whether there are any valid dice combinations or not
    //It also populates the vector in the roll object that stores all the combinations
    //isValidMove determines whether there's any moves that wouldn't make a player go over 10k points.
    if(playerRoll.findCombos() && isValidMove(playerRoll, points[playerNum] + playerRoll.pointValue)) {
        playerRoll.printMoves(); //Prints all the combinations

        //getPlayerMoves returns a selection from a menu
        playerChoice = getPlayersMove(playerRoll, points[playerNum]);

        //This function does various things including indicating that dice have been chosen, along with locking
        //how many dice have been rolled
        playerRoll.pickMove(playerChoice);

        cout << endl;

        while(!doneTurn) {
            cout << "Currently, your roll is worth " << playerRoll.pointValue << " points." << endl;
            cout << "You've now rolled at least once, so you can either: " << endl;
            cout << "1. Continue picking dice" << endl;
            cout << "2. Roll the remaining " << playerRoll.numDice << " dice" << endl;
            cout << "3. Stop your turn" << endl;
            cout << "What would you like to do: ";
            cin  >> playerChoice;

            //Needed a few different rule-specific error checking loops for this game. Added individual while
            //loops inside the loop to give more specific error messages.
            while((playerChoice != 1 && playerChoice != 2 && playerChoice != 3) || (playerChoice == 1 && playerRoll.numDice == 6) || (playerChoice == 3 && playerRoll.findCombos() && isValidMove(playerRoll, points[playerNum] + playerRoll.pointValue))) {
                while(playerChoice != 1 && playerChoice != 2 && playerChoice != 3) {
                    cout << "Please enter a valid choice: ";
                    cin  >> playerChoice;
                }
                while(playerChoice == 1 && playerRoll.numDice == 6) {
                    cout << "You have to roll the dice now or end your turn." << endl;
                    cout << "What would you like to do: ";
                    cin  >> playerChoice;
                }
                //Makes sure that the player doesn't end their turn while they can still pick dice.
                //Not sure why anyone would want to do that or why it's a problem, but I wrote it for some reason.
                while(playerChoice == 3 && playerRoll.findCombos() && isValidMove(playerRoll, points[playerNum] + playerRoll.pointValue)) {
                    cout << "Why would you end your turn? You can still pick more dice" << endl;
                    playerChoice = 1;
                }
            }

            cout << endl;

            switch(playerChoice) {
            case 1:
                playerRoll.sortRoll();
                //This finds the remaining combinations
                if(playerRoll.findCombos()) {
                    cout << playerRoll;
                    playerRoll.printMoves();
                    //Makes sure theres a valid move before asking what one they want to pick
                    if(isValidMove(playerRoll, points[playerNum] + playerRoll.pointValue)) {
                        playerChoice = getPlayersMove(playerRoll, points[playerNum]);
                        playerRoll.pickMove(playerChoice);
                    }
                } else {
                    cout << "There are no moves for you to pick." << endl;
                }
                break;
            case 2:
                playerRoll.newRoll();
                playerRoll.sortRoll();
                cout << playerRoll;
                //Same basic error checking functions
                if(playerRoll.findCombos() && isValidMove(playerRoll, points[playerNum] + playerRoll.pointValue)) {
                    playerRoll.printMoves();
                    playerChoice = getPlayersMove(playerRoll, points[playerNum]);
                    playerRoll.pickMove(playerChoice);
                } else {
                    //We support constructive criticism
                    cout << "HAHAHAHA you rolled too much and lost everything." << endl;
                    //Clears roll for next player
                    playerRoll = roll();
                    doneTurn = true;
                }
                break;
            case 3:
                doneTurn = true;
                break;
            }
            cout << endl;
        }

    } else {
        cout << "LOL you have no moves feelsbadman." << endl;
        playerRoll = roll();
    }

    //Adds however many points the roll is worth to the players score
    points[playerNum] += playerRoll.pointValue;
}

//Kind of have another function that already does this same thing, but this is for one combination.
//Can't be bothered to figure out how to concatenate them.
bool canPickCombo(int playerPoints, int rollPoints) {
    if(rollPoints + playerPoints > 10000) {
        cout << "You can't pick this move, since your score would exceed 10000 points." << endl;
        return false;
    } else {
        return true;
    }
}

//Function that gets the players move and returns the index of the combination.
//Figured I did this enough that it warranted its own code.
int  getPlayersMove(roll& playerRoll, int playerPoints) {
    int playerChoice = 0;
    cout << "Which move do you want to pick: ";
    cin  >> playerChoice;

    //Makes sure that the players number is one of the possible choices and that it wouldn't put their amount of points over 10k
    while((playerChoice < 0 || playerChoice > playerRoll.possibleMoves.size()) || !canPickCombo(playerPoints, playerRoll.possibleMoves[playerChoice-1].pointValue + playerRoll.pointValue)) {
        cout << "Please enter a valid choice: ";
        cin  >> playerChoice;
    }

    //For array indexing
    playerChoice--;
    return playerChoice;
}

//Makes sure that there is at least one move that wouldn't put the player over 10k.
//Doesn't guarantee that any one combination will do that, which is why there's another function that is mostly identical
bool isValidMove(roll& playerRoll, int playerPoints) {
    int numNotPossible = 0;
    for(int i = 0; i < playerRoll.possibleMoves.size(); i++) {
        if(playerRoll.possibleMoves[i].pointValue + playerPoints > 10000) {
            numNotPossible++;
        }
    }
    //Prints out a lovely error message if the player tries to do something impossible
    if(numNotPossible == playerRoll.possibleMoves.size()) {
        cout << "You can't pick any moves, since all of them would put you over 10000 points." << endl;
        return false;
    } else {
        return true;
    }


