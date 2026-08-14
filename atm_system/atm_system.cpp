#include <iostream>
#include <limits>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

const string clientsFile = "Build/clients.txt";

struct stClient
{
    string id, pinCode, name, phone;
    double balance;
    bool markedForDeletion = false;
};

enum enMainMenuOption
{
    QuickWithdraw = 1,
    NormalWithdraw,
    Deposit,
    CheckBalance,
    Logout
};

stClient currentClient;

// Prototypes

// Helpers
string clientToLine(const stClient &client, const string &separator);
void SaveCleintsDataToFile(const vector<stClient> &vClients);
bool confirmAction(const string &message);
void headerScreen(const string &title);
vector<string> split(string line, const string &separator);
stClient lineToClient(string line);
vector<stClient> loadClientFileToVector();

// Readers
enMainMenuOption readMainMenuOption(const string &message);
string readString(const string &message);
int readNumberInRange(const string &message, int from, int to);
int readDepositAmount(const string &message);

// Deposit
void deposit(vector<stClient> &vClients, int depositAmount);
void displayDeposit();

// Quick Withdraw
void showQuickWithdrawOptions();
int getQuickWithdrawAmount(int option);
bool handleQuickWithdrawOption(int option);
void displayQuickWithdraw();

// Normal Withdraw
void normalWithdraw();
void displayNormalWithdraw();

// Check Balance
void displayCheckBalance();

// Menu
void pause();
void showMainMenu();
void handleMainMenuOption(enMainMenuOption option);
void mainMenu();

// Login
bool findClientByIdAndPinCode(const vector<stClient> &vClients,
                              const string &id,
                              const string &pinCode,
                              stClient &client);
bool checkLogin(const vector<stClient> &vClients);
bool login();

// Helplers
string clientToLine(const stClient &client, const string &separator)
{
    string line = "";

    line = client.id + separator;
    line += client.pinCode + separator;
    line += client.name + separator;
    line += client.phone + separator;
    line += to_string(client.balance);

    return line;
}

void SaveCleintsDataToFile(const vector<stClient> &vClients)
{
    ofstream file(clientsFile);

    if (file.is_open())
    {
        for (const stClient &client : vClients)
        {
            if (!client.markedForDeletion)
                file << clientToLine(client, "#//#") << endl;
        }

        file.close();
    }
    else
    {
        cout << "\nfile not found\n";
    }
}

