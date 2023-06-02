#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <unistd.h> 
#include <sys/wait.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <sys/fcntl.h>
#include <stdlib.h>
using namespace std;
void d1tod2(string array1d, char**& array2d)
{
    int wordcount = 0;
    for (int i = 0; i < array1d[i] != '\0'; i++)
    {
        if (array1d[i] == ' ')
        {
            wordcount++;
        }
    }
    int index = 0;
    array2d = new char* [wordcount + 2];
    array2d[wordcount + 1] = NULL;
    int j = 0;
    int lengthof1d = (int)array1d.length() + 1;
    for (int i = 0; i < lengthof1d; i++)
    {
        int sizeof1d;
        for (sizeof1d = i; array1d[sizeof1d] != ' ' && array1d[sizeof1d] != '\0'; sizeof1d++);
        sizeof1d = sizeof1d - i + 1;
        if (sizeof1d > 1)
        {
            char* temp = new char[sizeof1d];
            while (true)
            {
                temp[j] = array1d[i];
                if (array1d[i] == ' ' || array1d[i] == '\0')
                {
                    temp[j] = '\0';
                    array2d[index] = new char[sizeof1d];
                    strcpy(array2d[index], temp);
                    delete[] temp;
                    index++;
                    j = 0;
                    break;
                }
                j++, i++;
            }
        }
    }
}
void d2tod3(char**& arry2d, char***& array3d)
{
    int size = 0;
    for (int i = 0; arry2d[i]; i++)
    {
        if (strcmp(arry2d[i], "|") == 0 || strcmp(arry2d[i], "<") == 0 || strcmp(arry2d[i], ">") == 0)
        {
            size++;
        }
    }
    size = size * 2 + 1;
    array3d = new char** [size + 2];

    array3d[size] = array3d[size + 1] = NULL;
    int index3d = 0;
    int index2d = 0;
    while (true)
    {
        int old = index2d;
        int tempsize2d = 1;
        for (index2d; arry2d[index2d]; index2d++)
        {
            if (strcmp(arry2d[index2d], "|") != 0 &&
                strcmp(arry2d[index2d], "<") != 0 &&
                strcmp(arry2d[index2d], ">") != 0)
            {
                tempsize2d++;
            }
            else
            {
                break;
            }
        }
        array3d[index3d] = new char* [tempsize2d];
        for (int i = 0; i < tempsize2d; i++)
        {
            array3d[index3d][i] = arry2d[i + old];
        }
        array3d[index3d][tempsize2d - 1] = NULL;
        char* temp = arry2d[index2d];
        if (temp == NULL)
        {
            break;
        }
        else
        {
            array3d[index3d + 1] = new char* [2];
            array3d[index3d + 1][0] = temp;
            array3d[index3d + 1][1] = NULL;
            index3d = index3d + 2;
            index2d++;
        }
    }
}
int main()
{
    string array1d;
    cout << "\n$   ";
    getline(cin, array1d);
    while (array1d != "exit")
    {
        char** array2d = NULL;
        d1tod2(array1d, array2d);
        char*** array3d = NULL;
        d2tod3(array2d, array3d);
        int index3d = 0;
        int readBackup = dup(0);
        int writeBackup = dup(1);
        while (array3d[index3d] != NULL)
        {
            int fd[2];
            fd[0] = -1;
            fd[1] = -1;
            if (array3d[index3d + 1])
            {
                if (strcmp(array3d[index3d + 1][0], "|") == 0 && array3d[index3d + 2])
                {
                    int status = pipe(fd);
                    if (status != 0)
                    {
                        cout << "Pipe Error";
                        return 1;
                    }
                    status = fork();
                    if (status == 0)
                    {
                        dup2(fd[1], 1);
                        if (execvp(array3d[index3d][0], array3d[index3d]) != 0)
                        {
                            cout << "command failed1\n";
                            exit(0);
                        }
                    }
                    else if (status > 0)
                    {
                        wait(NULL);
                        dup2(fd[0], 0);
                        close(fd[0]);
                        close(fd[1]);
                        index3d = index3d + 2;
                    }
                    else if (status < 0)
                    {
                        cout << "Fork Error\n";
                        return 1;
                    }
                }
                if (array3d[index3d + 2] && array3d[index3d + 1]) {
                    if (strcmp(array3d[index3d + 1][0], ">") == 0 && array3d[index3d + 2])
                    {
                        int status = fork();
                        if (status == 0)
                        {
                            fd[1] = open(array3d[index3d + 2][0], O_WRONLY | O_CREAT, 0777);
                            dup2(fd[1], 1);
                            if (execvp(array3d[index3d][0], array3d[index3d]) != 0)
                            {
                                cout << "command failed\n";
                                exit(0);
                            }
                        }
                        if (status > 0)
                        {
                            wait(NULL);
                            index3d += 3;
                        }
                        else if (status < 0)
                        {
                            cout << "Fork Error\n";
                            return 1;
                        }
                    }
                    else if (strcmp(array3d[index3d + 1][0], "<") == 0 && array3d[index3d + 2])
                    {
                        int status = fork();
                        if (status == 0)
                        {
                            fd[0] = open(array3d[index3d + 2][0], O_RDONLY, 0777);
                            dup2(fd[0], 0);
                            if (execvp(array3d[index3d][0], array3d[index3d]) != 0)
                            {
                                cout << "command failed\n";
                                exit(0);
                            }
                        }
                        if (status > 0)
                        {
                            wait(NULL);
                            index3d += 3;
                        }
                        else if (status < 0)
                        {
                            cout << "Fork Error\n";
                            return 1;
                        }
                    }
                }
            }
            if (array3d[index3d])
            {
                if (strcmp(array3d[index3d][0], "cd") != 0)
                {
                    int id = fork();
                    if (id == 0)
                    {
                        if (execvp(array3d[index3d][0], array3d[index3d]) != 0)
                        {
                            cout << "command failed2\n";
                            exit(0);
                        }
                    }
                    if (id > 0)
                    {
                        wait(NULL);
                    }
                    else
                    {
                        cout << "Fork failed" << endl;
                    }
                }
                else
                {
                    if (array3d[index3d][1])
                    {
                        if (chdir(array3d[index3d][1]) != 0)
                        {
                            cout << "command failed\n";
                        }
                    }
                }
                index3d = index3d + 2;
            }
        }
        dup2(readBackup, 0);
        dup2(writeBackup, 1);
        close(readBackup);
        close(writeBackup);
        for (int i = 0; array2d[i] != NULL; i++)
        {
            delete[] array2d[i];
        }
        delete[] array2d;
        for (int i = 0; array3d[i] != NULL; i++)
        {
            delete[] array3d[i];
        }
        delete[] array3d;
        cout << "\n$   ";
        array1d = "";
        getline(cin, array1d);
    }
    return 0;
}