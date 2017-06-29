#include "mathematica.h"

Mathematica::Mathematica(QObject *parent) : QObject(parent)
{
    initMLink();
}

void Mathematica::sendRequest   (std::string expression)
{
    MLPutFunction   ( mLink, "ToExpression", 1  );
    MLPutString     ( mLink, expression.c_str() );
    MLEndPacket     ( mLink);

    responseHandle();
}
void Mathematica::sendRequest   (string functionName, string param)
{
    MLPutFunction   ( mLink, functionName.c_str(), 1 );
    MLPutString     ( mLink, param.c_str() );
    MLEndPacket     ( mLink);

    responseHandle();
}
void Mathematica::sendRequest   (string functionName, string param1, string param2)
{
    MLPutFunction   ( mLink, functionName.c_str(), 2 );
    MLPutString     ( mLink, param1.c_str() );
    MLPutString     ( mLink, param2.c_str() );
    MLEndPacket     ( mLink);

    responseHandle();
}
void Mathematica::sendRequest   (string functionName, QVector<double>& param1 , string param2)
{
    MLPutFunction   ( mLink, functionName.c_str(), 2 );
    MLPutRealList   ( mLink, param1.data(), param1.size() );
    MLPutString     ( mLink, param2.c_str() );
    MLEndPacket     ( mLink);

    responseHandle();
}
void Mathematica::sendRequest   (string functionName, QVector<double>& param)
{
    MLPutFunction   ( mLink, functionName.c_str(), 1 );
    MLPutRealList   ( mLink, param.data(), param.size() );
    MLEndPacket     ( mLink);

    responseHandle();
}

void Mathematica::initMLink     ()
{
    string initParams = "-linkmode launch -linkname '/usr/local/bin/math'";

    MLENV env = MLInitialize( (MLEnvironmentParameter) 0 );

    int openStatus;
    mLink = MLOpenString(env, initParams.c_str(), &openStatus);

    if ( mLink == (MLINK) 0 || openStatus != MLEOK )
        throw Exception{"can not create link to mathematica kernel"};
}

void Mathematica::responseHandle()
{
    while ( MLNextPacket(mLink) != RETURNPKT)
        MLNewPacket(mLink);

    readResponse(mLink);
}
void Mathematica::readResponse  (MLINK lp)
{
            int   i, len;
    const   char *s;
    static  int   indent;

    switch( MLGetNext(lp) )
    {
    case MLTKSYM:
        MLGetSymbol( lp, &s);
        printf( "%s ", s);
        MLReleaseSymbol( lp, s);
        break;
    case MLTKSTR:
        MLGetString( lp, &s);
        printf( "\"%s\" ", s);
        MLReleaseString( lp, s);
        break;
    case MLTKINT:
        int a;
        MLGetInteger( lp, &a);
        printf( "%d ", a);
        break;
    case MLTKREAL:
        double d;
        MLGetReal( lp, &d);
        printf( "%g ", d);
        break;
    case MLTKFUNC:
        indent += 3;
        printf( "\n %*.*s", indent, indent, "");
        if( MLGetArgCount( lp, &len) == 0)
            Logger::log(this, __FUNCTION__, "problem");
        else
        {
            readResponse( lp);
            printf( "[");
            for( i = 1; i <= len; ++i)
            {
                readResponse( lp);
                if( i != len) printf( ", ");
            }
            printf( "]");
        }
        indent -= 3;
        break;
    case MLTKERROR:
        Logger::log(this,__FUNCTION__,"received error code");
        break;
    default:
        Logger::log(this,__FUNCTION__, "received unknown head of message");
    }
}
