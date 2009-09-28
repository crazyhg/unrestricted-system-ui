#include <QDebug>
#include <QTimer>
#include <DuiApplicationWindow>
#include <DuiApplication>

#include "shutdowndialogbusinesslogic.h"
#include <qmsystem/qmsystemstate.h>
#include <qmsystem/qmled.h>
#include <qmsystem/qmalarm.h>
#include <qmsystem/qmalarmevent.h>

const int SHUTDOWN_TIME = 2000/*ms*/;
const int UPDATE_INTERVAL = 50;/*ms*/
const int IDLE_TIME = 10000/*ms*/;

using namespace Maemo;

ShutdownDialogBusinessLogic::ShutdownDialogBusinessLogic() : QObject()
{
    shutdownDlg = NULL;
    shuttingDown = false;
    powerKeyPressTimer = NULL;
    idleTimer = NULL;
    currentPosInTime = 0;
    currentPos = 0;
}
ShutdownDialogBusinessLogic::~ShutdownDialogBusinessLogic()
{
    if (shutdownDlg)
        shutdownDlg->deleteLater();
    shutdownDlg = NULL;
}

void ShutdownDialogBusinessLogic::openDialog(bool btnDown)
{
    qDebug() << "openDialog";
    if (shutdownDlg != NULL || shuttingDown) {
        qDebug() << "won't open shuttingDown:" << shuttingDown;
        return;
    }
    emit dialogOpen(true);

    DuiApplication::instance()->applicationWindow()->show();

    shutdownDlg = new ShutdownDialog(QString("Shutdown dialog"),
                                     QString("next alarm event"), DuiDialog::NoButton);
    connect(shutdownDlg->slider(), SIGNAL(unlocked()), this, SLOT(shutdown()));
    connect(shutdownDlg->slider(), SIGNAL(released()), this, SLOT(resetIdleTimer()));

    if(btnDown)
        startPowerKeyPressTimer();        

    resetIdleTimer();

    /* shutdownDlg->setSystemModal(); NOT YET IMPLEMENTED */
    shutdownDlg->exec();

}

void ShutdownDialogBusinessLogic::closeDialog()
{
    /* Here we should turn off the device instead of just deleting the dialog */
    qDebug() << "closeDialog()";
    stopPowerKeyPressTimer();
    stopIdleTimer();
    if (shutdownDlg) {
        shutdownDlg->reject();
        shutdownDlg->deleteLater();
        shutdownDlg = NULL;
    }
    DuiApplication::instance()->applicationWindow()->hide();
    emit dialogOpen(false);
}

void ShutdownDialogBusinessLogic::powerKeyDown()
{       
    qDebug() << "powerKeyDown()";
    if(shuttingDown)
        return;

    resetIdleTimer();

    if(powerKeyPressTimer == NULL)
        startPowerKeyPressTimer();
    if(shutdownDlg != NULL)
        if(shutdownDlg->slider() != NULL)
            shutdownDlg->slider()->hwKeyDown(true);
}

void ShutdownDialogBusinessLogic::powerKeyUp()
{
    qDebug() << "powerKeyUp()";
    if(shuttingDown)
        return;

    stopPowerKeyPressTimer();
    if(shutdownDlg != NULL)
        if(shutdownDlg->slider() != NULL)
            shutdownDlg->slider()->hwKeyDown(false);
}

void ShutdownDialogBusinessLogic::startPowerKeyPressTimer()
{
    qDebug() << "startPowerKeyPressTimer()";
    t.start();

    if(shutdownDlg != NULL) {
        if(shutdownDlg->slider() != NULL) {
            currentPosInTime = (int)(SHUTDOWN_TIME * shutdownDlg->slider()->position());
            currentPos = shutdownDlg->slider()->position();
        }
    }

    if(powerKeyPressTimer == NULL) {
        powerKeyPressTimer = new QTimer(this);
        powerKeyPressTimer->setInterval(UPDATE_INTERVAL);
        connect(powerKeyPressTimer, SIGNAL(timeout()), this, SLOT(updateSlider()));
        powerKeyPressTimer->start();
    }
}

void ShutdownDialogBusinessLogic::stopPowerKeyPressTimer()
{   
    if(powerKeyPressTimer != NULL) { 
        powerKeyPressTimer->stop();
        powerKeyPressTimer->deleteLater();        
        powerKeyPressTimer = NULL;        
    }
}

void ShutdownDialogBusinessLogic::stopIdleTimer()
{
    qDebug() << "stopIdleTimer()";
    if(idleTimer != NULL) {
        idleTimer->stop();
        idleTimer->deleteLater();
        idleTimer = NULL;
    }
}

void ShutdownDialogBusinessLogic::resetIdleTimer()
{
    if(shuttingDown)
        return;

    if(idleTimer == NULL) {
        idleTimer = new QTimer(this);
        connect(idleTimer, SIGNAL(timeout()), this, SLOT(closeDialog()));
    }
    idleTimer->start(IDLE_TIME);
}

void ShutdownDialogBusinessLogic::updateSlider()
{
    qDebug() << "updateSlider";
    if(shutdownDlg != NULL) {
        if(shutdownDlg->slider() != NULL) {
            shutdownDlg->slider()->setPosition(
                    (double)(t.elapsed()/(double)(SHUTDOWN_TIME-currentPosInTime)) * (1 - currentPos)
                    );
        }
    }

    /* Note: We cannot just use the powerKeyPressTimer and check when it reaches the SHUTDOWN_TIME.
       Updating the slider takes time so that the powerKeyPressTimer reaches the SHUTDOWN_TIME way too
       late. Instead we use separate QTime t to define the actual SHUTDOWN_TIME. */
}

void ShutdownDialogBusinessLogic::shutdown()
{
    qDebug() << "shutdown()";
    stopPowerKeyPressTimer();
    stopIdleTimer();
    shuttingDown = true;
    if(shutdownDlg != NULL) {
        if(shutdownDlg->slider() != NULL) {
            shutdownDlg->slider()->setEnabled(false);
            //shutdownDlg->slider()->pressed();
        }
    }

    QmLED led;
    led.activate(QString("PatternShutDown"));

    QmSystemState state;
    state.set(QmSystemState::Shutdown);        
}

void ShutdownDialogBusinessLogic::setAlarmLabelText()
{
    /*
    QmAlarm alarm;
    QList<qmAlarmCookie> alarmList = alarm.query(0,0,QmAlarmEvent::Disabled, 0, QString("CLOCK_APP"));
    */
}
