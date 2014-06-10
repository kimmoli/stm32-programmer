/*
Copyright (c) 2014 kimmoli kimmo.lindholm@gmail.com @likimmo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stm32p.h"
#include "stm32driver.h"
#include <QSettings>
#include <QCoreApplication>
#include <QThread>
#include <QDebug>

#define GPIO_INT "67"

Stm32p::Stm32p(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();
}

QString Stm32p::readVersion()
{
    return APPVERSION;
}

Stm32p::~Stm32p()
{
}

/*
 * Function to log error on console, and make it available also on QML side
 */
void Stm32p::generateErrorMsg(QString msg)
{
    qCritical() << msg;
    _lastError = msg;
    emit errorMsgChanged();
}

/*
 * Function helper to set status message and progress bar value at once
 */
void Stm32p::setStatus(QString status, int progress)
{
    _status = status;
    _progress = progress;
    emit statusMsgChanged();
    emit progressChanged();
}

/*
 * Function to control TOH Vdd state
 */
void Stm32p::vddStateSet(bool state)
{
    qDebug() << "changing vdd state to" << state;

    int fd = open("/sys/devices/platform/reg-userspace-consumer.0/state", O_WRONLY);

    if (!(fd < 0))
    {
        if (write (fd, state ? "1" : "0", 1) != 1)
            generateErrorMsg("Failed to control VDD.");

        close(fd);
    }

    QThread::msleep(100);

    _vddState = state;
    emit vddStateChanged();
}

/*
 * Function to control state of GPIO pin
 */
void Stm32p::gpioStateSet(bool state)
{
    qDebug() << "changing GPIO state to" << state;

    int fd = open("/sys/class/gpio/export", O_WRONLY);

    if (!(fd < 0))
    {
        if (write (fd, GPIO_INT, strlen(GPIO_INT)) != strlen(GPIO_INT))
            generateErrorMsg("Failed to export GPIO");

        close(fd);
    }

    fd = open("/sys/class/gpio/gpio" GPIO_INT "/value", O_WRONLY);

    if (!(fd < 0))
    {
        if (write (fd, state ? "1" : "0", 1) != 1)
            generateErrorMsg("Failed to set GPIO state");

        close(fd);
    }
}

/*
 * Frontend of STM programmer
 */
void Stm32p::startProgram()
{
    setStatus("Programming", 10);
    generateErrorMsg("Not programming yet");
}

/*
 * Frontend of STM verifier (TBD)
 */
void Stm32p::startVerify()
{
    setStatus("Verifying", 50);
    generateErrorMsg("Not verifying yet");
}
