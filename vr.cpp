#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <map>
#include <vector>

namespace VRManager
{
    enum Color
    {
        bronze,
        silver,
        gold,
        platinum
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
    void PrintOpenManagerFailure(std::ostream &ostream);
    std::string Join(std::vector<std::string> stringVector, std::string joinString);
    std::vector<std::string> StringToCommand(std::string string);
    std::vector<std::string> ArgToCommand(int argc, char **argv);
    void Execute(std::vector<std::string> command, bool loadingHistory);

    std::string colorEscapeSequence[4] = {
        "\e[37;41m",
        "\e[30;47m",
        "\e[37;43m",
        "\e[37;46m"};
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
    std::string toManyArgs = "too many args\n";
    std::string alreadyInitialized = "You've already initialized Virtual-Reward.\n";
    std::string openManagerFailure = "\e[1;31mfatal:\e[m VRManager::StoreCommand: std::ofstream::open: cannot open file ~/.vrhistory\n";
    std::string askReallyReset = "\e[1;31m? You're going to delete all history. Do you really want to reset?\e[0m (y/N) ";
    std::string historyPath = std::string(getenv("HOME")) + "/.vrhistory";

    std::string WIP = "Work in Progress.";

    class VRData
    {
    private:
        std::string name;
        long long r;
        std::map<std::string, long long> tasks;
        std::map<int, long long> colors;
        std::map<std::string, std::pair<int, long long>> titles;

    public:
        std::string SetName(std::string newName)
        {
            std::string oldName = this->name;
            this->name = newName;
            return oldName;
        }
        std::string GetName()
        {
            return this->name;
        }
    } data;

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
        ostream << Join(command, "\n") << std::endl;
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

    std::string Join(std::vector<std::string> stringVector, std::string joinString)
    {
        std::string string;
        for (unsigned int i = 1; i < stringVector.size(); i++)
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

        for (unsigned int i = 0; i < argc; i++)
        {
            command.emplace_back((std::string)argv[i]);
        }
        return command;
    }

    void Execute(std::vector<std::string> command, bool loadingHistory)
    {
        if (command.size() <= 1U)
        {
            Print(std::cerr, vrUsage);
            exit(1);
        }
        else
        {
            if (command[1] == "init")
            {
                if (command.size() <= 2U)
                {
                    Print(std::cerr, initUsage);
                    exit(1);
                }
                else if (command[2] == "--help")
                {
                    Print(std::cout, initUsage);
                    exit(0);
                }
                else if (command.size() <= 3U)
                {
                    std::ios_base::openmode openmode = std::ios_base::in;
                    historyIfstream.open(historyPath, openmode);
                    if (bool(historyIfstream) == true)
                    {
                        historyIfstream.close();
                        Print(std::cerr, alreadyInitialized);
                        exit(1);
                    }

                    data.SetName(command[2]);
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
            else if (command[1] == "user")
            {
                if (command.size() <= 2U)
                {
                    Print(std::cerr, userUsage);
                    exit(1);
                }
                else if (command[2] == "status")
                {
                    Print(std::cout, WIP);
                    exit(0);
                }
                else if (command[2] == "set")
                {
                    Print(std::cout, WIP);
                    exit(0);
                }
                else if (command[2] == "--help")
                {
                    Print(std::cout, userUsage);
                    exit(0);
                }
                else
                {
                    Print(std::cerr, initUsage);
                    exit(1);
                }
            }
            else if (command[1] == "task")
            {
                Print(std::cout, WIP);
                exit(0);
            }
            else if (command[1] == "shop")
            {
                Print(std::cout, WIP);
                exit(0);
            }
            else if (command[1] == "--help")
            {
                Print(std::cout, vrUsage);
                exit(0);
            }
            else if (command[1] == "reset")
            {
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