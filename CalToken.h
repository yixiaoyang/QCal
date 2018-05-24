/*
 * (C) Copyright 2013-2015 Gaoee. All rights reserved.
 *
 * @brief
 * @file    CalToken.h
 * @version 1.0
 * @author  Leon
 * @note
 * @log
 *  - 2018-5-14 -Create by Leon<leon_e@qq.com>
 */
#ifndef CALTOKEN_H
#define CALTOKEN_H

#include <QString>
#include "types.h"

enum{
    CalTokenTypeUnknown,
    CalTokenTypeNum,
    CalTokenTypeOp,
    CalTokenTypeLeftQuote,
    CalTokenTypeRightQuote,
    CalTokenTypeDot,
    CalTokenTypeDouble,
};

enum{
    CalTokenStateEmpty,
    CalTokenStateInt,
    CalTokenStateDoubleNeedSuffix,
    CalTokenStateDouble,
    CalTokenStateFinished
};

typedef union _TokenValueUnion{
    double asDouble;
    long long int asInt;
    char asChar;
}TokenValueUnion;

typedef struct _TokenValue{
    TokenValueUnion value;
    QString token;
    struct {
        unsigned int type:3;
        unsigned int is_double:1;
        unsigned int inverse:4;
    }flags;

    bool operator >=(const struct _TokenValue& anotherToken){
        if(anotherToken.flags.type != this->flags.type)
            return false;
        if(this->flags.type != CalTokenTypeOp)
            return false;
        if((token == "*") ||
           (token == "/") ||
           (anotherToken.token == "+") ||
           (anotherToken.token == "-"))
        {
            return true;
        }
        return false;
    }

    double toDouble(){
        if(flags.type == CalTokenTypeNum)
            return value.asInt;
        else if(flags.type == CalTokenTypeDouble)
            return value.asDouble;
        return 0.0;
    }

    void val2str(){
        if(flags.type == CalTokenTypeNum){
            token = QString::number(value.asInt);
        }
        else if(flags.type == CalTokenTypeDouble){
            token = QString::number(value.asDouble);
        }
    }

    void str2val(){
        if(flags.type == CalTokenTypeNum){
            value.asInt = token.toInt();
        }
        else if(flags.type == CalTokenTypeDouble){
            value.asDouble = token.toDouble();
        }
    }
}TokenValue;

class CalTokenParser
{
public:
    CalTokenParser();
    void clear();
    int input(char ch);
    bool done();

public:
    int state;
    TokenValue value;

private:
    int run_state_machine(int type, char ch);
};

#endif // CALTOKEN_H
