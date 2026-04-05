#include <iostream>
#include <limits>
#include <iomanip>
using namespace std;

enum enSystemMenuOptions
{
    AddStudent = 1,
    DisplayStudentList,
    SearchStudent,
    ModifyStudent,
    DeleteStudent,
    Exit
};

enum enColor
{
    Red = 1,
    Green,
    Yellow,
    Reset
};

struct stStudent
{
    string firstName;
    string lastName;
    string id;
    bool markedForDeletion = false;
};

// helpers
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

void setColor(enColor color)
{
    switch (color)
    {
    case Red:
        cout << "\033[31m";
        break;

    case Green:
        cout << "\033[32m";
        break;

    case Yellow:
        cout << "\033[33m";
        break;

    case Reset:
        cout << "\033[0m";
        break;
    }
}

bool doesStudentExist(stStudent studentList[100], int studentsNumber, string id)
{
    for (int i = 0; i < studentsNumber; i++)
    {
        if (studentList[i].id == id && !studentList[i].markedForDeletion)
            return true;
    }
    return false;
}

int findStudent(stStudent studentList[], int studentsNumber, string id)
{
    for (int i = 0; i < studentsNumber; i++)
    {
        if (studentList[i].id == id && !studentList[i].markedForDeletion)
            return i;
    }
    return -1;
}

void printStudentInfo(const stStudent &student)
{
    cout << setw(20) << left << "\nId Number  : " << student.id << endl;
    cout << setw(20) << left << "First Name : " << student.firstName << endl;
    cout << setw(20) << left << "Last Name  : " << student.lastName << endl;
}

