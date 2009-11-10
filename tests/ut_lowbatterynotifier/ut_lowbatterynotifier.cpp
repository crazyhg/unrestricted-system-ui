#include "ut_lowbatterynotifier.h"
#include "batterybusinesslogic.h"
#include "displaystatestub.h"

#include <DuiApplication>
#include <DuiTheme>

#include <QTime>
#include <QThread>

namespace {
    const int Act = 1000;
    const int Inact = 2000;
}

void LowBatteryHelper::start()
{
    times.clear();
    time.start();   
}

QList<int> LowBatteryHelper::notificationTimes()
{
    return times;
}

void LowBatteryHelper::notificationShown()
{
    times << time.restart();
}

void Ut_LowBatteryNotifier::init()
{            
    m_subject = new LowBatteryNotifier();
    m_helper = new LowBatteryHelper();
    connect(m_subject, SIGNAL(showNotification(QString)), m_helper, SLOT(notificationShown()));
    m_subject->activeInterval = Act;
    m_subject->inactiveInterval = Inact;
}

void Ut_LowBatteryNotifier::cleanup()
{
    delete m_subject;
    m_subject = NULL;
    delete m_helper;
    m_helper = NULL;    
}

DuiApplication *app;
void Ut_LowBatteryNotifier::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./ut_lowbatterynotifier";
    app = new DuiApplication(argc, &app_name);
}

void Ut_LowBatteryNotifier::cleanupTestCase()
{
    delete app;
}

void Ut_LowBatteryNotifier::testShowNotificationInActiveUse()
{               
    /*
        1) Display is on
        2) Waiting [2 * Active timeout + 50]
        3) Verify that 3 notifications were sent
            - First was sent right away
            - Second and third were sent at [Active timeout (+0,1 secs)]
    */
    m_subject->display->set(Maemo::QmDisplayState::On);
    m_helper->start();
    m_subject->showLowBatteryNotification();
    QTest::qWait(Act * 2 + 50);

    QCOMPARE(m_helper->notificationTimes().count(), 3);
    for(int i=0; i<m_helper->notificationTimes().count(); ++i) {        
        qDebug() << "times(" << i << "): " << m_helper->notificationTimes().at(i);
        if(i>0) {
            QVERIFY(m_helper->notificationTimes().at(i) <= Act + 100);
            QVERIFY(m_helper->notificationTimes().at(i) >= Act);
        }
        else {
            QVERIFY(m_helper->notificationTimes().at(i) <= 100);
            QVERIFY(m_helper->notificationTimes().at(i) >= 0);
        }
    }
}

void Ut_LowBatteryNotifier::testShowNotificationInDiverseUse()
{
    /*
        1) Display is on
        2) Waiting [Active timeout/2]
        3) Display is dimmed
        4) Display is on
        5) Waiting [Active timeout/4]
        6) Display is dimmed
        7) Display is off
        8) Waiting [Active timeout/2]
        9) Display is on
        10) Verify that 2 notifications were sent
            - First was sent right away
            - Second was sent at [Active timeout * 1.25 (+0,1 secs)]
    */
    m_subject->display->set(Maemo::QmDisplayState::On);
    m_helper->start();
    m_subject->showLowBatteryNotification();
    QTest::qWait(Act/2);
    m_subject->display->set(Maemo::QmDisplayState::Dimmed);
    m_subject->display->set(Maemo::QmDisplayState::On);
    QTest::qWait(Act/4);
    m_subject->display->set(Maemo::QmDisplayState::Dimmed);
    m_subject->display->set(Maemo::QmDisplayState::Off);
    QTest::qWait(Act/2);
    m_subject->display->set(Maemo::QmDisplayState::On);

    QCOMPARE(m_helper->notificationTimes().count(), 2);
    qDebug() << "times(" << 0 << "): " << m_helper->notificationTimes().at(0);
    QVERIFY(m_helper->notificationTimes().at(0) <= 100);
    QVERIFY(m_helper->notificationTimes().at(0) >= 0);
    qDebug() << "times(" << 1 << "): " << m_helper->notificationTimes().at(1);
    QVERIFY(m_helper->notificationTimes().at(1) <= Act/2 + Act/2 + Act/4 + 100);
    QVERIFY(m_helper->notificationTimes().at(1) >= Act/2 + Act/2 + Act/4);

}

void Ut_LowBatteryNotifier::testShowNotificationInInactiveUse()
{
    /*
        1) Display is off
        2) Waiting [Inactive timeout + Active timeout /2]
        3) Display is on
        4) Display is dimmed
        4) Display is off
        5) Waiting [Inactive timeout - Active timeout /2 + 50]
        6) Verify that 3 notifications were sent
            - First was sent right away
            - Second and third were sent at [Inactive timeout]
    */
    m_subject->display->set(Maemo::QmDisplayState::Off);
    m_helper->start();
    m_subject->showLowBatteryNotification();
    QTest::qWait(Inact + Act/2);
    m_subject->display->set(Maemo::QmDisplayState::On);
    m_subject->display->set(Maemo::QmDisplayState::Dimmed);
    m_subject->display->set(Maemo::QmDisplayState::Off);
    QTest::qWait(Inact - Act/2 + 50);

    QCOMPARE(m_helper->notificationTimes().count(), 3);
    for(int i=0; i<m_helper->notificationTimes().count(); ++i) {
        qDebug() << "times(" << i << "): " << m_helper->notificationTimes().at(i);
        if(i>0) {
            QVERIFY(m_helper->notificationTimes().at(i) <= Inact + 100);
            QVERIFY(m_helper->notificationTimes().at(i) >= Inact);
        }
        else {
            QVERIFY(m_helper->notificationTimes().at(i) <= 100);
            QVERIFY(m_helper->notificationTimes().at(i) >= 0);
        }
    }
}

QTEST_APPLESS_MAIN(Ut_LowBatteryNotifier)