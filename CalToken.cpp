/*
 * (C) Copyright 2013-2015 Gaoee. All rights reserved.
 *
 * @brief
 * @file    CalToken.cpp
 * @version 1.0
 * @author  Leon
 * @note
 * @log
 *  - 2018-5-14 -Create by Leon<leon_e@qq.com>
 */
#include "CalToken.h"
#include <cstring>

CalTokenParser::CalTokenParser()
{
    clear();
}

void CalTokenParser::clear()
{
    state = CalTokenStateEmpty;
    memset((char*)&value.flags,0,sizeof(value.flags));
    value.token.clear();
}

int CalTokenParser::input(char ch)
{
    int type = CalTokenTypeUnknown;
    switch(ch){
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':{
        type = CalTokenTypeNum;
        break;
    }
    case '+':
    case '-':
    case '*':
    case '/':{
        type = CalTokenTypeOp;
        break;
    }
    case '.':{
        type = CalTokenTypeDot;
        break;
    }
    case '(':{
        type = CalTokenTypeLeftQuote;
        break;
    }
    case ')':{
        type = CalTokenTypeRightQuote;
        break;
    }
    case '=':{
        break;
    }
    default:{
        return -ERR_SYNTAX;
    }
    }

    return run_state_machine(type, ch);
}

bool CalTokenParser::done()
{
    return state == CalTokenStateFinished;
}

int CalTokenParser::run_state_machine(int type, char ch)
{
    int ret = STATUS_OK;

    if(ch == '='){
        state = CalTokenStateFinished;
        ret = STATUS_FINISHED;
        return ret;
    }

    switch(state){
    case CalTokenStateEmpty:{
        switch(type){
        case CalTokenTypeNum:{
            state = CalTokenStateInt;
            break;
        }
        case CalTokenTypeOp:
        case CalTokenTypeLeftQuote:
        case CalTokenTypeRightQuote:{
            state = CalTokenStateFinished;
            ret = STATUS_FINISHED;
            break;
        }
        //case CalTokenTypeDot:
        default:{
            // token不能以'.'开头
            return -ERR_SYNTAX;
        }
        }

        value.token += ch;
        value.flags.type = type;
        break;
    }
    case CalTokenStateInt:{
        if(type == CalTokenTypeNum){
            value.token += ch;
        }else if(type == CalTokenTypeDot){
            value.token += ch;
            state = CalTokenStateDoubleNeedSuffix;
        }else{
            if((type == CalTokenTypeOp) || (type == CalTokenTypeRightQuote)){
                state = CalTokenStateFinished;
                ret = STATUS_FINISHED_NEXT;
            }else{
                return ERR_SYNTAX;
            }
        }
        break;
    }
    case CalTokenStateDoubleNeedSuffix:{
        if(type == CalTokenTypeNum){
            value.token += ch;
            state = CalTokenStateDouble;
            value.flags.type = CalTokenTypeDouble;
        }else{
            return -ERR_SYNTAX;
        }
        break;
    }
    case CalTokenStateDouble:{
        if(type == CalTokenTypeNum){
            value.token += ch;
        }else{
            if((type == CalTokenTypeOp) || (type == CalTokenTypeRightQuote)){
                state = CalTokenStateFinished;
                ret = STATUS_FINISHED_NEXT;
            }else{
                return -ERR_SYNTAX;
            }
        }
        break;
    }
    case CalTokenStateFinished:{
        ret = STATUS_FINISHED_NEXT;
        break;
    }
    default:{
        return -ERR_SYNTAX;
    }
    }

    return ret;
}