// readers
enSystemMenuOptions readSystemMenuOption()
{
    int option = 0;
    cout << "\n\t\t\t\t" << "Choose an option: ";
    cin >> option;

    while (cin.fail() || option < 1 || option > 6)
    {
        if (cin.fail())
            cout << "Invalid input, tray again: ";
        else
            cout << "Option must be between (1-6): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> option;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return static_cast<enSystemMenuOptions>(option);
}

string readString(const string &message)
{
    string text;
    cout << message;
    getline(cin >> ws, text);
    return text;
}

// add
stStudent readStudentInfo()
{
    stStudent info;

    cout << "Enter First name : ";
    getline(cin >> ws, info.firstName);

    cout << "Enter Last name  : ";
    getline(cin >> ws, info.lastName);

    return info;
}

void addStudent(stStudent studentList[], int &studentsNumber)
{
    cout << "\t\t--- Add New Student ---\n";
    do
    {
        string id = readString("\nEnter Id number  : ");

        if (!doesStudentExist(studentList, studentsNumber, id))
        {
            studentList[studentsNumber] = readStudentInfo();
            studentList[studentsNumber].id = id;
            studentsNumber++;
        }
        else
        {
            setColor(Red);
            cout << "\nID already exists!\n";
            setColor(Reset);
        }

    } while (confirm("\nAdd more? (y/n): "));
}

// display
void printStudentListHeader(int studentsNumber)
{
    cout << "\n\n\t\t\t\t" << string(57, '=');
    setColor(Yellow);
    cout << "\n\t\t\t\t\t" << "            Students List Of (" << studentsNumber << ")";
    setColor(Reset);
    cout << "\n\t\t\t\t" << string(57, '=');
    cout << '\n';

    cout << "\n\t\t\t\t"
         << "| " << left << setw(16) << "Id Number"
         << "| " << setw(20) << "First Name"
         << "| " << setw(14) << "Last Name" << "|\n";
    cout << "\t\t\t\t" << string(57, '-');
    cout << endl;
}

void printStudentRowInfo(const stStudent &student)
{
    cout << "\t\t\t\t"
         << "| " << left << setw(16) << student.id
         << "| " << setw(20) << student.firstName
         << "| " << setw(14) << student.lastName << "|\n";
}

int activeStudents(const stStudent studentList[], int studentsNumber)
{
    int active = 0;
    for (int i = 0; i < studentsNumber; i++)
    {
        if (!studentList[i].markedForDeletion)
            active++;
    }
    return active;
}

void displayStudentList(const stStudent studentList[], int studentsNumber)
{
    printStudentListHeader(activeStudents(studentList, studentsNumber));

    for (int i = 0; i < studentsNumber; i++)
    {
        if (!studentList[i].markedForDeletion)
            printStudentRowInfo(studentList[i]);
    }
}

// search
void searchStudent(stStudent studentList[], int studentsNumber)
{
    string id = readString("Enter Student Id: ");
    int index = findStudent(studentList, studentsNumber, id);

    if (index != -1)
    {
        printStudentInfo(studentList[index]);
    }
    else
    {
        setColor(Red);
        cout << "\nStudent not found!\n";
        setColor(Reset);
    }
}

// modify
stStudent modifyStudentInfo(string id)
{
    stStudent info;
    info.id = id;

    cout << "\nEnter First name: ";
    getline(cin >> ws, info.firstName);

    cout << "Enter Last name: ";
    getline(cin >> ws, info.lastName);

    return info;
}

void modifyStudent(stStudent studentList[], int studentsNumber)
{
    string id = readString("\nEnter Student Id: ");
    int index = findStudent(studentList, studentsNumber, id);

    if (index != -1)
    {
        printStudentInfo(studentList[index]);

        if (confirm("do you want modify it? (y/n): "))
        {
            studentList[index] = modifyStudentInfo(id);
            setColor(Green);
            cout << "\nStudent details updated.\n";
            setColor(Reset);
        }
        else
        {
            setColor(Red);
            cout << "\nCanceled.\n";
            setColor(Reset);
        }
    }
    else
    {
        setColor(Red);
        cout << "\nStudent not found!\n";
        setColor(Reset);
    }
}

// delete
void deleteStudent(stStudent studentList[], int studentsNumber)
{
    string id = readString("Enter Student Id: ");
    int index = findStudent(studentList, studentsNumber, id);

    if (index != -1)
    {
        printStudentInfo(studentList[index]);

        if (confirm("do you want delete it? (y/n): "))
        {
            studentList[index].markedForDeletion = true;
            setColor(Green);
            cout << "\nStudent has deleted.\n";
            setColor(Reset);
        }
        else
        {
            setColor(Red);
            cout << "\nCanceled.\n";
            setColor(Reset);
        }
    }
    else
    {
        setColor(Red);
        cout << "\nStudent not found!\n";
        setColor(Reset);
    }
}

// handle options
void handleOption(enSystemMenuOptions option, stStudent studentList[], int &studentsNumber)
{
    system("cls");
    switch (option)
    {
    case AddStudent:
        addStudent(studentList, studentsNumber);
        break;

    case DisplayStudentList:
        displayStudentList(studentList, studentsNumber);
        break;

    case SearchStudent:
        searchStudent(studentList, studentsNumber);
        break;

    case ModifyStudent:
        modifyStudent(studentList, studentsNumber);
        break;

    case DeleteStudent:
        deleteStudent(studentList, studentsNumber);
        break;
    }
}

// main shows
void displayOptionsHeader()
{
    cout << "\n\n\t\t\t\t" << string(35, '=');
    setColor(Yellow);
    cout << "\n\t\t\t\t\t" << "   Student System";
    setColor(Reset);
    cout << "\n\t\t\t\t" << string(35, '=');
    cout << '\n';
}

void displayOptionsMenu()
{
    displayOptionsHeader();
    cout << "\t\t\t\t"
         << "[1] Add Student\n";
    cout << "\t\t\t\t"
         << "[2] Display Student List\n";
    cout << "\t\t\t\t"
         << "[3] Search Student\n";
    cout << "\t\t\t\t"
         << "[4] Modify Student\n";
    cout << "\t\t\t\t"
         << "[5] Delete Student\n";
    setColor(Red);
    cout << "\t\t\t\t"
         << "[6] Exit\n";
    setColor(Reset);
    cout << "\t\t\t\t" << string(35, '-');
}

void pressToContinue()
{
    cout << "\npress to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main()
{
    enSystemMenuOptions option;
    stStudent studentList[100];
    int studentsNumber = 0;

    do
    {
        system("cls");

        displayOptionsMenu();
        option = readSystemMenuOption();
        handleOption(option, studentList, studentsNumber);

        if (option != Exit)
            pressToContinue();

    } while (option != Exit);

    setColor(Yellow);
    cout << "\nprogram has ended.\n";
    setColor(Reset);
    return 0;
}
