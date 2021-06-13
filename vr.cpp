#include <iostream>
#include <fstream>
#include <string>

namespace VR
{
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

    void Register(int argc, char **argv, std::ostream &ostream)
    {
        std::string log;
        for (int i = 1; i < argc; i++)
        {
            log += (std::string)argv[i];
            if (i != argc - 1)
            {
                log += ' ';
            }
        }
        ostream << log << std::endl;
    }

    void VRUsage(std::ostream &ostream, std::string argv0)
    {
        std::string usage = "Usage: " + argv0 + " <command>\n"
                                                "Commands:\n";
        ostream << usage;
    }

    bool ManagerOpen()
    {
        std::ios_base::openmode openmode = std::ios_base::in | std::ios_base::out | std::ios_base::app;
        manager.open("~/.vrmanager", openmode);
        return (bool)manager;
    }

    void ParseCommand(int argc, char **argv)
    {
        if (argc <= 1)
        {
            VRUsage(std::cerr, argv[0]);
            exit(1);
        }
        else
        {
            std::cout << "Work in Progress." << std::endl;
        }
    }
};

int main(int argc, char **argv)
{
    VR::ParseCommand(argc, argv);
}