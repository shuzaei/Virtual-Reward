#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace VRManager
{

    std::ifstream historyIfstream;
    std::ofstream historyOfstream;
    class Data;
    std::string Bronze(std::string string);
    std::string Silver(std::string string);
    std::string Gold(std::string string);
    std::string Platinum(std::string string);
    void PrintVRUsage(std::ostream &ostream);
    void Load(std::ostream &istream);
    void Store(std::ostream &ostream, std::vector<std::string> command);
    void LoadHistory();
    void StoreCommand(std::vector<std::string> command);
    void PrintOpenManagerFailure(std::ostream &ostream);
    std::vector<std::string> StringToCommand(std::string string);
    std::vector<std::string> ArgToCommand(int argc, char **argv);
    void Execute(std::vector<std::string> command);

    class Data
    {
    private:
        std::string name;

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
    };

    std::string Bronze(std::string string)
    {
        return "\e[37;41m" + string + "\e[m";
    }

    std::string Silver(std::string string)
    {
        return "\e[30;47m" + string + "\e[m";
    }

    std::string Gold(std::string string)
    {
        return "\e[37;43m" + string + "\e[m";
    }

    std::string Platinum(std::string string)
    {
        return "\e[37;46m" + string + "\e[m";
    }

    void PrintVRUsage(std::ostream &ostream)
    {
        static std::string usage = "Usage: vr <command>\n"
                                   "Commands:\n"
                                   "  init  Create user\n"
                                   "  user  Manage user\n"
                                   "  task  Manage tasks\n"
                                   "  shop  Buy, sell items\n"
                                   "  log   View logs\n";
        ostream << usage;
    }

    void Load(std::istream &istream)
    {
        std::string command;
        while (std::getline(istream, command))
        {
            Execute(StringToCommand(command));
        }
    }

    void Store(std::ostream &ostream, std::vector<std::string> command)
    {
        std::string string;
        for (int i = 1; i < (int)command.size(); i++)
        {
            string += command[i];
            if (i != (int)command.size() - 1)
            {
                string += ' ';
            }
        }
        ostream << string << std::endl;
    }

    void LoadHistory()
    {
        std::ios_base::openmode openmode = std::ios_base::in;
        historyIfstream.open("~/.vrmanager", openmode);
        if ((bool)historyIfstream == true)
        {
            Load(historyIfstream);
            historyIfstream.close();
        }
    }

    void StoreCommand(std::vector<std::string> command)
    {
        std::ios_base::openmode openmode = std::ios_base::out | std::ios_base::app;
        historyOfstream.open("~/.vrmanager", openmode);
        if ((bool)historyOfstream == true)
        {
            Store(historyOfstream, command);
            historyOfstream.close();
        }
        else
        {
            VRManager::PrintOpenManagerFailure(std::cerr);
            exit(1);
        }
    }

    void PrintOpenManagerFailure(std::ostream &ostream)
    {
        static std::string failure = "\e[31mfatal:\e[m VRManager::OpenManager: std::ofstream&::open: cannot open file ~/.vrmanager\n";
        ostream << failure;
    }

    std::vector<std::string> StringToCommand(std::string string)
    {
        std::string::iterator leftIterator = string.begin(), rightIterator = string.end();
        std::vector<std::string> command;

        while (leftIterator != string.end())
        {
            command.emplace_back(std::string(leftIterator, std::find(leftIterator, rightIterator, ' ')));
        }
        return command;
    }
    std::vector<std::string> ArgToCommand(int argc, char **argv)
    {
        std::vector<std::string> command;

        for (int i = 0; i < argc; i++)
        {
            command.emplace_back((std::string)argv[i]);
        }
        return command;
    }

    void Execute(std::vector<std::string> command)
    {
        if (command.size() <= 1)
        {
            PrintVRUsage(std::cerr);
            exit(1);
        }
        else
        {
            if (command[1] == "init")
            {
            }
            else if (command[1] == "user")
            {
            }
            else if (command[1] == "task")
            {
            }
            else if (command[1] == "shop")
            {
            }
            else if (command[1] == "log")
            {
            }
            else
            {
                PrintVRUsage(std::cerr);
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
        VRManager::Execute(VRManager::ArgToCommand(argc, argv));
    }
};

int main(int argc, char **argv)
{
    VR::Main(argc, argv);
}