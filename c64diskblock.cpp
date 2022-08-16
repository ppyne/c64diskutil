#include "c64diskblock.h"
#include "c64disk.h"
#include "c64disktrack.h"
#include "c64disksector.h"
#include <string.h>
#include <QDebug>
#include <QByteArray>

const unsigned char CHAR_NULL = 0x00;
const unsigned char CHAR_BACKSPACE = 0x08;
const unsigned char CHAR_NBSP = 0xA0;
const unsigned char CHAR_POUND = 0xA3;

const unsigned char PETSCII_TABLE[256] = {
    /* Control characters */
    /*         0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F */
    /* 0x00 */ '\0', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0x10 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* Printable characters */
    /* 0x20 */ ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
    /* 0x30 */ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
    /* 0x40 */ '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    /* 0x50 */ 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', CHAR_POUND, ']', '^', '_',
    /* 0x60 */ '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    /* 0x70 */ 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '.', /* Delete */
    /* Extended ASCII */
    /* 0x80 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0x90 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0xA0 */ CHAR_NBSP, '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0xB0 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0xC0 */ '.', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    /* 0xD0 */ 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.', '.', '.', '.', '.',
    /* 0xE0 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0xF0 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'
};

const unsigned char GEOS_ASCII_TABLE[256] = {
    /* Control characters */
    /*         0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F */
    /* 0x00 */ '\0', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0x10 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* Printable characters */
    /* 0x20 */ ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
    /* 0x30 */ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
    /* 0x40 */ '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    /* 0x50 */ 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
    /* 0x60 */ '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    /* 0x70 */ 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '.', /* Delete */
    /* Extended ASCII */
    /* 0x80 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0x90 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0xA0 */ CHAR_NBSP, '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0xB0 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0xC0 */ '.', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    /* 0xD0 */ 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.', '.', '.', '.', '.',
    /* 0xE0 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    /* 0xF0 */ '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'
};

C64DiskBlock::C64DiskBlock(C64DiskSector *parent, char *data) {
    this->Parent = parent;
    this->Data = data;
    this->Sector = this->parent();
    this->Track = this->Sector->parent();
    this->Disk = this->Track->parent();
}

C64DiskSector *C64DiskBlock::parent() {
    return this->Parent;
}

int C64DiskBlock::nextTrack() {
    return (unsigned char)this->Data[0];
}

int C64DiskBlock::nextSector() {
    return (unsigned char)this->Data[1];
}

void C64DiskBlock::removeA0Padding(char *str) {
    int i = strlen(str) - 1;
    while (i >= 0 && str[i] == (char)0xA0) {
        str[i] = 0;
        i--;
    }
}

void C64DiskBlock::petAsciiToAscii(char *str) {
    unsigned char c;
    unsigned char *s = (unsigned char *)str;
    while ((c = *s)) {
        c &= 0x7f;
        /*if (c >= 'A' && c <= 'Z') c += 32;
        else if (c >= 'a' && c <= 'z') c -= 32;
        else if (c == 0x7f) c = 0x3f;*/
        if (c == 0x7f) c = 0x3f;
        *s++ = c;
    }
}

char *C64DiskBlock::rawData() {
    return this->Data + 2;
}

QString C64DiskBlock::toHex(const char *raw, size_t len) {
    QByteArray ba;
    for (size_t i = 0; i < len; i++) ba.append(raw[i]);
    return ba.toHex(' ');
}

QString C64DiskBlock::fromGeos(const char *raw, size_t len) {
    char *str = new char[len];
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)raw[i];
        str[i] =  GEOS_ASCII_TABLE[c];
    }
    QByteArray ba;
    for (size_t i = 0; i < len; i++) ba.append(str[i]);
    delete []str;
    return QString::fromLatin1(ba);
}

QByteArray C64DiskBlock::data() {
    QByteArray data;
    for (int i = 0; i < 256; i++) data.append(this->Data[i]);
    return data;
}

int C64DiskBlock::track() {
    return this->Track->id();
}

int C64DiskBlock::sector() {
    return this->Sector->id();
}
