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

    // 计算结果
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

    /**
     * @brief calRPN 计算逆波兰表示式的结果
     * @return true 计算完成
     *         false 计算失败，语义错误
     */
    bool calRPN();

    /**
     * @brief calculate 计算二目算式，返回结果
     * @param left
     * @param right
     * @param op
     * @return 返回操作结果
     * @todo 抛出异常，如除以0之类的情况
     */
    TokenValue calculate(TokenValue left, TokenValue right, TokenValue op);

};

#endif // CALENGINE_H
