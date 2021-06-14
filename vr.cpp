#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace VRManager
{
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

    std::fstream manager;

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

    void Register(std::ostream &ostream, std::vector<std::string> command)
    {
        std::string log;
        for (int i = 1; i < (int)command.size(); i++)
        {
            log += command[i];
            if (i != (int)command.size() - 1)
            {
                log += ' ';
            }
        }
        ostream << log << std::endl;
    }

    // update
    void PrintVRUsage(std::ostream &ostream)
    {
        static std::string usage = "Usage: vr <command>\n"
                                   "Commands:\n";
        ostream << usage;
    }

    void PrintOpenManagerFailure(std::ostream &ostream)
    {
        static std::string failure = "VRManager::OpenManager: std::fstream&::open: cannot open file ~/.vrmanager\n";
        ostream << failure;
    }

    void OpenManager()
    {
        std::ios_base::openmode openmode = std::ios_base::in | std::ios_base::out | std::ios_base::app;
        manager.open("~/.vrmanager", openmode);
        if ((bool)manager == false)
        {
            openmode = std::ios_base::out;
            manager.open("~/.vrmanager", openmode);
            if ((bool)manager == false)
            {
                VRManager::PrintOpenManagerFailure(std::cerr);
                exit(1);
            }
        }
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
        }
    }
};

namespace VR
{
    void Main(int argc, char **argv)
    {
        VRManager::OpenManager();
        std::string command;
        while (std::getline(VRManager::manager, command))
        {
            VRManager::Execute(VRManager::StringToCommand(command));
        }
        VRManager::Execute(VRManager::ArgToCommand(argc, argv));
    }
};

int main(int argc, char **argv)
{
    VR::Main(argc, argv);
}