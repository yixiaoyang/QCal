/*
 * (C) Copyright 2013-2015 Gaoee. All rights reserved.
 *
 * @brief
 * @file    CalEngine.h
 * @version 1.0
 * @author  Leon
 * @note
 * @log
 *  - 2018-5-14 -Create by Leon<leon_e@qq.com>
 */
#ifndef CALENGINE_H
#define CALENGINE_H

#include <QString>
#include <QVector>
#include <QStack>

#include "CalToken.h"

class CalEngine
{
public:
    CalEngine();

    bool input(char ch);
    void clear();
    bool cal();

    QString getExpression() const;
    TokenValue getResult() const;

private:
    QString expression;
    bool expressionValid;
    bool expressionFinished;
    int status;
    TokenValue _result;

    QVector <TokenValue> tokens;
    QStack <int> rpnTokens;
    CalTokenParser tokenParser;
private:
    void appendToken(CalTokenParser& t);
    /**
     * @brief rpn 逆波兰表示法，中缀转后缀
     * @return true 转换成功
     *         false 转换失败，语法错误
     */
    bool buildRPN();
    bool calRPN();

    /**
     * @brief calculate 计算二目算式，返回结果
     * @param left
     * @param right
     * @param op
     * @return
     */
    TokenValue calculate(TokenValue left, TokenValue right, TokenValue op);

};

#endif // CALENGINE_H
