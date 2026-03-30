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
    Computer,
    Player,
    Draw
};

struct stGamePoints
{
    int playerPoint = 0;
    int computerPoint = 0;
    int drawPoint = 0;
};

struct stPlayers
{
    enPlayerChoice computerChoice;
    enPlayerChoice playerChoice;
};

struct stRound
{
    stPlayers players;
    enWinner winner;
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

// winner name for both round and ends
string getWinnerName(enWinner winner)
{
    switch (winner)
    {
    case enWinner::Computer:
        return "Computer";
    case enWinner::Player:
        return "Player";
    case enWinner::Draw:
        return "Draw";
    }
    return "ERROR";
}

// display game ends
enWinner finalWinner(stGamePoints points)
{
    if (points.computerPoint == points.playerPoint)
        return enWinner::Draw;

    if (points.playerPoint > points.computerPoint)
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

void displayGameResult(int roundsNumber, stGamePoints &points)
{
    cout << "\t\t";
    cout << string(14, '_');
    cout << " [ Game Result ] ";
    cout << string(14, '_') << endl;

    cout << "\t\tGame rounds        : " << roundsNumber << endl;
    cout << "\t\tPlayer won times   : " << points.playerPoint << endl;
    cout << "\t\tComputer won times : " << points.computerPoint << endl;
    cout << "\t\tDraw times         : " << points.drawPoint << endl;
    cout << "\t\tFinal winner       : " << getWinnerName(finalWinner(points)) << endl;
    cout << "\t\t";
    cout << string(45, '_') << endl;
}

// rounds
string getPlayerChoiceName(enPlayerChoice choice)
{
    switch (choice)
    {
    case Rock:
        return "Stone";
    case Paper:
        return "Paper";
    case Scissors:
        return "Scissors";
    }
    return "ERROR";
}

void displayRoundDetails(stPlayers players, enWinner winner, int round)
{
    cout << '\n';
    cout << string(10, '_');
    cout << " Round [" << round + 1 << "] ";
    cout << string(10, '_') << endl;

    cout << "Player choice     :" << getPlayerChoiceName(players.playerChoice) << endl;
    cout << "Computer choice   :" << getPlayerChoiceName(players.computerChoice) << endl;
    cout << "Round winner      :" << getWinnerName(winner) << endl;
    cout << string(31, '_') << endl;
}

enWinner roundWinner(enPlayerChoice playerChoice, enPlayerChoice ComputerChoice)
{
    if (playerChoice == ComputerChoice)
        return enWinner::Draw;

    switch (playerChoice)
    {
    case Rock:
        return (ComputerChoice == Scissors) ? enWinner::Player : enWinner::Computer;
    case Scissors:
        return (ComputerChoice == Paper) ? enWinner::Player : enWinner::Computer;
    case Paper:
        return (ComputerChoice == Rock) ? enWinner::Player : enWinner::Computer;
    }
    return enWinner::Draw;
}

void pointsOfPlayers(enWinner winner, stGamePoints &points)
{
    switch (winner)
    {
    case enWinner::Player:
        points.playerPoint++;
        break;
    case enWinner::Computer:
        points.computerPoint++;
        break;
    case enWinner::Draw:
        points.drawPoint++;
        break;
    }
}

void startRounds(int roundsNumber, stGamePoints &points)
{
    stRound roundInfo;

    for (int round = 0; round < roundsNumber; round++)
    {
        cout << "\nRound [" << round + 1 << "] begins: \n";
        roundInfo.players.playerChoice = readPlayerChoice();
        roundInfo.players.computerChoice = generateComputerChoice();
        roundInfo.winner = roundWinner(roundInfo.players.playerChoice, roundInfo.players.computerChoice);

        pointsOfPlayers(roundInfo.winner, points);
        displayRoundDetails(roundInfo.players, roundInfo.winner, round);
    }
}

void startGame()
{
    stGamePoints points;
    int roundsNumber = readRoundsNumber("how many rounds do you want to play? ");

    startRounds(roundsNumber, points);

    displayGameOverScreen();
    displayGameResult(roundsNumber, points);
}

int main()
{
    srand((unsigned)time(NULL));

    do
    {
        system("cls");
        startGame();
    } while (confirm("\t\tdo you want play again? (y/n):  "));

    return 0;
}
