#include <cstdlib>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <iterator>
#include <wait.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

class Command
{
    public:
    // vector<int> redirInd;
    int infd = -1;
    int outfd = -1;
    char** args = NULL;

    Command(size_t length)
    {
        if (length>0)
        {
            args = new char*[length]; 
        }
        else
        {
            exit(1);
        }
    }
    // ~Command()
    // {
    //     delete []args;
    // }
};

void findOps(vector<int>& redirInd, vector<int>& pipeInd, vector<string>& results)
{
    for(size_t i = 0; i < results.size(); i++)
    {
        if (results[i].length() == 1)
        {
            // cout << "a char" << endl;
            if (results[i].compare(">") == 0 || results[i].compare("<") == 0)
            {
                // cout << "equal" << endl;
                redirInd.push_back(i);
            }
            if (results[i].compare("|") == 0)
            {
                // cout << "equal" << endl;
                pipeInd.push_back(i);
            }
        }
    }
}

void parse_and_run_command(const std::string &command) {
    /* TODO: Implement this. */
    /* Note that this is not the correct way to test for the exit command.
       For example the command "   exit  " should also exit your shell.
     */
    istringstream iss(command);
    vector<string> results((istream_iterator<string>(iss)), istream_iterator<string>());
    if (results.size()==0)
    {

        return;
    }

    if (results[0] == "exit") 
    {
        exit(0);
    }

    vector<int> redirInd;
    vector<int> pipeInd;
    vector<Command> commands;
    findOps(redirInd, pipeInd, results);

    int count = 0;
    int in = dup(0);
    int out = dup(1); 
    int incopy = dup(0);
    int outcopy = dup(1);
    if (results.size() <= 2*redirInd.size() + pipeInd.size())
    {
        cerr << "invalid command" << endl;
        return;
    }

    // char** args = new char*[results.size() + 1 - 2*redirInd.size() - 2*pipeInd.size()];
    // Command commands[commandInd];
    commands.push_back(Command(results.size() + 1 - 2*redirInd.size() - pipeInd.size()));
    int commandInd = 0;
    int redirLeftCount = 0;
    int redirRightCount = 0;
    // cout << commandInd << endl;
    for(size_t i=0; i < results.size(); i++)
    {
        if (find(redirInd.begin(), redirInd.end(), i) != redirInd.end())
        {
            if (i + 1 >= results.size() || string(results[i+1]).compare("<") == 0 || results[i+1].compare(">") == 0 || results[i+1].compare("|") == 0)
            {
                cerr << "invalid command" << endl;
                close(in);
                close(out);
                close(incopy);
                close(outcopy);
                return;
            }
            if (results[i].compare("<") == 0)
            {
                in = open(results[i+1].c_str(), O_RDONLY, 0666);//S_IRWXU | S_IRWXG | S_IRWXO);
                if (in<0)
                {
                    // perror("error");
                    close(in);
                    close(out);
                    close(incopy);
                    close(outcopy);
                    cerr << "failed to open file" << endl;
                    return;
                }
                redirLeftCount += 1;
                commands[commandInd].infd = in;
                // dup2(in, 0);
                // close(in);
                i++;
            }
            else if (results[i].compare(">") == 0)
            {
                out = open(results[i+1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);// S_IRWXU | S_IRWXG | S_IRWXO);
                // cout << "file des: " << out << endl;
                if (out<0)
                {
                    // perror("error");
                    close(in);
                    close(out);
                    close(incopy);
                    close(outcopy);
                    cerr << "failed to open file" << endl;
                    return;
                }
                redirRightCount += 1;
                commands[commandInd].outfd = out;
                // dup2(out, 1);
                i++;
            }
        }
        else if (find(pipeInd.begin(), pipeInd.end(), i) != pipeInd.end())
        {
            if (i+1 >= results.size() || string(results[i+1]).compare("<") == 0 || results[i+1].compare(">") == 0 || results[i+1].compare("|") == 0)
            {
                cerr << "invalid command" << endl;
                close(in);
                close(out);
                close(incopy);
                close(outcopy);
                return;
            }
            commands[commandInd].args[count]=NULL;
            count=0;
            if (redirRightCount > 1 || redirRightCount > 1)
            {
                cerr << "invalid command" << endl;
                close(in);
                close(out);
                close(incopy);
                close(outcopy);
                return;
            }
            commandInd++;
            commands.push_back(Command(results.size() + 1 - 2*redirInd.size() - pipeInd.size()));
        }
        else
        {
            commands[commandInd].args[count] = (char *)results[i].c_str();
            count++;
        }
    }
    commands[commandInd].args[count]=NULL;
    if (count==0)
    {
        if (commands.size())
            commands.pop_back();
    }

    int prevfd = in;
    vector<pid_t> pidList;
    for (size_t i = 0; i < commands.size(); i++)
    {
        int pipefd[2];
        if (i<commands.size()-1)
        {
            if (pipe(pipefd)== -1)
            {
                cerr << "pipe failed" << endl;
                close(in);
                close(out);
                close(incopy);
                close(outcopy);
                return;
            }
        }
        pid_t pid = fork();
        // cout << "see if cout stayed changed";
        if (pid == -1)
        {
            cerr << "for failed" << endl;
        }
        else if (pid == 0)
        {
            // cout << "in child" << endl;
            for (size_t j = 0; j<commands.size(); j++)
            {
                if (i != j)
                {
                    
                    close(commands[j].infd);
                    close(commands[j].outfd);
                    delete[] commands[j].args;
                }
            }
            dup2(prevfd, 0);
            close(prevfd);
            if (i<commands.size()-1)
            {
                dup2(pipefd[1],1);
                close(pipefd[0]);
                close(pipefd[1]);
            }
            else
            {
                dup2(out, 1);
                close(out);
            }
            if (commands[i].infd > -1)
            {
                dup2(commands[i].infd, 0);
                close(commands[i].infd);
            }
            if (commands[i].outfd > -1)
            {
                dup2(commands[i].outfd, 1);
                close(commands[i].outfd);
            }
            close(in);
            close(out);
            close(prevfd);
            close(incopy);
            close(outcopy);
            execv(commands[i].args[0], commands[i].args);
            perror(commands[i].args[0]);
            for (size_t j = 0; j<commands.size();j++)
            {
                delete[] commands[j].args;
            }
            exit(0);
        }
        else
        {
            pidList.push_back(pid);
        }
        // close(prevfd);
        prevfd = pipefd[0];
        close(pipefd[1]);
    }
    int status;
    for (size_t i=0; i < pidList.size(); i++){
        waitpid(pidList[i], &status, 0);
        dup2(incopy, 0);
        dup2(outcopy, 1);
        if (status<0)
        {
            cerr << "waitpid failed" << endl;
        }
        else
        {
            cout << commands[i].args[0] << " Exit status: " << WEXITSTATUS(status) << endl;
        }
        if (WIFSIGNALED(status))
        {
            cout << commands[i].args[0] << " ended by signal" << endl;
        }
        // delete[] commands[i].args;
    }
    for (size_t i=0; i<commands.size(); i++)
    {
        delete[] commands[i].args;
    }
    for (size_t j = 0; j<commands.size(); j++)
    {
            
            close(commands[j].infd);
            close(commands[j].outfd);
    }
    dup2(incopy, 0);
    dup2(outcopy, 1);
    close(in);
    close(out);
    close(prevfd);
    close(incopy);
    close(outcopy);
    // cerr << "Not implemented.\n";
}

int main(void) {
    string command;
    cout << "> ";
    while (getline(cin, command)) {
        parse_and_run_command(command);
        cout << "> ";
    }
    return 0;
}
