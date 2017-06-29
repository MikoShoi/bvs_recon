#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <QtConcurrent/QtConcurrent>

class Exception : public QException
{
public:
    Exception(std::string errorSource, std::string errorDescription = "")
    {
        std::cerr << "\n------ ERROR ------ \n \t source: \t" + errorSource + "\n";

        if( !errorDescription.empty() )
            std::cerr << "\t description: \t" + errorDescription + "\n";

        std::cerr << "\n";
    }

    void        raise()     const
    {
        throw *this;
    }

    Exception   *clone()    const
    {
        return new Exception(*this);
    }
};

#endif // EXCEPTION_H
