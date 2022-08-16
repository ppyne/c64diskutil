#ifndef C64EXCEPTION_H
#define C64EXCEPTION_H

#include <QException>
#include <QString>

class C64Exception : public QException
{
public:
    C64Exception(QString message);
    QString message();
private:
    QString Message;
};

#endif // C64EXCEPTION_H
