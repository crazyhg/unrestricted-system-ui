/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
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

#include "statusareaview.h"
#include "statusarea.h"
#include "statusindicator.h"
#include "clock.h"
#include "contextframeworkcontext.h"
#include "notificationstatusindicator.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>
#include <QDBusConnection>
#include <MDeviceProfile>
#include <MViewCreator>

StatusAreaView::StatusAreaView(StatusArea *controller) :
    MWidgetView(controller),
    landscapeWidget(new QGraphicsWidget),
    portraitWidget(new QGraphicsWidget),
    controller(controller),
    landscapePhoneNetworkIndicator(new PhoneNetworkStatusIndicator(contextFrameworkContext, controller)),
    portraitPhoneNetworkIndicator(new PhoneNetworkStatusIndicator(contextFrameworkContext, controller)),
    landscapeAlarmIndicator(new AlarmStatusIndicator(contextFrameworkContext, controller)),
    portraitAlarmIndicator(new AlarmStatusIndicator(contextFrameworkContext, controller)),
    landscapeBatteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    portraitBatteryIndicator(new BatteryStatusIndicator(contextFrameworkContext, controller)),
    landscapePhoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    portraitPhoneSignalStrengthIndicator(new PhoneNetworkSignalStrengthStatusIndicator(contextFrameworkContext, controller)),
    landscapePhoneNetworkTypeIndicator(new PhoneNetworkTypeStatusIndicator(contextFrameworkContext, controller)),
    portraitPhoneNetworkTypeIndicator(new PhoneNetworkTypeStatusIndicator(contextFrameworkContext, controller)),
    landscapeInternetConnectionIndicator(new InternetConnectionStatusIndicator(contextFrameworkContext, controller)),
    portraitInternetConnectionIndicator(new InternetConnectionStatusIndicator(contextFrameworkContext, controller)),
    landscapeBluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    portraitBluetoothIndicator(new BluetoothStatusIndicator(contextFrameworkContext, controller)),
    landscapeGPSIndicator(new GPSStatusIndicator(contextFrameworkContext, controller)),
    portraitGPSIndicator(new GPSStatusIndicator(contextFrameworkContext, controller)),
    landscapePresenceIndicator(new PresenceStatusIndicator(contextFrameworkContext, controller)),
    portraitPresenceIndicator(new PresenceStatusIndicator(contextFrameworkContext, controller)),
    landscapeProfileIndicator(new ProfileStatusIndicator(contextFrameworkContext, controller)),
    portraitProfileIndicator(new ProfileStatusIndicator(contextFrameworkContext, controller)),
    landscapeInputMethodIndicator(new InputMethodStatusIndicator(controller)),
    landscapeCallIndicator(new CallStatusIndicator(contextFrameworkContext, controller)),
    portraitCallIndicator(new CallStatusIndicator(contextFrameworkContext, controller)),
    landscapeClock(new Clock(controller)),
    portraitClock(new Clock(controller))
{
    // Connect related phone network indicators
    connect(portraitPhoneNetworkTypeIndicator,  SIGNAL(networkAvailabilityChanged(bool)), portraitPhoneSignalStrengthIndicator, SLOT(setDisplay(bool)));
    connect(landscapePhoneNetworkTypeIndicator, SIGNAL(networkAvailabilityChanged(bool)), landscapePhoneSignalStrengthIndicator, SLOT(setDisplay(bool)));

    // Set the clock to short time display when alarm is present
    connect(portraitAlarmIndicator, SIGNAL(alarmSettingChanged(bool)), portraitClock, SLOT(setShortDisplay(bool)));
    connect(landscapeAlarmIndicator, SIGNAL(alarmSettingChanged(bool)), landscapeClock, SLOT(setShortDisplay(bool)));

    // Set up landscape and portrait widgets and anchor them on top of each other
    landscapeWidget->setLayout(createLandscapeLayout());
    landscapeWidget->setParent(controller);
    portraitWidget->setParent(controller);
    portraitWidget->setLayout(createPortraitLayout());
    QGraphicsAnchorLayout *compositeLayout = new QGraphicsAnchorLayout;
    compositeLayout->setContentsMargins(0, 0, 0, 0);
    compositeLayout->setSpacing(0);

    compositeLayout->addCornerAnchors(landscapeWidget, Qt::TopLeftCorner, compositeLayout, Qt::TopLeftCorner);
    compositeLayout->addCornerAnchors(landscapeWidget, Qt::TopRightCorner, compositeLayout, Qt::TopRightCorner);
    compositeLayout->addCornerAnchors(portraitWidget, Qt::TopLeftCorner, landscapeWidget, Qt::BottomLeftCorner);
    compositeLayout->addCornerAnchors(portraitWidget, Qt::BottomLeftCorner, compositeLayout, Qt::BottomLeftCorner);
    controller->setLayout(compositeLayout);

    // Connect to D-Bus and register the DBus source as an object
    QDBusConnection::sessionBus().registerService("com.meego.core.MInputMethodStatusIndicator");
    QDBusConnection::sessionBus().registerObject("/inputmethodstatusindicator", landscapeInputMethodIndicator);

    //Set up the class for functional testing
    setupTestability();
}

