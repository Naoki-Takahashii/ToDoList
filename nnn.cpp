#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

class ToDoList
{
public:

    struct Task
    {
        string description;
        string details;
        bool completed;
        string deadline;

    };
    ToDoList()
    {
        tasks = vector<Task>(); // tasks����̃x�N�^�[�ŏ���������
    }

    vector<Task> tasks;

    using Iterator = vector<Task>::iterator;

    void loadTasksFromFile(const string& filename)
    {
        ifstream file(filename);
        if (file.is_open())
        {
            Task task;
            while (getline(file, task.description) &&
                getline(file, task.details) &&
                file >> task.completed &&
                file.ignore() &&
                getline(file, task.deadline))
            {
                tasks.push_back(task);
            }
            file.close();
        }
    }

    void saveTasksToFile(const string& filename)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (const auto& task : tasks)
            {
                file << task.description << endl;
                file << task.details << endl;
                file << task.completed << endl;
                file << task.deadline << endl;
            }
            file.close();
        }
    }

    void addTask(const string& description, const string& deadline, const string& details = "")
    {
        Task newTask;
        newTask.description = description;
        newTask.details = details;
        newTask.completed = false;
        newTask.deadline = deadline;
        tasks.push_back(newTask);

        sortTasksByDeadline();
    }

    void removeTask(int index)
    {
        if (index >= 1 && index <= static_cast<int>(tasks.size()))
        {
            cout << "�{���Ƀ^�X�N���폜���܂����H (y/n): ";
            char choice;
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                Iterator it = tasks.begin() + index - 1;
                tasks.erase(it);
                cout << "�^�X�N���폜����܂����B" << endl;
            }
            else
            {
                cout << "�^�X�N�̍폜���L�����Z������܂����B" << endl;
            }
        }
        else
        {
            cout << "�����Ȕԍ��ł��B" << endl;
        }
    }

    void completeTask(int index)
    {
        if (index >= 1 && index <= static_cast<int>(tasks.size()))
        {
            Iterator it = tasks.begin() + index - 1;
            it->completed = true;
            cout << "�^�X�N���������܂����B" << endl;
        }
        else
        {
            cout << "�����Ȕԍ��ł��B" << endl;
        }
    }

    bool isEmpty() const
    {
        return tasks.empty();
    }

    void printTasks() const
    {
        cout << "ToDo���X�g:" << endl;
        int index = 1;
        for (auto it = tasks.begin(); it != tasks.end(); ++it)
        {
            const Task& task = *it;
            cout << index << ". ";
            if (task.completed)
                cout << "[����] ";
            cout << task.description << " (����: " << task.deadline << ")" << endl;
            cout << "   �ڍ�: " << task.details << endl;
            ++index;
        }
        cout << endl;
    }

    void editTask(int index)
    {
        if (index >= 1 && index <= static_cast<int>(tasks.size()))
        {
            Iterator it = tasks.begin() + index - 1;
            Task& task = *it;

            cout << "�^�X�N�̐V������������͂��Ă�������: ";
            cin.ignore();
            getline(cin, task.description);

            cout << "�^�X�N�̐V������������͂��Ă������� (��: 2023/07/31): ";
            cin >> task.deadline;

            cout << "�ڍׂ�ҏW���܂����H (y/n): ";
            char choice;
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                cout << "�^�X�N�̐V�����ڍׂ���͂��Ă�������: ";
                cin.ignore();
                getline(cin, task.details);
            }

            cout << "�^�X�N���ҏW����܂����B" << endl;
        }
        else
        {
            cout << "�����Ȕԍ��ł��B" << endl;
        }
    }

private:
    static bool compareByDeadline(const Task& task1, const Task& task2)
    {
        return task1.deadline < task2.deadline;
    }

    void sortTasksByDeadline()
    {
        sort(tasks.begin(), tasks.end(), compareByDeadline);
    }
};

int main()
{
    const string filename = "tasks.txt";
    ToDoList todo;

    todo.loadTasksFromFile(filename);

    while (true)
    {
        todo.printTasks();

        cout << "�����I�����Ă��������i���p�����j:" << endl;
        cout << "1. �^�X�N�̒ǉ�" << endl;
        cout << "2. �^�X�N�̍폜" << endl;
        cout << "3. �^�X�N�̊���" << endl;
        cout << "4. �^�X�N�̕ҏW" << endl;
        cout << "5. �I��" << endl;

        int choice;
        if (!(cin >> choice))
        {
            cout << "�����ȑI���ł��B" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1)
        {
            cout << "�^�X�N�̐�������͂��Ă�������: ";
            string description;
            cin.ignore();
            getline(cin, description);

            cout << "��������͂��Ă������� (��: 2023/07/31): ";
            string deadline;
            cin >> deadline;

            cout << "�ڍׂ���͂��܂����H (y/n): ";
            char choice;
            cin >> choice;

            string details;
            if (choice == 'y' || choice == 'Y')
            {
                cout << "�ڍׂ���͂��Ă�������: ";
                cin.ignore();
                getline(cin, details);
            }

            todo.addTask(description, deadline, details);

            cout << "�^�X�N���ǉ�����܂����B" << endl;
        }
        else if (choice == 2)
        {
            if (todo.isEmpty())
            {
                cout << "�폜����^�X�N������܂���B" << endl;
            }
            else
            {
                cout << "�폜����^�X�N�̔ԍ�����͂��Ă�������: ";
                int index;
                if (!(cin >> index))
                {
                    cout << "�����Ȕԍ��ł��B" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                todo.removeTask(index);
            }
        }
        else if (choice == 3)
        {
            if (todo.isEmpty())
            {
                cout << "��������^�X�N������܂���B" << endl;
            }
            else
            {
                cout << "��������^�X�N�̔ԍ�����͂��Ă�������: ";
                int index;
                if (!(cin >> index))
                {
                    cout << "�����Ȕԍ��ł��B" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                todo.completeTask(index);
            }
        }
        else if (choice == 4)
        {
            if (todo.isEmpty())
            {
                cout << "�ҏW����^�X�N������܂���B" << endl;
            }
            else
            {
                cout << "�ҏW����^�X�N�̔ԍ�����͂��Ă�������: ";
                int index;
                if (!(cin >> index))
                {
                    cout << "�����Ȕԍ��ł��B" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                todo.editTask(index);
            }
        }
        else if (choice == 5)
        {
            todo.saveTasksToFile(filename);
            break;
        }
        else
        {
            cout << "�����ȑI���ł��B" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << endl;
    }

    cout << "�v���O�������I�����܂��B" << endl;

    return 0;
}

