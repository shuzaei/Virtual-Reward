#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <map>
#include <vector>

#define DEBUG

namespace VRManager
{
    enum Color
    {
        bronze,
        silver,
        gold,
        platinum,
        none
    };

    std::ifstream historyIfstream;
    std::ofstream historyOfstream;
    class VRData;
    std::string ColorString(std::string string, Color color);
    void Print(std::ostream &ostream, std::string string);
    void PrintVRUsage(std::ostream &ostream);
    void PrintInitUsage(std::ostream &ostream);
    void Load(std::ostream &istream);
    void Store(std::ostream &ostream, std::vector<std::string> command);
    void LoadHistory();
    void StoreCommand(std::vector<std::string> command);
    bool Initialized();
    void PrintOpenManagerFailure(std::ostream &ostream);
    std::string Join(std::vector<std::string> stringVector, std::string joinString);
    std::vector<std::string> StringToCommand(std::string string);
    std::vector<std::string> ArgToCommand(int argc, char **argv);
    void Execute(std::vector<std::string> command, bool loadingHistory);
    void Test();

    class VRData
    {
    private:
        std::string name;
        long long property = 0;
        Color color = none;
        std::vector<std::string> title;
        std::map<std::string, long long> tasks;
        std::map<int, long long> colors;
        std::map<std::string, std::pair<int, long long>> titles;
        int taskCount = 0;

    public:
        void SetName(std::string newName)
        {
            this->name = newName;
        }
        std::string GetName()
        {
            return this->name;
        }
        void SetColor(Color newColor)
        {
            this->color = newColor;
        }
        Color GetColor()
        {
            return this->color;
        }
        void SetProperty(long long newProperty)
        {
            this->property = newProperty;
        }
        long long GetProperty()
        {
            return this->property;
        }
        void IncrementTaskCount()
        {
            this->taskCount++;
        }
        int GetTaskCount()
        {
            return this->taskCount;
        }
    } data;

    std::string colorEscapeSequence[5] = {
        "\e[30;48;5;166m",
        "\e[30;48;5;252m",
        "\e[30;48;5;227m",
        "\e[30;48;5;159m",
        "\e[0m",
    };
    std::string vrUsage = "Usage: vr <command>\n"
                          "Commands:\n"
                          "  init  Create user\n"
                          "  user  Manage user\n"
                          "  task  Manage tasks\n"
                          "  shop  Buy and sell items\n"
                          "  --help  Show usage\n";
    std::string initUsage = "Usage: vr init [subcommand] <username>\n"
                            "Discription:\n"
                            "  Create an user with name <username>.\n"
                            "Subcommand:\n"
                            "  --help  Show usage\n";
    std::string userUsage = "Usage: vr user <subcommand>\n"
                            "Subcommands:\n"
                            "  status  Show user details\n"
                            "  set     Set properties\n"
                            "  --help  Show usage\n";
    std::string toManyArgs = "\e[1;31mError:\e[m Too many args.\n";
    std::string alreadyInitialized = "\e[1;31mError:\e[m You've already initialized Virtual-Reward.\n";
    std::string notYetInitialized = "\e[1;31mError:\e[m You've not initialized Virtual-Reward yet. Do first `vr init` to initialize.\n";
    std::string openManagerFailure = "\e[1;31mfatal:\e[m VRManager::StoreCommand: std::ofstream::open: cannot open file ~/.vrhistory\n";
    std::string askReallyReset = "\e[1;31m! You're going to delete all history. Do you really want to reset?\e[0m (y/N) ";
    std::string historyPath = std::string(getenv("HOME")) + "/.vrhistory";

    std::string WIP = "Work in Progress.\n";

    std::string ColorString(std::string string, Color color)
    {
        return colorEscapeSequence[color] + string + "\e[0m";
    }

    void Print(std::ostream &ostream, std::string string)
    {
        ostream << string;
        std::flush(ostream);
    }

    void Load(std::istream &istream)
    {
        std::string buffer, command;
        while (std::getline(istream, buffer))
        {
            if (buffer == "")
            {
                Execute(StringToCommand(command), true);
                command = "";
            }
            else
            {
                command += buffer + "\n";
            }
        }
    }

    void Store(std::ostream &ostream, std::vector<std::string> command)
    {
        ostream << Join(command, "\n") << "\n"
                << std::endl;
    }

    void LoadHistory()
    {
        std::ios_base::openmode openmode = std::ios_base::in;
        historyIfstream.open(historyPath, openmode);
        if (bool(historyIfstream) == true)
        {
            Load((std::istream &)(historyIfstream));
            historyIfstream.close();
        }
    }

    void StoreCommand(std::vector<std::string> command)
    {
        std::ios_base::openmode openmode = std::ios_base::out | std::ios_base::app;
        historyOfstream.open(historyPath, openmode);
        if (bool(historyOfstream) == true)
        {
            Store((std::ostream &)(historyOfstream), command);
            historyOfstream.close();
        }
        else
        {
            Print(std::cerr, openManagerFailure);
            exit(1);
        }
    }

    bool Initialized()
    {
        std::ios_base::openmode openmode = std::ios_base::in;
        historyIfstream.open(historyPath, openmode);
        if (bool(historyIfstream) == true)
        {
            historyIfstream.close();
            return true;
        }
        return false;
    }

