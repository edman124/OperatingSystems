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
    int infd = 0;
    int outfd = 1;
    char** args = NULL;

    Command()
    {

    }
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

    // cout << "result 0: \'" << results[0] << "\' " << endl;
    if (results[0] == "exit") 
    {
        // cout << "exiting";
        cout << "Exit status: 0" << endl << "<";
        exit(0);
    }

    vector<int> redirInd;
    vector<int> pipeInd;
    vector<Command> commands;
    // vector<char*[]>commands;
    findOps(redirInd, pipeInd, results);
    // if (redirInd.size()>0)
    //     cout << redirInd[0] << endl;
    // cout << "redir size: " << redirInd.size() << endl;


    int count = 0;
    // streambuf *coutbuf, *cinbuf;
    // coutbuf = cout.rdbuf();
    int in = 0;
    int out = 1; 
    int incopy = dup(0);
    int outcopy = dup(1);
    if (redirInd.size()>2)
    {
        cerr << "malformed" << endl;
        return;
    }
    else if (redirInd.size()==2 && redirInd[0] == redirInd[1])
    {
        cerr << "malformed" << endl;
        return;
    }

    char** args = new char*[results.size() + 1 - 2*redirInd.size() - 2*pipeInd.size()];
    // Command commands[commandInd];
    commands.push_back(Command());
    int commandInd = 0;
    cout << commandInd << endl;
    for(size_t i=0; i < results.size(); i++)
    {
        if (find(redirInd.begin(), redirInd.end(), i) != redirInd.end())
        {
            if (i + 1 >= results.size() || results[i+1].compare("<") == 0 || results[i+1].compare(">") == 0 || results[i+1].compare("|") == 0)
            {
                cerr << "malformed" << endl;
                return;
            }
            if (results[i].compare("<") == 0)
            {
                in = open(results[i+1].c_str(), O_RDONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
                if (in<0)
                {
                    // perror("error");
                    cerr << "failed to open file";
                    return;
                }
                commands[commandInd].infd = in;
                dup2(in, 0);
                i++;
                // cin.rdbuf(in.rdbuf());
            }
            else if (results[i].compare(">") == 0)
            {
                // cout << "filename: " << results[i+1].c_str() << endl;
                // char buff[FILENAME_MAX];
                // char* ptr = getcwd(buff, FILENAME_MAX);
                // string currdir(buff);
                // cout << "file path: " << currdir << endl;
                // cout << "file path: " << ptr << endl;
                // string temp = currdir + "/" + results[i+1];
                // cout << "full path: " << temp << endl;
                out = open(results[i+1].c_str(), O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
                // cout << "file des: " << out << endl;
                if (out<0)
                {
                    // perror("error");
                    cerr << "failed to open file";
                    return;
                }
                commands[commandInd].outfd = out;
                dup2(out, 1);
                i++;
                // cout.rdbuf(out.rdbuf());
                // cout << "is it in file";
            }
        }
        else if (find(pipeInd.begin(), pipeInd.end(), i) != pipeInd.end())
        {
            args[count]=NULL;
            count=0;
            commands[commandInd].args=args;
            // delete[] args;
            char** args = new char*[results.size() + 1 - 2*redirInd.size() - 2*pipeInd.size()];
            args[count] = NULL;
            commandInd++;
            commands.push_back(Command());
            // cout << "found pipe" << endl;
            cout << args << endl;
        }
        else
        {
            args[count] = (char *)results[i].c_str();
            count++;
            // cout << "creating args" << endl;   
        }
    }
    // if (i==results.size()-1)
    // {
        args[count]=NULL;
        cout << "args length: " << args[0] << endl;
        commands[commandInd].args = args;
    // }
    if (count==0)
    {
        if (commands.size())
            commands.pop_back();
    }


    cout << "commands: " << endl;
    // args[0] = (char *)command.c_str();
    for (size_t i=0; i< commands.size(); i++)
    {
        for (size_t c=0; c<sizeof(commands[i].args)/sizeof(commands[i].args[0]);c++)
        {
            cout << commands[i].args[c] << endl;
        }
    }

    // args[results.size() - 2*redirInd.size() - 2*pipeInd.size()] = NULL;
    // Command c1;
    // c1.redirInd = redirInd;
    // c1.args = args; 
    // commands.push_back(c1);


    // dup2(in, 0);
    // dup2(out, 1);
    int prevfd = 0;
    for (size_t i = 0; i < commands.size(); i++)
    {
        int pipefd[2];
        // if (i!=0)
        // {
        //     if (pipe(pipeIn)== -1)
        //     {
        //         cerr << "pipe failed" << endl;
        //         exit(EXIT_FAILURE);
        //     }
        //     pipeIn = NULL;
        // }
        if (i<commands.size()-1)
        {
            if (pipe(pipefd)== -1)
            {
                cerr << "pipe failed" << endl;
                return;
            }
        }
        pid_t pid = fork();
        // cout << "see if cout stayed changed";
        if (pid == 0)
        {
            // cout << "in child" << endl;
            dup2(prevfd, 0);
            close(prevfd);
            if (i<commands.size()-1)
            {
                dup2(pipefd[1],1);
                close(pipefd[0]);
                close(pipefd[1]);
            }
            execvp(commands[i].args[0],commands[i].args);
            perror(commands[i].args[0]);
            break;
        }
        close(prevfd);
        prevfd = pipefd[0];
        close(pipefd[1]);
    }
    delete[] args;
    int status;
    for (size_t i=0; i < commands.size(); i++){
        wait(&status);
    }
    close(in);
    close(out);
    dup2(incopy, 0);
    dup2(outcopy, 1);
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
