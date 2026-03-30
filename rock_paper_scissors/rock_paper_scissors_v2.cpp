#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;

// helpler
enum enPlayerChoice
{
    Rock = 1,
    Paper = 2,
    Scissors = 3
};

enum enWinner
{
    Computer = 1,
    Player,
    Draw
};

struct stGamePoints
{
    int player = 0;
    int computer = 0;
    int draw = 0;
};

struct stGameResult
{
    stGamePoints points;
    enWinner winner;
    int roundsNumber = 0;
};

struct stRound
{
    enPlayerChoice computerChoice;
    enPlayerChoice playerChoice;
    enWinner winner;
    int roundNumber = 0;
};

int randomNumber(int from, int to)
{
    return rand() % (to - from + 1) + from;
}

int readRoundsNumber(const string &message)
{
    int number = 0;
    cout << message;
    cin >> number;

    while (cin.fail() || number < 1)
    {
        if (cin.fail())
            cout << "Invalid input, try again: ";
        else
            cout << "number must be greater than 0, try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> number;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return number;
}

bool confirm(const string &message)
{
    char answer = 0;

    do
    {
        cout << message;
        answer = cin.get();
        answer = tolower(answer);

        if (answer != '\n')
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (answer != 'y' && answer != 'n' && answer != '\n');

    return answer == 'y' || answer == '\n';
}


// read players choice
enPlayerChoice readPlayerChoice()
{
    int choice = 0;
    cout << "Enter your choice [1] stone, [2] paper, [3] scissors: ";
    cin >> choice;

    while (cin.fail() || choice < 1 || choice > 3)
    {
        cout << "choice must be between (1-3): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> choice;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return static_cast<enPlayerChoice>(choice);
}

enPlayerChoice generateComputerChoice()
{
    return static_cast<enPlayerChoice>(randomNumber(1, 3));
}


// functions for both round and ends
string getWinnerName(enWinner winner)
{
    const string winnerNames[] = {"Computer", "Player", "Draw"};
    return winnerNames[winner - 1];
}

void setWinnerColor(enWinner winner)
{
    switch (winner)
    {
    case Player:
        cout << "\033[32m"; // Green
        break;

    case Computer:
        cout << "\033[31m"; // Red
        break;

    case Draw:
        cout << "\033[33m"; // Yellow
        break;
    }
}

void resetColor()
{
    cout << "\033[0m";
}


// display game ends
enWinner finalWinner(const stGamePoints &points)
{
    if (points.computer == points.player)
        return enWinner::Draw;

    if (points.player > points.computer)
        return enWinner::Player;
    else
        return enWinner::Computer;
}

void displayGameOverScreen()
{
    cout << "\n\n\t\t";
    cout << string(45, '-') << endl;
    cout << "\t\t\t\tG A M E  O V E R\n";
    cout << "\t\t";
    cout << string(45, '-') << endl;
}

void displayGameResult(const stGameResult &gameResult)
{
    cout << "\n\t\t";
    cout << string(8, '_');
    cout << " [ Game Result Of " << gameResult.roundsNumber << " Rounds ] ";
    cout << string(8, '_') << endl;

    cout << "\t\tPlayer won times   : " << gameResult.points.player << endl;
    cout << "\t\tComputer won times : " << gameResult.points.computer << endl;
    cout << "\t\tDraw times         : " << gameResult.points.draw << endl;
    cout << "\t\tFinal winner       : ";
    setWinnerColor(gameResult.winner);
    cout << getWinnerName(gameResult.winner) << endl;
    resetColor();
    cout << "\t\t";
    cout << string(45, '_') << endl;
}



// rounds
string getPlayerChoiceName(enPlayerChoice choice)
{
    const string choiceNames[] = {"Stone", "Paper", "Scissors"};
    return choiceNames[choice - 1];
}

void displayRoundDetails(const stRound &roundInfo)
{
    cout << '\n';
    cout << string(10, '_');
    cout << " Round [" << roundInfo.roundNumber << "] ";
    cout << string(10, '_') << endl;

    cout << "Player choice     : " << getPlayerChoiceName(roundInfo.playerChoice) << endl;
    cout << "Computer choice   : " << getPlayerChoiceName(roundInfo.computerChoice) << endl;
    cout << "Round winner      : ";
    setWinnerColor(roundInfo.winner); // winner color
    cout << getWinnerName(roundInfo.winner) << endl;
    resetColor();
    cout << string(31, '_') << endl;
}

enWinner roundWinner(enPlayerChoice playerChoice, enPlayerChoice computerChoice)
{
    if (playerChoice == computerChoice)
        return enWinner::Draw;

    switch (playerChoice)
    {
    case Rock:
        return (computerChoice == Scissors) ? enWinner::Player : enWinner::Computer;
    case Scissors:
        return (computerChoice == Paper) ? enWinner::Player : enWinner::Computer;
    case Paper:
        return (computerChoice == Rock) ? enWinner::Player : enWinner::Computer;
    }
    return enWinner::Draw;
}

void pointsOfPlayers(enWinner winner, stGamePoints &points)
{
    switch (winner)
    {
    case enWinner::Player:
        points.player++;
        break;
    case enWinner::Computer:
        points.computer++;
        break;
    case enWinner::Draw:
        points.draw++;
        break;
    }
}

void startRounds(stGameResult &gameResult)
{
    stRound roundInfo;
    
    for (int round = 1; round <= gameResult.roundsNumber; round++)
    {
        cout << "\nRound [" << round << "] begins: \n";

        roundInfo.roundNumber = round;
        roundInfo.playerChoice = readPlayerChoice();
        roundInfo.computerChoice = generateComputerChoice();
        roundInfo.winner = roundWinner(roundInfo.playerChoice, roundInfo.computerChoice);

        pointsOfPlayers(roundInfo.winner, gameResult.points);
        displayRoundDetails(roundInfo);
    }
}


// start
void startGame()
{
    stGameResult gameResult;
    gameResult.roundsNumber = readRoundsNumber("how many rounds do you want to play? ");

    startRounds(gameResult);

    gameResult.winner = finalWinner(gameResult.points);

    displayGameOverScreen();
    displayGameResult(gameResult);
}

int main()
{
    srand((unsigned)time(NULL));

    do
    {
        system("cls");
        startGame();
    } while (confirm("\n\t\tdo you want to play again? (y/n):  "));

    return 0;
}
