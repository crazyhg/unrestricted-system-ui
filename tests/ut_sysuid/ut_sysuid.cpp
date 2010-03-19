/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of system ui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "ut_sysuid.h"

#include <QDBusConnection>
#include <DuiApplication>
#include "duicompositornotificationsink.h"
#include "duifeedbacknotificationsink.h"
#include "testcontextitem.h"
#include "contextframeworkcontext.h"
#include "sysuid.h"
#include "widgetnotificationsink_stub.h"
#include "eventtypestore_stub.h"
#include "notificationmanager_stub.h"
#include "statusareawindow_stub.h"
#include "unlockslider_stub.h"

maemosec::storage::~storage()
{
}

Notification::~Notification()
{
}

NotificationGroup::~NotificationGroup()
{
}

NotificationParameters::NotificationParameters()
{
}

NotificationParameters::~NotificationParameters()
{
}

// NotificationSink stubs (used by DuiCompositorNotificationSink)
NotificationSink::NotificationSink()
{
}

void NotificationSink::addGroup(uint, const NotificationParameters &)
{
}

void NotificationSink::removeGroup(uint)
{
}

QHash <NotificationSink *, bool> sinkSetEnabled;
void NotificationSink::setApplicationEventsEnabled(bool enabled)
{
    sinkSetEnabled[this] = enabled;
}

// DuiCompositorNotificationSink stubs (used by Sysuid)
DuiCompositorNotificationSink *duiCompositorNotificationSink = 0;
DuiCompositorNotificationSink::DuiCompositorNotificationSink()
{
    duiCompositorNotificationSink = this;
}

DuiCompositorNotificationSink::~DuiCompositorNotificationSink()
{
}

void DuiCompositorNotificationSink::addNotification(const Notification &)
{
}

void DuiCompositorNotificationSink::removeNotification(uint)
{
}

void DuiCompositorNotificationSink::timeout()
{
}

void DuiCompositorNotificationSink::rotateInfoBanners(const Dui::Orientation &)
{
}

// DuiFeedbackNotificationSink stubs (used by Sysuid)
DuiFeedbackNotificationSink *duiFeedbackNotificationSink = 0;
DuiFeedbackNotificationSink::DuiFeedbackNotificationSink()
{
    duiFeedbackNotificationSink = this;
}

DuiFeedbackNotificationSink::~DuiFeedbackNotificationSink()
{
}

void DuiFeedbackNotificationSink::addNotification(const Notification &)
{
}

void DuiFeedbackNotificationSink::removeNotification(uint)
{
}

// QDBusConnection stubs (used by Sysuid)
bool QDBusConnection::registerService(QString const &)
{
    return true;
}

bool QDBusConnection::registerObject(QString const &, QObject *, QFlags<QDBusConnection::RegisterOption>)
{
    return true;
}

// ContextItem & ContextFrameworkItem stubs
TestContextItem *testContextItem;
ContextItem *ContextFrameworkContext::createContextItem(const QString&)
{
    testContextItem = new TestContextItem;
    return testContextItem;
}

ContextFrameworkItem::ContextFrameworkItem(const QString &key)
    : property(key)
{
    connect(&property, SIGNAL(valueChanged()), this, SIGNAL(contentsChanged()));
}

QVariant ContextFrameworkItem::value() const
{
    return property.value();
}

void Ut_Sysuid::initTestCase()
{
}

void Ut_Sysuid::cleanupTestCase()
{
}

void Ut_Sysuid::init()
{
    static char *args[] = {(char *) "./ut_sysuid"};
    static int argc = sizeof(args) / sizeof(char *);

    app = new DuiApplication(argc, args);
    sysuid = new Sysuid();
}

void Ut_Sysuid::cleanup()
{
    delete sysuid;
    delete app;
}

void Ut_Sysuid::testUseMode()
{
    testContextItem->setValue("");
    QVERIFY(sinkSetEnabled[duiCompositorNotificationSink]);
    QVERIFY(sinkSetEnabled[duiFeedbackNotificationSink]);
    testContextItem->setValue("recording");
    QVERIFY(!sinkSetEnabled[duiCompositorNotificationSink]);
    QVERIFY(!sinkSetEnabled[duiFeedbackNotificationSink]);
}

QTEST_APPLESS_MAIN(Ut_Sysuid)
