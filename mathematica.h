#ifndef MATHEMATICA_H
#define MATHEMATICA_H

#include <QObject>
#include <logger.h>
#include <exception.h>
#include <mathlink.h>

using std::string;
using MikoTools::Logger;

class Mathematica : public QObject
{
    Q_OBJECT
  public:
    explicit Mathematica(QObject *parent = 0);

    void sendRequest    (string expression);
    void sendRequest    (string functionName, string param);
    void sendRequest    (string functionName, string param1, string param2);
    void sendRequest    (string functionName, QVector<double>& param1, string param2);
    void sendRequest    (string functionName, QVector<double>& param);

  private:
    void initMLink      ();
    void responseHandle ();
    void readResponse   (MLINK lp);

    MLINK mLink;
};

#endif // MATHEMATICA_H
