#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
using namespace std;

enum enToDoListMenuOptions
{
    AddTask = 1,
    DisplayTasks,
    ModifyTask,
    CompleteTask,
    SearchTask,
    DeleteTask,
    Exit
};

enum enColor
{
    Red = 1,
    Green,
    Yellow,
    Reset
};

struct stTask
{
    string id = "";
    string title = "";
    string description = "";
    bool isCompleted = false;
    bool markedForDeletion = false;
};

// readers
enToDoListMenuOptions readSystemMenuOption()
{
    int option = 0;
    cout << "\t\t\t\t" << "Choose an option: ";
    cin >> option;

    while (cin.fail() || option < 1 || option > 7)
    {
        if (cin.fail())
            cout << "Invalid input, tray again: ";
        else
            cout << "Option must be between (1-7): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> option;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return static_cast<enToDoListMenuOptions>(option);
}

string readString(const string &message)
{
    string text;
    cout << message;
    getline(cin >> ws, text);
    return text;
}

stTask readTaskInfo(string id)
{
    stTask t;
    t.id = id;
    t.title = readString("Enter Title       : ");
    t.description = readString("Enter Subject     : ");
    return t;
}

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

int findTask(const stTask taskList[], int tasksNumber, string id)
{
    for (int i = 0; i < tasksNumber; i++)
    {
        if (taskList[i].id == id && !taskList[i].markedForDeletion)
            return i;
    }
    return -1;
}

bool doesTaskExist(const stTask taskList[], int tasksNumber, string id)
{
    return findTask(taskList, tasksNumber, id) != -1;
}

string getTaskCompletedText(bool isCompleted)
{
    if (isCompleted)
    {
        return "Completed";
    }
    else
    {
        return "Uncompleted";
    }
}

void printTaskInfo(const stTask &task)
{
    cout << '\n';
    cout << setw(15) << left << "Id number" << ": " << task.id << endl;
    cout << setw(15) << left << "Title" << ": " << task.title << endl;
    cout << setw(15) << left << "Subject" << ": " << task.description << endl;
    cout << setw(15) << left << "State" << ": " << getTaskCompletedText(task.isCompleted) << endl;
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

// Add
void addTask(stTask taskList[], int &tasksNumber, string id)
{
    if (doesTaskExist(taskList, tasksNumber, id))
    {
        setColor(Red);
        cout << "\nTask Already Exists.";
        setColor(Reset);
        return;
    }
    taskList[tasksNumber] = readTaskInfo(id);
    tasksNumber++;
}

void displayAddTask(stTask taskList[], int &tasksNumber)
{
    if (tasksNumber >= 100)
    {
        setColor(Red);
        cout << "\nList is full, cannot add more tasks!";
        setColor(Reset);
        return;
    }

    do
    {
        string id = readString("\nEnter Id Number   : ");
        addTask(taskList, tasksNumber, id);
    } while (confirm("\nAdd more? (y/n): "));
}

// Display
int countExistingTasks(stTask taskList[], int tasksNumber)
{
    int active = 0;
    for (int i = 0; i < tasksNumber; i++)
    {
        if (!taskList[i].markedForDeletion)
            active++;
    }
    return active;
}

void displayTaskHeader(int tasksNumber)
{
    cout << "\n\t\t\t\t" << string(70, '=') << endl;
    setColor(Yellow);
    cout << "\t\t\t\t\t\t\t" << "  Tasks List of (" << tasksNumber << ")" << endl;
    setColor(Reset);
    cout << "\t\t\t\t" << string(70, '=') << endl;
}

void displayTaskRow()
{
    cout << "\n\t\t\t\t"
         << "| " << left << setw(14) << "   Id Number"
         << "| " << setw(14) << "   Title"
         << "| " << setw(20) << "   Subject"
         << "| " << setw(14) << "   State" << "|\n";
    cout << "\t\t\t\t" << string(70, '-') << endl;
}

void printTaskRowInfo(const stTask &task)
{
    cout << "\t\t\t\t"
         << "| " << left << setw(14) << task.id
         << "| " << setw(14) << task.title
         << "| " << setw(20) << task.description
         << "| " << setw(14) << getTaskCompletedText(task.isCompleted) << "|\n";
}

void displayTaskList(stTask taskList[], int tasksNumber)
{
    displayTaskHeader(countExistingTasks(taskList, tasksNumber));
    displayTaskRow();

    for (int i = 0; i < tasksNumber; i++)
    {
        if (!taskList[i].markedForDeletion)
            printTaskRowInfo(taskList[i]);
    }
    cout << "\t\t\t\t" << string(70, '-') << endl;
}

// Modify
void modifyTask(stTask taskList[], int tasksNumber, string id)
{
    int index = findTask(taskList, tasksNumber, id);

    if (index == -1)
    {
        setColor(Red);
        cout << "\nTask Not Found.";
        setColor(Reset);
        return;
    }

    printTaskInfo(taskList[index]);

    if (confirm("\ndo you want to modify it? (y/n): "))
    {
        bool completed = taskList[index].isCompleted;
        taskList[index] = readTaskInfo(id);
        taskList[index].isCompleted = completed;

        setColor(Green);
        cout << "\nTask is updated";
        setColor(Reset);
    }
    else
    {
        setColor(Red);
        cout << "\nCanceled.";
        setColor(Reset);
    }
}

void displayModifyTask(stTask taskList[], int tasksNumber)
{
    string id = readString("Enter Id number: ");
    modifyTask(taskList, tasksNumber, id);
}

// Complete
void completeTask(stTask taskList[], int tasksNumber, string id)
{
    int index = findTask(taskList, tasksNumber, id);

    if (index == -1)
    {
        setColor(Red);
        cout << "\nTask Not Found.";
        setColor(Reset);
        return;
    }

    printTaskInfo(taskList[index]);

    if (taskList[index].isCompleted)
    {
        setColor(Red);
        cout << "\nTask is already completed.";
        setColor(Reset);
        return;
    }

    if (confirm("\nmark as completed? (y/n): "))
    {
        taskList[index].isCompleted = true;
        setColor(Green);
        cout << "\nMarked as completed";
        setColor(Reset);
    }
    else
    {
        setColor(Red);
        cout << "\nCanceled.";
        setColor(Reset);
    }
}

void displayCompleteTask(stTask taskList[], int tasksNumber)
{
    string id = readString("Enter Id number: ");
    completeTask(taskList, tasksNumber, id);
}

// Search
void displaySearchTask(stTask taskList[], int tasksNumber)
{
    string id = readString("Enter Id number: ");
    int index = findTask(taskList, tasksNumber, id);

    if (index == -1)
    {
        setColor(Red);
        cout << "\nTask Not Found.";
        setColor(Reset);
        return;
    }
    printTaskInfo(taskList[index]);
}

// Delete
void deleteTask(stTask taskList[], int tasksNumber, string id)
{
    int index = findTask(taskList, tasksNumber, id);
    if (index == -1)
    {
        setColor(Red);
        cout << "\nTask Not Found.";
        setColor(Reset);
        return;
    }

    printTaskInfo(taskList[index]);

    if (confirm("\ndo you want to delete it? (y/n): "))
    {
        taskList[index].markedForDeletion = true;
        setColor(Green);
        cout << "\nTask is Deleted";
        setColor(Reset);
    }
    else
    {
        setColor(Red);
        cout << "\nCanceled.";
        setColor(Reset);
    }
}

void displayDeleteTask(stTask taskList[], int tasksNumber)
{
    string id = readString("Enter Id number: ");
    deleteTask(taskList, tasksNumber, id);
}

// main show
void handleOption(enToDoListMenuOptions option, stTask taskList[], int &tasksNumber)
{
    system("cls");
    switch (option)
    {
    case AddTask:
        displayAddTask(taskList, tasksNumber);
        break;

    case DisplayTasks:
        displayTaskList(taskList, tasksNumber);
        break;

    case ModifyTask:
        displayModifyTask(taskList, tasksNumber);
        break;

    case CompleteTask:
        displayCompleteTask(taskList, tasksNumber);
        break;

    case SearchTask:
        displaySearchTask(taskList, tasksNumber);
        break;

    case DeleteTask:
        displayDeleteTask(taskList, tasksNumber);
        break;
    }
}

void displayHeader(const string &title = "")
{
    cout << "\n\t\t\t\t" << string(35, '=') << endl;
    setColor(Yellow);
    cout << "\t\t\t\t\t" << "    To Do List" << endl;
    setColor(Reset);
    cout << "\t\t\t\t" << string(35, '=') << endl;
}

void displayMenuOptions()
{
    displayHeader();
    cout << "\t\t\t\t" << "[1] Add task\n";
    cout << "\t\t\t\t" << "[2] Display task\n";
    cout << "\t\t\t\t" << "[3] Modify task\n";
    cout << "\t\t\t\t" << "[4] Complete task\n";
    cout << "\t\t\t\t" << "[5] Search task\n";
    cout << "\t\t\t\t" << "[6] Delete task\n";
    setColor(Red);
    cout << "\t\t\t\t" << "[7] Exit\n";
    setColor(Reset);
    cout << "\t\t\t\t" << string(35, '-') << endl;
}

void pressToContinue()
{
    setColor(Yellow);
    cout << "\npress to continue...";
    setColor(Reset);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main()
{
    enToDoListMenuOptions option;
    stTask taskList[100];
    int tasksNumber = 0;
    do
    {
        system("cls");

        displayMenuOptions();
        option = readSystemMenuOption();
        handleOption(option, taskList, tasksNumber);

        if (option != Exit)
            pressToContinue();

    } while (option != Exit);

    setColor(Yellow);
    cout << "\nprogram has ended.";
    setColor(Reset);

    return 0;
}
