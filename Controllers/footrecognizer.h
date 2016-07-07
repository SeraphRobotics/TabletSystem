#ifndef FOOTRECOGNIZER_H
#define FOOTRECOGNIZER_H

#include <QString>

class FootRecognizer
{
public:
    enum FootType
    {
        RightFoot,
        LeftFoot
    };

    static FootType recognize(const QString &scanPath);
};

#endif // FOOTRECOGNIZER_H
