#include "phonenetworkcontainer.h"
#include "networksignalstrengthicon.h"

#include <DuiButton>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiGridLayoutPolicy>

PhoneNetworkContainer::PhoneNetworkContainer(DuiWidget *parent) :
        DuiContainer(parent),
        toggleButton(NULL),
        operatorLabel(NULL),
        signalStrengthIcon(NULL),
        networkOperator(NULL),
        radioAccess(NULL),
        networkTechnology(NULL)
{
    signalStrengthIcon = new NetworkSignalStrengthIcon();
    networkOperator = new NetworkOperator();
    radioAccess = new RadioAccess();
    networkTechnology = new NetworkTechnology(radioAccess);
    connect(networkTechnology, SIGNAL(technologyChanged(NetworkTechnology::Technology)),
            this, SLOT(updateButtonIcon(NetworkTechnology::Technology)));

    toggleButton = new DuiButton();
    toggleButton->setCheckable(true);
    toggleButton->setChecked(/* TODO: check state from correct API */true);
    toggleButton->setObjectName("connectivityButton");
    connect(toggleButton, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)));    

    operatorLabel = new DuiLabel();
    operatorLabel->setObjectName("connectivityLabel2");
    operatorLabel->setText(networkOperator->name());
    connect(networkOperator, SIGNAL(nameChanged(QString)), operatorLabel, SLOT(setText(QString)));   

    updateButtonIcon(networkTechnology->currentTechnology());

    setLayout();
}

PhoneNetworkContainer::~PhoneNetworkContainer()
{
    delete networkOperator;
    networkOperator = NULL;
    delete networkTechnology;
    networkTechnology = NULL;
    delete radioAccess;
    radioAccess = NULL;
}

bool PhoneNetworkContainer::phoneNetworkOn()
{
    return toggleButton->isChecked();
}

void PhoneNetworkContainer::togglePhoneNetwork(bool toggle)
{ 
    toggleButton->setChecked(toggle);
}

void PhoneNetworkContainer::updateButtonIcon(NetworkTechnology::Technology technology)
{
    toggleButton->setIconID(mapTechnologyToIcon(technology));
}

QString PhoneNetworkContainer::mapTechnologyToIcon(NetworkTechnology::Technology technology)
{    
    QString icon;
    switch(technology) {
        case NetworkTechnology::TwoG:
            icon = QString("icon-s-gsm");
            break;
        case NetworkTechnology::TwoPointFiveG:
            icon = QString("icon-s-25g");
            break;
        case NetworkTechnology::ThreeG:
            icon = QString("icon-s-3g");
            break;
        case NetworkTechnology::ThreePointFiveG:
            icon = QString("icon-s-35g");
            break;
        default:
            icon = QString("icon-s-network-0");
            break;
    }
    return icon;
}

void PhoneNetworkContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    DuiLabel *headerLabel = new DuiLabel(trid("qtn_cell_phone_network", "Phone network"));
    headerLabel->setObjectName("connectivityLabel1");
    layoutPolicy->addItemAtPosition(headerLabel, 0, 0, 1, 2);
    layoutPolicy->addItemAtPosition(signalStrengthIcon, 1, 0, 1, 1);
    layoutPolicy->addItemAtPosition(operatorLabel, 1, 1, 1, 1);
    layoutPolicy->addItemAtPosition(toggleButton, 0, 2, 2, 1);
    layoutPolicy->setColumnStretchFactor(1, 1);
    centralWidget()->setLayout(layout);
}