    std::string Join(std::vector<std::string> stringVector, std::string joinString)
    {
        std::string string;
        for (unsigned int i = 0; i < stringVector.size(); i++)
        {
            string += stringVector[i];
            if (i != stringVector.size() - 1)
            {
                string += joinString;
            }
        }
        return string;
    }

    std::vector<std::string> StringToCommand(std::string string)
    {
        std::string::iterator leftIterator = string.begin(), splitIterator, rightIterator = string.end();
        std::vector<std::string> command;

        while (leftIterator != string.end())
        {
            splitIterator = std::find(leftIterator, rightIterator, '\n');
            command.emplace_back(std::string(leftIterator, splitIterator));
            leftIterator = splitIterator + 1;
        }
        return command;
    }
    std::vector<std::string> ArgToCommand(int argc, char **argv)
    {
        std::vector<std::string> command;

        for (unsigned int i = 1; i < argc; i++)
        {
            command.emplace_back((std::string)argv[i]);
        }
        return command;
    }

    void Execute(std::vector<std::string> command, bool loadingHistory)
    {
        if (command.size() <= 0U)
        {
            Print(std::cerr, vrUsage);
            exit(1);
        }
        else
        {
#ifdef DEBUG
            if (command[0] == "test")
            {
                Test();
                exit(0);
            }
#endif
            if (command[0] == "init")
            {
                if (command.size() <= 1U)
                {
                    Print(std::cerr, initUsage);
                    exit(1);
                }
                if (command[1] == "--help")
                {
                    Print(std::cout, initUsage);
                    exit(0);
                }

                if (command.size() <= 2U)
                {
                    if (Initialized() == true && loadingHistory == false)
                    {
                        Print(std::cerr, alreadyInitialized);
                        exit(1);
                    }

                    data.SetName(command[1]);
                    if (loadingHistory == false)
                    {
                        StoreCommand(command);
                    }
                }
                else
                {
                    Print(std::cerr, toManyArgs);
                    Print(std::cout, initUsage);
                    exit(1);
                }
            }
            else if (command[0] == "user")
            {
                if (command.size() <= 1U)
                {
                    Print(std::cerr, userUsage);
                    exit(1);
                }
                if (command[1] == "--help")
                {
                    Print(std::cout, userUsage);
                    exit(0);
                }
                if (Initialized() == false)
                {
                    Print(std::cerr, notYetInitialized);
                    exit(1);
                }

                if (command[1] == "status")
                {
                    std::string userName = "Username: " + ColorString(" " + data.GetName() + " ", data.GetColor()) + "\n";
                    std::string property = "Property: " + std::to_string(data.GetProperty()) + "r\n";
                    std::string taskCount = "Tasks:    " + std::to_string(data.GetTaskCount()) + " task(s) completed\n";
                    Print(std::cout, userName + property + taskCount);
                    exit(0);
                }
                else if (command[1] == "set")
                {
                    Print(std::cout, WIP);
                    exit(0);
                }
                else
                {
                    Print(std::cerr, initUsage);
                    exit(1);
                }
            }
            else if (command[0] == "task")
            {
                if (Initialized() == false)
                {
                    Print(std::cerr, notYetInitialized);
                    exit(1);
                }
                Print(std::cout, WIP);
                exit(0);
            }
            else if (command[0] == "shop")
            {
                if (Initialized() == false)
                {
                    Print(std::cerr, notYetInitialized);
                    exit(1);
                }
                Print(std::cout, WIP);
                exit(0);
            }
            else if (command[0] == "--help")
            {
                Print(std::cout, vrUsage);
                exit(0);
            }
            else if (command[0] == "reset")
            {
                if (Initialized() == false)
                {
                    Print(std::cerr, notYetInitialized);
                    exit(1);
                }
                Print(std::cout, askReallyReset);

                while (true)
                {
                    std::string answer;

                    if (bool(std::getline(std::cin, answer)) == false || answer == "" || answer == "n" || answer == "N")
                    {
                        Print(std::cout, "No\n");
                        break;
                    }
                    else if (answer == "y" || answer == "Y")
                    {
                        Print(std::cout, "Yes\nCleaning...\n");
                        std::filesystem::remove(historyPath);
                        Print(std::cout, "Clean completed.\n");
                        break;
                    }
                }
            }
            else
            {
                Print(std::cerr, vrUsage);
                exit(1);
            }
        }
    }

    void Test()
    {
        using namespace std;
        Print(cout, "Username: " + ColorString(" " + data.GetName() + " ", bronze) + "\n");
        Print(cout, "Username: " + ColorString(" " + data.GetName() + " ", silver) + "\n");
        Print(cout, "Username: " + ColorString(" " + data.GetName() + " ", gold) + "\n");
        Print(cout, "Username: " + ColorString(" " + data.GetName() + " ", platinum) + "\n");
    }
};

namespace VR
{
    void Main(int argc, char **argv)
    {
        VRManager::LoadHistory();
        VRManager::Execute(VRManager::ArgToCommand(argc, argv), false);
    }
};

int main(int argc, char **argv)
{
    VR::Main(argc, argv);
}
