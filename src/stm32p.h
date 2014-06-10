/*
Copyright (c) 2014 kimmoli kimmo.lindholm@gmail.com @likimmo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef STM32P_H
#define STM32P_H
#include <QObject>

class Stm32p : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ readVersion NOTIFY versionChanged())
    Q_PROPERTY(bool vddState READ vddStateGet WRITE vddStateSet NOTIFY vddStateChanged)
    Q_PROPERTY(QString errorMsg READ errorMsgGet NOTIFY errorMsgChanged)
    Q_PROPERTY(QString statusMsg READ statusMsgGet NOTIFY statusMsgChanged)
    Q_PROPERTY(int progress READ progressGet NOTIFY progressChanged)

public:
    explicit Stm32p(QObject *parent = 0);
    ~Stm32p();

    QString readVersion();
    bool vddStateGet() { return _vddState; }
    void vddStateSet(bool state);

    QString errorMsgGet() { return _lastError; }
    QString statusMsgGet() { return _status; }
    int progressGet() { return _progress; }

    void generateErrorMsg(QString msg);
    void setStatus(QString status, int progress);

    Q_INVOKABLE void startProgram();
    Q_INVOKABLE void startVerify();


signals:
    void versionChanged();
    void vddStateChanged();
    void errorMsgChanged();
    void statusMsgChanged();
    void progressChanged();

private:
    bool _vddState;
    QString _lastError;
    QString _status;
    int _progress;

};


#endif // STM32P_H

