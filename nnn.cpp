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
        tasks = vector<Task>(); // tasksを空のベクターで初期化する
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
            cout << "本当にタスクを削除しますか？ (y/n): ";
            char choice;
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                Iterator it = tasks.begin() + index - 1;
                tasks.erase(it);
                cout << "タスクが削除されました。" << endl;
            }
            else
            {
                cout << "タスクの削除がキャンセルされました。" << endl;
            }
        }
        else
        {
            cout << "無効な番号です。" << endl;
        }
    }

    void completeTask(int index)
    {
        if (index >= 1 && index <= static_cast<int>(tasks.size()))
        {
            Iterator it = tasks.begin() + index - 1;
            it->completed = true;
            cout << "タスクが完了しました。" << endl;
        }
        else
        {
            cout << "無効な番号です。" << endl;
        }
    }

    bool isEmpty() const
    {
        return tasks.empty();
    }

    void printTasks() const
    {
        cout << "ToDoリスト:" << endl;
        int index = 1;
        for (auto it = tasks.begin(); it != tasks.end(); ++it)
        {
            const Task& task = *it;
            cout << index << ". ";
            if (task.completed)
                cout << "[完了] ";
            cout << task.description << " (期限: " << task.deadline << ")" << endl;
            cout << "   詳細: " << task.details << endl;
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

            cout << "タスクの新しい説明を入力してください: ";
            cin.ignore();
            getline(cin, task.description);

            cout << "タスクの新しい期限を入力してください (例: 2023/07/31): ";
            cin >> task.deadline;

            cout << "詳細を編集しますか？ (y/n): ";
            char choice;
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                cout << "タスクの新しい詳細を入力してください: ";
                cin.ignore();
                getline(cin, task.details);
            }

            cout << "タスクが編集されました。" << endl;
        }
        else
        {
            cout << "無効な番号です。" << endl;
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

        cout << "操作を選択してください（半角数字）:" << endl;
        cout << "1. タスクの追加" << endl;
        cout << "2. タスクの削除" << endl;
        cout << "3. タスクの完了" << endl;
        cout << "4. タスクの編集" << endl;
        cout << "5. 終了" << endl;

        int choice;
        if (!(cin >> choice))
        {
            cout << "無効な選択です。" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1)
        {
            cout << "タスクの説明を入力してください: ";
            string description;
            cin.ignore();
            getline(cin, description);

            cout << "期限を入力してください (例: 2023/07/31): ";
            string deadline;
            cin >> deadline;

            cout << "詳細を入力しますか？ (y/n): ";
            char choice;
            cin >> choice;

            string details;
            if (choice == 'y' || choice == 'Y')
            {
                cout << "詳細を入力してください: ";
                cin.ignore();
                getline(cin, details);
            }

            todo.addTask(description, deadline, details);

            cout << "タスクが追加されました。" << endl;
        }
        else if (choice == 2)
        {
            if (todo.isEmpty())
            {
                cout << "削除するタスクがありません。" << endl;
            }
            else
            {
                cout << "削除するタスクの番号を入力してください: ";
                int index;
                if (!(cin >> index))
                {
                    cout << "無効な番号です。" << endl;
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
                cout << "完了するタスクがありません。" << endl;
            }
            else
            {
                cout << "完了するタスクの番号を入力してください: ";
                int index;
                if (!(cin >> index))
                {
                    cout << "無効な番号です。" << endl;
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
                cout << "編集するタスクがありません。" << endl;
            }
            else
            {
                cout << "編集するタスクの番号を入力してください: ";
                int index;
                if (!(cin >> index))
                {
                    cout << "無効な番号です。" << endl;
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
            cout << "無効な選択です。" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << endl;
    }

    cout << "プログラムを終了します。" << endl;

    return 0;
}

