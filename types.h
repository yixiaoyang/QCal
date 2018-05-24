/*
 * (C) Copyright 2013-2015 Gaoee. All rights reserved.
 *
 * @brief
 * @file    types.h
 * @version 1.0
 * @author  Leon
 * @note
 * @log
 *  - 2018-5-14 -Create by Leon<leon_e@qq.com>
 */
#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QDateTime>
#include <QDebug>

enum{
    STATUS_OK = 0,

    STATUS_FINISHED,
    STATUS_FINISHED_NEXT,

    STATUS_FAILED = -1,
    ERR_SYNTAX = 2,
};

#define DEBUG 1

#if DEBUG
#define g_debug()   qDebug()    << QTime::currentTime().toString("hh:mm:ss.zzz") << "[DBG][" << __FUNCTION__ << "]"
#else
#define g_debug()   if(false)\
                        qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz") << "[DBG][" << __FUNCTION__ << "]"
#endif

#define USE_LOG4QT                      (0)
#define USE_QLOG                        (1)

#if USE_QLOG
//#define qDebug                              QT_NO_QDEBUG_MACRO
#define g_fatal()   qFatal()    << QTime::currentTime().toString("hh:mm:ss.zzz") << "[FAT][" << __FUNCTION__ << "]"
#define g_error()   qCritical() << QTime::currentTime().toString("hh:mm:ss.zzz") << "[ERR][" << __FUNCTION__ << "]"
#define g_info()    qDebug()    << QTime::currentTime().toString("hh:mm:ss.zzz") << "[INF][" << __FUNCTION__ << "]"
//#define g_debug()   qDebug()    << QTime::currentTime().toString("hh:mm:ss.zzz") << "[DBG][" << __FUNCTION__ << "]"
#define g_warn()    qWarning()  << QTime::currentTime().toString("hh:mm:ss.zzz") << "[WAR][" << __FUNCTION__ << "]"
#endif

#define TEST        (0)

#endif // TYPES_H
