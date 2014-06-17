/*
Copyright (c) 2014 kimmoli kimmo.lindholm@gmail.com @likimmo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <QCoreApplication>
#include "stm32p.h"
#include "i2ctester.h"
#include <QDebug>
#include <QString>
#include <QProcessEnvironment>


int main(int argc, char *argv[])
{
    bool fileNameGiven = false;
    bool rootUser = false;

    QStringList environment = QProcessEnvironment::systemEnvironment().toStringList();
    for (int n=0; n<environment.length(); n++)
        if (environment.at(n) == "USER=root")
        {
            rootUser = true;
            break;
        }

    if (!rootUser)
    {
        printf("Error: You need to be root to use this utility!\n");
        return 0;
    }

    if (argc < 2)
    {
        printf("stm32-programmer version " APPVERSION " (C) kimmoli 2014\n\n");
        printf("Usage:\n");
        printf("stm32-programmer {-p filename} {options...}\n\n");
        printf(" -p filename    program hex file\n");
        printf(" -s             reset and start after programming\n");
        printf(" -r addr count  test reading from i2c address (hex)\n");
        printf(" -w addr data   test write one data byte to i2c addres (hex)\n");
        printf(" -o             shutdown\n");
        return 0;
    }

    Stm32p* stm32 = new Stm32p();

    for (int i=1; i<argc; i++)
    {
        if (QString(argv[i]).left(2) == "-p")
        {
            if (!stm32->filenameSet(QString(argv[i+1])))
            {
                printf("File %s not found\n", argv[i+1]);
                delete stm32;
                return 0;
            }
            else
            {
                fileNameGiven = true;
            }
        }
        else if (QString(argv[i]).left(2) == "-s")
        {
            stm32->startAfterProgramming = true;
        }
        else if (QString(argv[i]).left(2) == "-r")
        {
            bool b;

            printf("Reading...\n");

            i2cTester* i2cTest = new i2cTester(QString(argv[i+1]).toInt(&b, 16));

            i2cTest->testRead(QString(argv[i+2]).toInt());

            delete i2cTest;

            /* Keep alive */
            stm32->startAfterProgramming = true;
        }
        else if (QString(argv[i]).left(2) == "-w")
        {
            bool b;

            printf("Writing...\n");

            i2cTester* i2cTest = new i2cTester(QString(argv[i+1]).toInt(&b, 16));

            i2cTest->testWrite(QString(argv[i+2]).toInt(&b, 16));

            delete i2cTest;

            /* Keep alive */
            stm32->startAfterProgramming = true;
        }

        else if (QString(argv[i]).left(2) == "-o")
        {
            stm32->vddStateSet(false);
        }
    }

    if (fileNameGiven)
        stm32->startProgram();

    delete stm32;
    return 1;
}

