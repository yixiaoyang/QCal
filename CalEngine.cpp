/*
 * (C) Copyright 2013-2015 Gaoee. All rights reserved.
 *
 * @brief
 * @file    CalEngine.cpp
 * @version 1.0
 * @author  Leon
 * @note
 * @log
 *  - 2018-5-14 -Create by Leon<leon_e@qq.com>
 */
#include "CalEngine.h"

template<class T>
T l_calc(T left, T right, char op);


CalEngine::CalEngine()
{
    clear();
}

bool CalEngine::input(char ch)
{
    if(expressionFinished){
        expression += ch;
        expressionValid = false;
        return expressionValid;
    }

    int ret = tokenParser.input(ch);
    switch (ret) {
    case STATUS_FINISHED:
        expression += ch;
        appendToken(tokenParser);
        tokenParser.clear();
        break;
    case STATUS_FINISHED_NEXT:
        appendToken(tokenParser);
        tokenParser.clear();
        input(ch);
        break;
    case STATUS_OK:
        expression += ch;
        break;
    default:
        expression += ch;
        expressionValid = false;
    }

    if(ch == '='){
        expressionFinished = true;
    }

    return expressionValid;
}

void CalEngine::clear()
{
    expressionFinished = false;
    expressionValid = true;
    expression.clear();
    tokens.clear();
    tokenParser.clear();
    rpnTokens.clear();
}

bool CalEngine::cal()
{
    if(!tokenParser.value.token.isEmpty()){
        tokens.append(tokenParser.value);
        tokenParser.clear();
    }

    if(!expressionValid)
        return false;

    // 构建表达式树
    if(!buildRPN()){
        return false;
    }

    // 计算结果
    return this->calRPN();
}

QString CalEngine::getExpression() const
{
    return expression;
}

TokenValue CalEngine::getResult() const
{
    return _result;
}

void CalEngine::appendToken(CalTokenParser &t)
{
    if(t.value.token.isEmpty())
        return ;
    t.value.str2val();
    tokens.append(t.value);
    g_debug() << "token:" << t.value.token;
}

/**
 * @brief CalEngine::buildRPN 建立后缀表达时
 * @return
 */
bool CalEngine::buildRPN()
{
    rpnTokens.clear();
    QStack<int> opsTokenIdxs;
    for(int t = 0 ; t < tokens.size(); t++){
        const TokenValue& token = tokens.at(t);
        switch (token.flags.type) {
        case CalTokenTypeDouble:
        case CalTokenTypeNum:
            rpnTokens.append(t);
            break;
        case CalTokenTypeOp:
            if(opsTokenIdxs.isEmpty()){
                opsTokenIdxs.append(t);
            }else{
                // 如果遇到运算符号且栈非空
                //   查看栈顶元素
                //   如果栈顶元素的运算优先级大于或者等于该运算符号
                //      则持续出栈，直到栈顶元素优先级小于该运算符
                // 最后将该元素入栈
                while(!opsTokenIdxs.isEmpty()){
                    int idx = opsTokenIdxs.top();
                    TokenValue topToken = tokens.at(idx);
                    if(topToken >= token){
                        opsTokenIdxs.pop();
                        rpnTokens.append(idx);
                    }else{
                        break;
                    }
                }
                opsTokenIdxs.append(t);
            }
            break;
        case CalTokenTypeLeftQuote:
            opsTokenIdxs.append(t);
            break;
        case CalTokenTypeRightQuote:{
            // 如果遇到右括号，持续出栈直到左括号
            // 如果没有看到左括号报语法错
            bool syntaxErr = true;
            while(!opsTokenIdxs.isEmpty()){
                int idx = opsTokenIdxs.pop();
                TokenValue topToken = tokens.at(idx);
                if(topToken.flags.type == CalTokenTypeLeftQuote){
                    syntaxErr = false;
                    break;
                }
                rpnTokens.append(idx);
            }
            if(syntaxErr)
                return false;
            break;
        }
        default:
            return false;
        }
    }

    // 读到了输入的末尾，则将栈中所有元素依次弹出
    while(!opsTokenIdxs.isEmpty()){
        rpnTokens.append(opsTokenIdxs.pop());
    }

    return true;
}

/**
 * @brief CalEngine::calRPN 计算后缀表达式
 * @return
 */
bool CalEngine::calRPN()
{
#if TEST
    for(int i = 0; i < rpnTokens.size(); i++){
        int idx = rpnTokens.at(i);
        TokenValue topToken = tokens.at(idx);
        g_debug() << topToken.token;
    }
#endif

    QStack<TokenValue> valueTokens;
    bool meaningErr = false;
    while(!(rpnTokens.isEmpty() || meaningErr)){
        int idx = rpnTokens.front();
        rpnTokens.pop_front();
        TokenValue topToken = tokens.at(idx);
        switch (topToken.flags.type) {
        case CalTokenTypeNum:
            valueTokens.append(topToken);
            break;
        case CalTokenTypeDouble:
            valueTokens.append(topToken);
            break;
        case CalTokenTypeOp:
            if(valueTokens.size() < 2){
                meaningErr = true;
            }else{
                TokenValue tempResult = calculate(valueTokens.pop(), valueTokens.pop(), topToken);
                valueTokens.append(tempResult);
            }
            break;
        default:
            meaningErr = true;
            break;
        }
    }

    if(valueTokens.size() != 1){
        g_error() << "result size invalid" << valueTokens.size() ;
        return false;
    }
    if(!meaningErr){
        _result = valueTokens.top();
        _result.val2str();
        if(_result.flags.type == CalTokenTypeDouble){
            g_debug() << "result double" << _result.value.asDouble;
        }else{
            g_debug() << "result int" <<  _result.value.asInt;
        }
        return true;
    }
    return false;
}

TokenValue CalEngine::calculate(TokenValue left, TokenValue right, TokenValue op)
{
    TokenValue result;
    if(op.token.size() < 1){
        g_error() << "operator invalid" << op.token;
        return result;
    }

    char opCh = op.token.at(0).toLatin1();
    if((left.flags.type == CalTokenTypeDouble) || (right.flags.type == CalTokenTypeDouble)
            || (opCh=='/')){
        // 浮点数操作
        double lval = left.toDouble();
        double rval = right.toDouble();
        result.flags.type = CalTokenTypeDouble;
        result.value.asDouble = l_calc<double>(lval,rval,opCh);
    }else{
        // 整型操作
        int lval = left.value.asInt;
        int rval = right.value.asInt;
        result.flags.type = CalTokenTypeNum;
        result.value.asInt = l_calc<int>(lval,rval,opCh);
    }
    return result;
}

template<class T>
T l_calc(T left, T right, char op)
{
    T res;
    switch (op) {
    case '+':
        res = left+right;
        break;
    case '-':
        res = left-right;
        break;
    case '*':
        res = left*right;
        break;
    case '/':
        if(right != 0)
            res = left/right;
        else{
            g_error() << "oprator failed: div zero";
        }
        break;
    default:{
        g_error() << "operator invalid" << op;
        break;
    }
    }
    return res;
}
