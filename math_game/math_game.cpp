#include <iostream>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <iomanip>
using namespace std;

// -------------------- Phase 1 : Members --------------------
enum enQuizLevel
{
    Easy = 1,
    Med,
    Hard,
    MixLevel
};

enum enOperationType
{
    Add = 1,
    Sub,
    Mul,
    Div,
    MixOp
};

struct stQuestion
{
    int num1 = 0;
    int num2 = 0;
    enOperationType opType;
    enQuizLevel level;
    int correctAnswer = 0;
    int playerAnswer = 0;
    bool answerResult = false;
};

struct stQuiz
{
    stQuestion questionList[100];
    int questionsNumber = 0;
    int rightAnswers = 0;
    int wrongAnswers = 0;
    enOperationType opType;
    enQuizLevel level;
    bool isPass = false;
};

// -------------------- Phase 2 : readers --------------------
enOperationType readOperationType()
{
    int opType = 0;
    cout << "\nEnter operation type [1] add, [2] sub, [3] mul, [4] div, [5] mix: ";
    cin >> opType;

    while (cin.fail() || opType < 1 || opType > 5)
    {
        if (cin.fail())
            cout << "Invalid input, tray again: ";
        else
            cout << "choice must be between (1-5): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> opType;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return static_cast<enOperationType>(opType);
}

enQuizLevel readQuizLevel()
{
    int level = 0;
    cout << "\nEnter quiz level [1] easy, [2] med, [3] hard, [4] mix: ";
    cin >> level;

    while (cin.fail() || level < 1 || level > 4)
    {
        if (cin.fail())
            cout << "Invalid input, tray again: ";
        else
            cout << "choice must be between (1-4): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> level;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return static_cast<enQuizLevel>(level);
}

int readQuestionsNumber()
{
    int number = 0;
    cout << "Enter questions number (1-100):  ";
    cin >> number;

    while (cin.fail() || number < 1 || number > 100)
    {
        if (cin.fail())
            cout << "Invalid input, try again: ";
        else
            cout << "Number must be between (1-100): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> number;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return number;
}

int readPlayerAnswer(const string &message = "")
{
    int number = 0;
    cout << message;
    cin >> number;

    while (cin.fail())
    {
        cout << "Invalid Input, try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> number;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return number;
}

// -------------------- Phase 3 : helpers --------------------
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

void setAnswerColor(bool isSuccess)
{
    if (isSuccess)
        cout << "\033[32m"; // Green
    else
        cout << "\033[31m"; // Red
}

void resetColor()
{
    cout << "\33[0m";
}

int randomNumber(int from, int to)
{
    return rand() % (to - from + 1) + from;
}

string getOperationTypeSymbol(enOperationType opType)
{
    const string opSymbols[] = {"+", "-", "*", "/", "Mix"};
    return opSymbols[opType - 1];
}

string getQuizLevelText(enQuizLevel quizlevel)
{
    const string quizLevelText[] = {"Easy", "med", "Hard", "Mix"};
    return quizLevelText[quizlevel - 1];
}

int simpleCalculator(int num1, int num2, enOperationType opType)
{
    switch (opType)
    {
    case Add:
        return num1 + num2;
    case Sub:
        return num1 - num2;
    case Mul:
        return num1 * num2;
    case Div:
        return (num2 != 0) ? num1 / num2 : 0;
    }
    return num1 + num2;
}

// -------------------- Phase 4 : questions --------------------

stQuestion generateQuestion(const stQuiz &quiz)
{
    stQuestion question;

    if (quiz.level == enQuizLevel::MixLevel)
        question.level = static_cast<enQuizLevel>(randomNumber(1, 3));
    else
        question.level = quiz.level;

    if (quiz.opType == enOperationType::MixOp)
        question.opType = static_cast<enOperationType>(randomNumber(1, 4));
    else
        question.opType = quiz.opType;

    switch (question.level)
    {
    case Easy:
        question.num1 = randomNumber(1, 10);
        question.num2 = randomNumber(1, 10);
        break;

    case Med:
        question.num1 = randomNumber(1, 50);
        question.num2 = randomNumber(1, 50);
        break;

    case Hard:
        question.num1 = randomNumber(1, 100);
        question.num2 = randomNumber(1, 100);
        break;
    }

    question.correctAnswer = simpleCalculator(question.num1, question.num2, question.opType);

    return question;
}

void printQuestionInfo(const stQuestion &question)
{
    cout << question.num1 << ' '
         << getOperationTypeSymbol(question.opType) << ' '
         << question.num2 << " = ";
}

void printQuestionResult(stQuiz &quiz, stQuestion &q)
{
    if (q.playerAnswer == q.correctAnswer)
    {
        q.answerResult = true;
        setAnswerColor(true);
        cout << "Right answer :)\n";
        quiz.rightAnswers++;
    }
    else
    {
        q.answerResult = false;
        setAnswerColor(false);
        cout << "Wrong answer :(\n";
        cout << "The right answer is: " << q.correctAnswer << endl;
        quiz.wrongAnswers++;
    }

    resetColor();
}

void generateQuizQuestions(stQuiz &quiz)
{
    for (int i = 0; i < quiz.questionsNumber; i++)
    {
        quiz.questionList[i] = generateQuestion(quiz);
    }
}

void startQuizQuestions(stQuiz &quiz)
{
    for (int i = 0; i < quiz.questionsNumber; i++)
    {
        cout << "\n\nQuestion [" << i + 1 << "/" << quiz.questionsNumber << "]\n\n";
        printQuestionInfo(quiz.questionList[i]);

        quiz.questionList[i].playerAnswer = readPlayerAnswer();

        printQuestionResult(quiz, quiz.questionList[i]);
    }

    quiz.isPass = (quiz.rightAnswers >= quiz.wrongAnswers);
}

// -------------------- Phase 5 : final --------------------
void displayFinalResultScreen()
{
    cout << "\n\n\t\t\t\t" << string(40, '=');
    cout << "\n\t\t\t\t" << "         F I N A L   R E S U L T";
    cout << "\n\t\t\t\t" << string(40, '=');
    cout << '\n';
}

string getFinalResult(bool isPass)
{
    if (isPass)
        return "PASS :)";
    else
        return "FAIL :(";
}

void displayQuizResult(const stQuiz &quiz)
{
    displayFinalResultScreen();

    setAnswerColor(quiz.isPass);
    cout << "\t\t\t\t\t\t "
         << getFinalResult(quiz.isPass) << '\n';
    resetColor();

    cout << "\t\t\t\t" << string(40, '-') << '\n';

    cout << "\t\t\t\t" << left << setw(22)
         << "Questions Number"
         << ": " << quiz.questionsNumber << '\n';

    cout << "\t\t\t\t" << setw(22)
         << "Question Level"
         << ": " << getQuizLevelText(quiz.level) << '\n';

    cout << "\t\t\t\t" << setw(22)
         << "Operation Type"
         << ": " << getOperationTypeSymbol(quiz.opType) << '\n';

    cout << "\t\t\t\t" << setw(22)
         << "Right Answers"
         << ": " << quiz.rightAnswers << '\n';

    cout << "\t\t\t\t" << setw(22)
         << "Wrong Answers"
         << ": " << quiz.wrongAnswers << '\n';

    cout << "\t\t\t\t" << string(40, '-') << '\n';
}

// -------------------- Phase 0 : start --------------------

void startQuiz()
{
    stQuiz quiz;

    quiz.questionsNumber = readQuestionsNumber();
    quiz.opType = readOperationType();
    quiz.level = readQuizLevel();

    generateQuizQuestions(quiz);
    startQuizQuestions(quiz);

    displayQuizResult(quiz);
}

int main()
{
    srand((unsigned)time(NULL));
    do
    {
        system("cls");
        startQuiz();
    } while (confirm("do you want to play again? (y/n): "));

    return 0;
}