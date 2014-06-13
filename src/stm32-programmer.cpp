/*
Copyright (c) 2014 kimmoli kimmo.lindholm@gmail.com @likimmo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <QCoreApplication>
#include "stm32p.h"
#include <QDebug>
#include <QString>


int main(int argc, char *argv[])
{
    printf("stm32-programmer version " APPVERSION " (C) kimmoli 2014\n\n");

    if (argc < 2)
    {
        printf("Usage:\n");
        printf("stm32-programmer [filename] {options...}\n\n");
        printf(" no options yet\n");
        return 0;
    }

    Stm32p* stm32 = new Stm32p();

    if (!stm32->filenameSet(QString(argv[1])))
    {
        printf("File not found\n");
        delete stm32;
        return 0;
    }

    stm32->startProgram();

    delete stm32;
    return 1;
}