bool confirmAction(const string &message)
{
    char answer = ' ';
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

void headerScreen(const string &title)
{
    cout << "\t\t   " << title << endl;
    cout << string(50, '-') << endl;
}

vector<string> split(string line, const string &separator) // test
{
    vector<string> vTokens;
    string word;
    size_t pos;

    while ((pos = line.find(separator)) != string::npos)
    {
        word = line.substr(0, pos);

        if (!word.empty())
            vTokens.push_back(word);

        line.erase(0, pos + separator.length());
    }

    if (!line.empty())
        vTokens.push_back(line);

    return vTokens;
}

stClient lineToClient(string line)
{
    stClient client;
    vector<string> vTokens = split(line, "#//#");

    if (vTokens.size() > 5)
        return {};

    client.id = vTokens[0];
    client.pinCode = vTokens[1];
    client.name = vTokens[2];
    client.phone = vTokens[3];
    client.balance = stod(vTokens[4]);

    return client;
}

vector<stClient> loadClientFileToVector()
{
    vector<stClient> vClients;
    ifstream file(clientsFile);

    if (file.is_open())
    {
        string line = "";
        while (getline(file, line))
        {
            vClients.push_back(lineToClient(line));
        }

        file.close();
    }
    else
        cout << "\nFile not found\n";

    return vClients;
}

// Readers
enMainMenuOption readMainMenuOption(const string &message)
{
    int option = 0;
    cout << message;
    cin >> option;

    while (cin.fail() || option < 1 || option > 5)
    {
        if (cin.fail())
            cout << "invalid input, please try again: ";
        else
            cout << "Please enter a number between 1-5: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> option;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return static_cast<enMainMenuOption>(option);
}

string readString(const string &message)
{
    string s;
    cout << message;
    getline(cin >> ws, s);
    return s;
}

int readNumberInRange(const string &messsage, int from, int to)
{
    int number;
    cout << messsage;
    cin >> number;

    bool isFail = false;
    while ((isFail = cin.fail()) || number < from || number > to)
    {
        if (isFail)
            cout << "Invalid input. Please try again: ";
        else
            cout << "Please enter a number between " << from << "-" << to << ": ";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> number;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return number;
}

// Quick Withdraw

void showQuickWithdrawOptions()
{
    cout << "\t[1] 20";
    cout << "\t\t[2] 50\n";
    cout << "\t[3] 100";
    cout << "\t\t[4] 200\n";
    cout << "\t[5] 400";
    cout << "\t\t[6] 600\n";
    cout << "\t[7] 800";
    cout << "\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << string(50, '-') << endl;
}

int getQuickWithdrawAmount(int option)
{
    int amounts[] = {20, 50, 100, 200, 400, 600, 800, 1000};

    return amounts[option - 1];
}

bool handleQuickWithdrawOption(vector<stClient> &vClients, int option)
{
    if (option == 9)
        return true;

    int withdrawBalance = getQuickWithdrawAmount(option);

    if (withdrawBalance > currentClient.balance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        return false;
    }

    if (confirmAction("Are you sure you want to perform this transaction? [y/n]: "))
    {
        deposit(vClients, withdrawBalance * -1);
        return true;
    }
    else
    {
        cout << "Transaction has been cancelled.\n";
        return true;
    }
}

void displayQuickWithdraw()
{
    int option = 0;
    vector<stClient> vClients = loadClientFileToVector();
    do
    {
        system("cls");
        headerScreen("Quick Withdraw");
        showQuickWithdrawOptions();

        cout << "your balance is: " << currentClient.balance << endl;
        option = readNumberInRange("choose an option [1-9]: ", 1, 9);

    } while (!handleQuickWithdrawOption(vClients, option));
}

// Normal Withdraw
void normalWithdraw(vector<stClient> &vClients, int withdrawAmount)
{
    if (withdrawAmount > currentClient.balance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        return;
    }

    if (confirmAction("Are you sure you want to perform this transaction? [y/n]: "))
    {
        deposit(vClients, -withdrawAmount);
    }
}

void displayNormalWithdraw()
{
    headerScreen("Normal Withdraw");

    vector<stClient> vClients = loadClientFileToVector();
    int withdrawAmount = readDepositAmount("Enter withdraw amount: ");

    normalWithdraw(vClients, withdrawAmount);
}

// Deposit
int readDepositAmount(const string &message)
{
    int amount = 0;
    cout << message;
    cin >> amount;

    bool isFail = false;
    while ((isFail = cin.fail()) || amount < 1)
    {
        if (isFail)
            cout << "Invalid input. Please try again: ";
        else
            cout << "Please enter a positive deposit amount: ";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> amount;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return amount;
}

void deposit(vector<stClient> &vClients, int depositAmount)
{
    currentClient.balance += depositAmount;

    for (stClient &client : vClients)
    {
        if (client.id == currentClient.id)
        {
            client.balance = currentClient.balance;
            cout << "done successfully, new balance is: " << currentClient.balance << endl;
            SaveCleintsDataToFile(vClients);
            break;
        }
    }
}

void displayDeposit()
{
    headerScreen("Deposit Screen");

    vector<stClient> vClients = loadClientFileToVector();
    int depositAmount = readDepositAmount("enter deposit amount: ");

    deposit(vClients, depositAmount);
}

// Check Balance
void displayCheckBalance()
{
    headerScreen("Client Balance Screen");
    cout << "Your balance is: " << currentClient.balance << endl;
}

// Start
void pause()
{
    cout << "\nPress Enter To Continue . . .\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void showMainMenu()
{
    headerScreen("Main Menu Screen");
    cout << "[1] Quick Withdraw\n";
    cout << "[2] Normal withdraw\n";
    cout << "[3] Deposit\n";
    cout << "[4] Check Balance\n";
    cout << "[5] Logout\n";
    cout << string(50, '-') << endl;
}

void handleMainMenuOption(enMainMenuOption option)
{
    system("cls");

    switch (option)
    {
    case QuickWithdraw:
        displayQuickWithdraw();
        break;

    case NormalWithdraw:
        displayNormalWithdraw();
        break;

    case Deposit:
        displayDeposit();
        break;

    case CheckBalance:
        displayCheckBalance();
        break;

    case Logout:
        break;
    }
}

void mainMenu()
{
    enMainMenuOption option;
    do
    {
        system("cls");
        showMainMenu();
        option = readMainMenuOption("choose an option [1-5]: ");
        handleMainMenuOption(option);

        if (option != Logout)
            pause();

    } while (option != Logout);
}

// Login
bool findClientByIdAndPinCode(const vector<stClient> &vClients,
                              const string &id,
                              const string &pinCode,
                              stClient &client)
{
    for (const stClient &c : vClients)
    {
        if (c.id == id && c.pinCode == pinCode)
        {
            client = c;
            return true;
        }
    }

    return false;
}

bool checkLogin(const vector<stClient> &vClients)
{
    string id = readString("Enter ID: ");
    string pinCode = readString("Enter PIN Code: ");

    return findClientByIdAndPinCode(vClients, id, pinCode, currentClient);
}

bool login()
{
    vector<stClient> vClients = loadClientFileToVector();

    headerScreen("Login Screen");
    while (!checkLogin(vClients))
    {
        system("cls");
        headerScreen("Login Screen");
        cout << "Invalid ID or PIN code\n";
    }

    return true;
}

int main()
{
    while (login())
    {
        mainMenu();
    }
    return 0;
}
