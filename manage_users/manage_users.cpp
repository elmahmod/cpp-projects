#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <limits>
#include <cctype>
#include <string>
using namespace std;

const string usersFile = "Build/users.txt";

enum enUserMainMenu
{
    ShowUsers = 1,
    AddUser,
    DeleteUser,
    UpdateUser,
    FindUser,
    Logout
};

enum enPermissions
{
    eRead = 1,
    eAdd = 2,
    eDelete = 4,
    eUpdate = 8,
    eFind = 16,
    eALL = -1
};

struct stUser
{
    string username, password;
    int permission = 0;
    bool markedForDeletion = false;
};

stUser currentUser;

// helplers
void headerScreen(const string &title)
{
    cout << "\t\t   " << title << endl;
    cout << string(50, '-') << endl;
}

void pause()
{
    cout << "\nPress Enter To Continue . . .\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

vector<string> split(string line, const string &separator)
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

stUser lineToUser(string line)
{
    stUser user;
    vector<string> vTokens = split(line, "#//#");

    if (vTokens.size() != 3)
        return {};

    user.username = vTokens[0];
    user.password = vTokens[1];
    user.permission = stoi(vTokens[2]);

    return user;
}

vector<stUser> loadUserFileToVector()
{
    vector<stUser> vUsers;
    ifstream file(usersFile);

    if (file.is_open())
    {
        string line = "";
        while (getline(file, line))
        {
            vUsers.push_back(lineToUser(line));
        }

        file.close();
    }
    else
    {
    }
    return vUsers;
}

string userToLine(stUser user, const string &separator)
{
    string line = "";
    line = user.username + separator;
    line += user.password + separator;
    line += to_string(user.permission);
    return line;
}

void saveUsersDataToFile(vector<stUser> &vUsers)
{
    ofstream file(usersFile);

    if (file.is_open())
    {
        for (stUser user : vUsers)
        {
            if (!user.markedForDeletion)
                file << userToLine(user, "#//#") << endl;
        }

        file.close();
    }
    else
    {
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

bool doesUserExist(const string &username)
{
    vector<stUser> vUsers = loadUserFileToVector();
    for (const stUser &user : vUsers)
    {
        if (user.username == username)
            return true;
    }
    return false;
}

void printUserData(const stUser &user)
{
    cout << left << setw(10) << "Username" << ": " << user.username << '\n';
    cout << left << setw(10) << "Password" << ": " << user.password << '\n';
    cout << left << setw(10) << "permission" << ": " << user.permission << endl;
}

int findUserIndex(const vector<stUser> &vUsers, const string &username)
{
    for (size_t i = 0; i < vUsers.size(); i++)
    {
        if (vUsers[i].username == username)
            return i;
    }
    return -1;
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

enUserMainMenu readMainMenuOption(const string &message)
{
    cout << message;
    return static_cast<enUserMainMenu>(readNumberInRange(1, 6));
}

string readString(const string &message)
{
    string text;
    cout << message;
    getline(cin >> ws, text);
    return text;
}

int readPermission()
{
    int permission = 0;

    if (confirmAction("Do you want to give full access? (y/n): "))
        return eALL;

    if (confirmAction("Access to Show users list? (y/n): "))
        permission |= eRead;

    if (confirmAction("Access to Add new user? (y/n): "))
        permission |= eAdd;

    if (confirmAction("Access to Delete user? (y/n): "))
        permission |= eDelete;

    if (confirmAction("Access to Update user? (y/n): "))
        permission |= eUpdate;

    if (confirmAction("Access to Find user? (y/n): "))
        permission |= eFind;

    return permission;

    // 0 0 0 0 -> 1:,,  1:,,  1:w  1:r (with acess)
    // 1 -> 0 0 0 1
    // 2 -> 0 0 1 0
    // 4 -> 0 1 0 0
    // 8 -> 1 0 0 0
    // example 3 = -> 0 0 1 1 (has to acess) to write and read

    // when reading use |
    // when confirming use &
    // 0 0 1 1 & write( 0 0 1 0 ) = 0 0 1 0 == write ( has eccess to write)
}

// Show Users
void showUsersList(const vector<stUser> &vUsers)
{
    cout << "\t\t\t User Members [" << vUsers.size() << "]\n";
    cout << string(80, '-') << endl;

    cout << "| " << left << setw(20) << "Username";
    cout << "| " << left << setw(20) << "Password";
    cout << "| " << left << setw(20) << "Permission" << endl;

    for (const stUser &user : vUsers)
    {
        cout << "| " << left << setw(20) << user.username;
        cout << "| " << left << setw(20) << user.password;
        cout << "| " << left << setw(20) << user.permission << endl;
    }

    cout << string(80, '_') << endl;
}

void displayShowUsers()
{
    vector<stUser> vUsers = loadUserFileToVector();

    showUsersList(vUsers);
}

// Add User
stUser readNewUser()
{
    stUser user;
    user.username = readString("Enter Username: ");
    while (doesUserExist(user.username))
    {
        user.username = readString("User already exists. Try another username: ");
    }
    user.password = readString("Enter password: ");
    user.permission = readPermission();

    return user;
}

void addNewUser(vector<stUser> &vUsers)
{
    vUsers.push_back(readNewUser());
}

void displayAddUser()
{
    headerScreen("Add New User Screen");
    vector<stUser> vUsers = loadUserFileToVector();

    do
    {
        addNewUser(vUsers);

        cout << "\nUser added successfully.\n";
        saveUsersDataToFile(vUsers);

    } while (confirmAction("Add another user? [y-n]: "));
}

// Delete User
void deleteUser(vector<stUser> &vUsers, const string &username)
{
    int index = findUserIndex(vUsers, username);

    if (index == -1)
    {
        cout << "\nUser not found.\n";
        return;
    }

    if (vUsers[index].permission == eALL)
    {
        cout << "\nCannot delete admin\n";
        return;
    }

    printUserData(vUsers[index]);

    if (confirmAction("Do you want to delete this user? [y/n]: "))
    {
        vUsers[index].markedForDeletion = true;
        saveUsersDataToFile(vUsers);
    }
    else
    {
        cout << "\nDeletion has been canceled.\n";
    }
}

void displayDeleteUser()
{
    headerScreen("Delete User");

    vector<stUser> vUsers = loadUserFileToVector();
    const string username = readString("Enter username: ");

    deleteUser(vUsers, username);
}

// Update User
void updateUserData(stUser &user)
{
    user.password = readString("Enter password: ");
    user.permission = readPermission();
}

void updateUser(vector<stUser> &vUsers, const string &username)
{
    int index = findUserIndex(vUsers, username);
    if (index == -1)
    {
        cout << "\nUser not found\n";
        return;
    }

    if (vUsers[index].permission == eALL)
    {
        cout << "\nCannot update admin\n";
        return;
    }

    printUserData(vUsers[index]);
    if (confirmAction("Do you want to update this user? [y/n]: "))
    {
        updateUserData(vUsers[index]);
        saveUsersDataToFile(vUsers);
    }
    else
    {
        cout << "\nUpdate has been canceled.\n";
    }
}

void displayupdateUser()
{
    headerScreen("Update User Screen");
    vector<stUser> vUsers = loadUserFileToVector();
    string username = readString("Enter username: ");
    updateUser(vUsers, username);
}

// Find User
void findUser(const vector<stUser> &vUsers, const string &username)
{
    int index = findUserIndex(vUsers, username);

    if (index == -1)
    {
        cout << "\nUser not found.\n";
        return;
    }

    printUserData(vUsers[index]);
}

void displayFindUser()
{
    headerScreen("Find User");

    vector<stUser> vUsers = loadUserFileToVector();
    string username = readString("Enter username: ");

    findUser(vUsers, username);
}

// User Main Menu
bool checkPermission(enPermissions permission)
{
    if (currentUser.permission == eALL)
        return true;

    return (currentUser.permission & permission) == permission;
}

void showUserMainMenuOptions()
{
    headerScreen("Main Menu Screen");
    cout << "[1] Show Users\n";
    cout << "[2] Add New Users\n";
    cout << "[3] Delete User\n";
    cout << "[4] Update Users\n";
    cout << "[5] Find User\n";
    cout << "[6] Logout\n";
    cout << string(50, '-') << endl;
}

void accessDenied()
{
    cout << "\nAccess Denied.\n";
}

void handleUserMainMenuOption(enUserMainMenu option)
{
    system("cls");

    switch (option)
    {
    case ShowUsers:
        if (checkPermission(eRead))
            displayShowUsers();
        else
            accessDenied();
        break;

    case AddUser:
        if (checkPermission(eAdd))
            displayAddUser();
        else
            accessDenied();
        break;

    case DeleteUser:
        if (checkPermission(eDelete))
            displayDeleteUser();
        else
            accessDenied();
        break;

    case UpdateUser:
        if (checkPermission(eUpdate))
            displayupdateUser();
        else
            accessDenied();
        break;

    case FindUser:
        if (checkPermission(eFind))
            displayFindUser();
        else
            accessDenied();
        break;

    case Logout:
        break;
    }
}

void userMainMenu()
{
    enUserMainMenu option;
    do
    {
        system("cls");

        showUserMainMenuOptions();
        option = readMainMenuOption("Choose an option [1-6]: ");
        handleUserMainMenuOption(option);

        if (option != Logout)
            pause();

    } while (option != Logout);
}

// Login Screen
bool findUserByUsernameAndPassword(const string &username, const string &password)
{
    vector<stUser> vUsers = loadUserFileToVector();

    for (const stUser &user : vUsers)
    {
        if (user.username == username && user.password == password)
        {
            currentUser = user;
            return true;
        }
    }

    return false;
}

void loginScreen()
{
    while (true)
    {
        system("cls");

        headerScreen("Login Screen");

        string username = readString("Enter Username: ");
        string password = readString("Enter Password: ");

        if (findUserByUsernameAndPassword(username, password))
        {
            userMainMenu();
        }
        else
        {
            cout << "\nInvalid Username or Password.\n";

            if (!confirmAction("Do you want to try again? [y/n]: "))
                break;
        }
    }
}

int main()
{
    loginScreen();
    return 0;
}
