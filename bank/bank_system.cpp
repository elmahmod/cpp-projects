#include <iostream> // main
#include <fstream>  // file
#include <vector>   // vector
#include <iomanip>  // setw()
#include <limits>   // numeric_limits<T>::T()
#include <string>   // getline()
using namespace std;

const string clientFile = "Build/clients.txt";

enum enMainMenuOption
{
    ShowClients = 1,
    AddClient,
    DeleteClient,
    UpdateClient,
    FindClient,
    Exit
};

struct stClient
{
    string id, pinCode, name, phone;
    double balance;
    bool markedForDeletion = false;
};

// helplers

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
    ifstream file(clientFile);

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
    {
        cout << "\nFile not found\n";
    }

    return vClients;
}

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

void saveChangesToFile(const vector<stClient> &vClients)
{
    ofstream file(clientFile);

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

int findClientIndex(const vector<stClient> &vClients, const string &id)
{
    for (int i = 0; i < vClients.size(); i++)
    {
        if (vClients[i].id == id)
            return i;
    }
    return -1;
}

void printClientData(const stClient &client)
{
    cout << left << setw(10) << "Id" << ": " << client.id << '\n';
    cout << left << setw(10) << "Name" << ": " << client.name << '\n';
    cout << left << setw(10) << "Pin code" << ": " << client.pinCode << '\n';
    cout << left << setw(10) << "Phone" << ": " << client.phone << '\n';
    cout << left << setw(10) << "Balance" << ": " << client.balance << endl;
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

bool doesClientExist(const string &id)
{
    vector<stClient> vClients = loadClientFileToVector();
    for (const stClient &client : vClients)
    {
        if (client.id == id)
            return true;
    }
    return false;
}

// readers

int readNumberInRange(int from, int to)
{
    int number;
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

enMainMenuOption readMainMenuOption(const string &message)
{
    cout << message;
    return static_cast<enMainMenuOption>(readNumberInRange(1, 6));
}

string readString(const string &message)
{
    string text;
    cout << message;
    getline(cin >> ws, text);
    return text;
}

double readPositiveDouble(const string &message)
{
    double number;
    cout << message;
    cin >> number;

    while (number < 0 || cin.fail())
    {
        if (number < 0)
            cout << "Please enter a positive number: ";
        else
            cout << "Invalid input. Please try again: ";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> number;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return number;
}

stClient readNewClient()
{
    stClient client;
    client.id = readString("Enter Id: ");

    while (doesClientExist(client.id))
    {
        client.id = readString("Client already exists. Try another ID: ");
    }

    client.pinCode = readString("Enter Pin Code: ");
    client.name = readString("Enter Name: ");
    client.phone = readString("Enter Phone: ");
    client.balance = readPositiveDouble("Enter Balance: ");

    return client;
}

// show

void showClientsList(const vector<stClient> &vClients)
{
    cout << "\t\t\t Client Members [" << vClients.size() << "]\n";
    cout << string(50, '-') << endl;

    cout << "| " << left << setw(15) << "Id";
    cout << "| " << left << setw(15) << "Pin code";
    cout << "| " << left << setw(18) << "Name";
    cout << "| " << left << setw(15) << "Phone";
    cout << "| " << left << setw(15) << "Balance" << endl;

    for (const stClient &client : vClients)
    {
        cout << "| " << left << setw(15) << client.id;
        cout << "| " << left << setw(15) << client.pinCode;
        cout << "| " << left << setw(18) << client.name;
        cout << "| " << left << setw(15) << client.phone;
        cout << "| " << left << setw(15) << client.balance << endl;
    }
    cout << string(50, '_') << endl;
}

void displayShowClients()
{
    vector<stClient> vClients = loadClientFileToVector();

    showClientsList(vClients);
}

// add

void addNewClient(vector<stClient> &vClients)
{

    vClients.push_back(readNewClient());

    cout << "\nClient added successfully.\n";
    saveChangesToFile(vClients);
}

void displayAddNewClient()
{
    headerScreen("Add New Client");

    vector<stClient> vClients = loadClientFileToVector();
    do
    {
        addNewClient(vClients);
    } while (confirmAction("Add another client? [y/n]: "));
}

// delete

void deleteClient(vector<stClient> &vClients, const string &id)
{
    int index = findClientIndex(vClients, id);
    if (index == -1)
    {
        cout << "\nclient not found.\n";
        return;
    }

    printClientData(vClients[index]);
    if (confirmAction("Do you want to delete this client? [y/n]: "))
    {
        vClients[index].markedForDeletion = true;
        saveChangesToFile(vClients);
    }
    else
    {
        cout << "\nDeletion has been canceled\n";
    }
}

void displayDeleteClient()
{
    headerScreen("Delete Client");

    vector<stClient> vClients = loadClientFileToVector();
    const string id = readString("Enter Id: ");
    deleteClient(vClients, id);
}

// update

void updateClientData(stClient &client)
{
    client.pinCode = readString("Enter Pin Code: ");
    client.name = readString("Enter Name: ");
    client.phone = readString("Enter Phone: ");
    client.balance = readPositiveDouble("Enter Balance: ");
}

void updateClient(vector<stClient> &vClients, const string &id)
{
    int index = findClientIndex(vClients, id);
    if (index == -1)
    {
        cout << "\nclient not found\n";
        return;
    }

    printClientData(vClients[index]);
    if (confirmAction("Do you want to update this client? [y/n]: "))
    {
        updateClientData(vClients[index]);
        saveChangesToFile(vClients);
    }
    else
    {
        cout << "\nUpdate has been canceled.\n";
    }
}

void displayUpdateClient()
{
    headerScreen("Update Client");
    vector<stClient> vClients = loadClientFileToVector();
    string id = readString("Enter Id: ");
    updateClient(vClients, id);
}

// find

void findClient(const vector<stClient> &vClients, const string &id)
{
    int index = findClientIndex(vClients, id);

    if (index == -1)
    {
        cout << "\nclient not found\n";
        return;
    }

    printClientData(vClients[index]);
}

void displayFindClient()
{
    headerScreen("Find Client");

    vector<stClient> vClients = loadClientFileToVector();
    string id = readString("Enter Id: ");
    findClient(vClients, id);
}

// start

void pause()
{
    cout << "\nPress Enter To Continue . . .\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void showMainMenu()
{
    headerScreen("Main Menu Screen");
    cout << "[1] Show Client List\n";
    cout << "[2] Add New Client\n";
    cout << "[3] Delete Client\n";
    cout << "[4] Update Client\n";
    cout << "[5] Find Client\n";
    cout << "[6] Exit\n";
    cout << string(50, '-') << endl;
}

void handleMainMenuOption(enMainMenuOption option)
{
    system("cls");

    switch (option)
    {
    case ShowClients:
        displayShowClients();
        break;

    case AddClient:
        displayAddNewClient();
        break;

    case DeleteClient:
        displayDeleteClient();
        break;

    case UpdateClient:
        displayUpdateClient();
        break;

    case FindClient:
        displayFindClient();
        break;

    case Exit:
        cout << "\nProgram closed\n";
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
        option = readMainMenuOption("Choose an option [1-6]: ");
        handleMainMenuOption(option);

        if (option != Exit)
            pause();

    } while (option != Exit);
}

int main()
{
    mainMenu();
    return 0;
}
