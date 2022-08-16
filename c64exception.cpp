#include "c64exception.h"

C64Exception::C64Exception(QString message)
{
    this->Message = message;
}

QString C64Exception::message() {
    return this->Message;
}
