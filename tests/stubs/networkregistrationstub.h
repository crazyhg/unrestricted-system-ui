#ifndef NETWORKREGISTRATION_H
#define NETWORKREGISTRATION_H

#include <QObject>

namespace Cellular
{
//! The AvailableOperator class provides a network operator
class AvailableOperator
{
public:

    enum Availability
    {
        UnknownAvailability = -1, //!< Availability unknown
        Available, //!< Operator is available
        Current, //!< Operator is the current registered operator
        NotAvailable //!< Operator is not available
    };

    AvailableOperator(const QString &mnc, const QString &mcc, const QString &name, AvailableOperator::Availability availability);
    ~AvailableOperator();

     //! Set operator MNC
    void setMnc(const QString &mnc);
    //! Set operator MCC
    void setMcc(const QString &mcc);
    //! Set operator Name
    void setName(const QString &name);
    //! Set operator availability
    void setAvailability(AvailableOperator::Availability availability);

     //! Get operator MNC
    QString mnc() const;
    //! Get operator MCC
    QString mcc() const;
    //! Get operator name
    QString name() const;
    //! Get operator availability
    AvailableOperator::Availability availability() const;

private:
    QString currentMnc;
    QString currentMcc;
    QString currentName;
    AvailableOperator::Availability currentAvailibility;

};

class NetworkRegistration : public QObject
{
    Q_OBJECT

public:
    NetworkRegistration(QObject *parent=0);
    ~NetworkRegistration();

    enum Mode
    {
        UnknownMode = -1, //!< Network access mode is unknown
        Manual, //!< Manual network access selection
        Automatic //!< Automatic network access selection
    };

    //! Get access selection mode
    NetworkRegistration::Mode mode() const;
    //! Automatically select any available operator
    void selectOperator();
    //! Select an operator based on MNC/MCC code
    void selectOperator(const QString &mnc, const QString &mcc);
     //! Query for available networks
    void queryAvailableOperators();

signals:
     //! Access selection mode changed
    void modeChanged(int mode);
    //! Asynchronous response to available network scan
    void availableOperators(bool success, const QList<AvailableOperator*> &operators, const QString &reason);
    //! Asynchronous response to network selection
    void selectionCompleted(bool success, const QString &reason);

private:
    QList<AvailableOperator*> operators;

};
}

#endif // NETWORKREGISTRATION_H