void StatusAreaView::setupTestabilityObjectNames()
{
    landscapeClock->setObjectName("landscapeclock");
    portraitClock->setObjectName("portraitclock");
}

void StatusAreaView::setupTestabilityParents()
{
    portraitPhoneNetworkIndicator->setParent(portraitWidget);
    landscapeAlarmIndicator->setParent(landscapeWidget);
    portraitAlarmIndicator->setParent(portraitWidget);
    landscapeBatteryIndicator->setParent(landscapeWidget);
    portraitBatteryIndicator->setParent(portraitWidget);
    landscapePhoneSignalStrengthIndicator->setParent(landscapeWidget);
    portraitPhoneSignalStrengthIndicator->setParent(portraitWidget);
    landscapePhoneNetworkTypeIndicator->setParent(landscapeWidget);
    portraitPhoneNetworkTypeIndicator->setParent(portraitWidget);
    landscapeInternetConnectionIndicator->setParent(landscapeWidget);
    portraitInternetConnectionIndicator->setParent(portraitWidget);
    landscapeBluetoothIndicator->setParent(landscapeWidget);
    portraitBluetoothIndicator->setParent(portraitWidget);
    landscapeGPSIndicator->setParent(landscapeWidget);
    portraitGPSIndicator->setParent(portraitWidget);
    landscapePresenceIndicator->setParent(landscapeWidget);
    portraitPresenceIndicator->setParent(portraitWidget);
    landscapeProfileIndicator->setParent(landscapeWidget);
    portraitProfileIndicator->setParent(portraitWidget);
    landscapeInputMethodIndicator->setParent(landscapeWidget);
    landscapeCallIndicator->setParent(landscapeWidget);
    portraitCallIndicator->setParent(portraitWidget);
    landscapeClock->setParent(landscapeWidget);
    portraitClock->setParent(portraitWidget);
}

void StatusAreaView::setupTestability()
{
    setupTestabilityObjectNames();
    setupTestabilityParents();
}

StatusAreaView::~StatusAreaView()
{
}

void StatusAreaView::setGeometry(const QRectF &rect)
{
    int screenWidth = MDeviceProfile::instance()->resolution().width();
    int screenHeight = MDeviceProfile::instance()->resolution().height();
    qreal areaHeight = rect.height() / 2;
    landscapeWidget->setMinimumSize(screenWidth, areaHeight);
    landscapeWidget->setMaximumSize(screenWidth, areaHeight);
    portraitWidget->setMinimumSize(screenHeight, areaHeight);
    portraitWidget->setMaximumSize(screenHeight, areaHeight);
}

QGraphicsLinearLayout* StatusAreaView::createLandscapeLayout()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Put indicators into the layout
    layout->addItem(landscapeBatteryIndicator);
    layout->addItem(landscapePhoneSignalStrengthIndicator);
    layout->addItem(landscapePhoneNetworkTypeIndicator);
    layout->addItem(landscapePhoneNetworkIndicator);
    layout->addStretch();
    layout->addItem(landscapeInternetConnectionIndicator);
    layout->addItem(landscapeBluetoothIndicator);
    layout->addItem(landscapeGPSIndicator);
    layout->addItem(landscapePresenceIndicator);
    layout->addItem(landscapeProfileIndicator);
    layout->addItem(landscapeInputMethodIndicator);
    layout->addItem(landscapeCallIndicator);
    layout->addItem(landscapeClock);
    layout->addItem(landscapeAlarmIndicator);

    return layout;
}

QGraphicsLinearLayout* StatusAreaView::createPortraitLayout()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Put indicators into the layout
    layout->addItem(portraitBatteryIndicator);
    layout->addItem(portraitPhoneSignalStrengthIndicator);
    layout->addItem(portraitPhoneNetworkTypeIndicator);
    layout->addItem(portraitPhoneNetworkIndicator);
    layout->addStretch();
    layout->addItem(portraitInternetConnectionIndicator);
    layout->addItem(portraitBluetoothIndicator);
    layout->addItem(portraitGPSIndicator);
    layout->addItem(portraitPresenceIndicator);
    layout->addItem(portraitProfileIndicator);
    layout->addItem(portraitCallIndicator);
    layout->addItem(portraitClock);
    layout->addItem(portraitAlarmIndicator);

    return layout;
}

M_REGISTER_VIEW_NEW(StatusAreaView, StatusArea)