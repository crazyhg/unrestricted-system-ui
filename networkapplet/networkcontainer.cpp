#include "networkcontainer.h"
#include "networklist.h"
#include "networktranslation.h"

#include <DuiComboBox>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiList>
#include <DuiWidgetListModel>

#include <QDebug>

NetworkContainer::NetworkContainer(DuiWidget *parent) :
        DuiContainer(parent),        
        modeComboBox(new DuiComboBox(this)),
        selectionComboBox(new DuiComboBox(this)),
        infoLabel(NULL),
        modeLabel(new DuiLabel(DcpNetwork::NetworkModeText, this)),
        selectionLabel(new DuiLabel(DcpNetwork::NetworkSelectionText, this)),
        selectionDefaultIndex(-1),
        networkList(new NetworkList(parent))
{
    toggleComboBoxSignalConnection(modeComboBox);
    toggleComboBoxSignalConnection(selectionComboBox);
    connect(networkList, SIGNAL(availableNetworkSelected(QString)), this, SIGNAL(availableNetworkSelected(QString)));
    connect(this, SIGNAL(headerClicked()), this, SLOT(toggleExpand()));
    setLayout();
}

NetworkContainer::~NetworkContainer()
{
}

void NetworkContainer::setLayout()
{
    // create the layout for combo boxes
    DuiLayout *cbLayout = new DuiLayout();

    DuiGridLayoutPolicy *cbLandscapeLayoutPolicy = new DuiGridLayoutPolicy(cbLayout);
    cbLandscapeLayoutPolicy->addItemAtPosition(modeLabel, 0, 0);
    cbLandscapeLayoutPolicy->addItemAtPosition(selectionLabel, 0, 1);
    cbLandscapeLayoutPolicy->addItemAtPosition(modeComboBox, 1, 0);
    cbLandscapeLayoutPolicy->addItemAtPosition(selectionComboBox, 1, 1);
    cbLayout->setLandscapePolicy(cbLandscapeLayoutPolicy);

    DuiLinearLayoutPolicy *cbPortraitLayoutPolicy = new DuiLinearLayoutPolicy(cbLayout, Qt::Vertical);
    cbPortraitLayoutPolicy->addItem(modeLabel, Qt::AlignLeft);
    cbPortraitLayoutPolicy->addItem(modeComboBox, Qt::AlignLeft);
    cbPortraitLayoutPolicy->addItem(selectionLabel, Qt::AlignLeft);    
    cbPortraitLayoutPolicy->addItem(selectionComboBox, Qt::AlignLeft);
    cbLayout->setPortraitPolicy(cbPortraitLayoutPolicy);

    // create the main layout
    DuiLayout *layout = new DuiLayout();
    layoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->addItem(cbLayout, Qt::AlignLeft);

    centralWidget()->setLayout(layout);
}

void NetworkContainer::initModeComboBox(int selected, const QStringList &values)
{
    qDebug() << "NetworkContainer::initModeComboBox("<< values.size() <<")";
    initComboBox(modeComboBox, selected, values);
}    

void NetworkContainer::initSelectionComboBox(int defaultIndex, int selected, const QStringList &values)
{
    selectionDefaultIndex = defaultIndex;
    initComboBox(selectionComboBox, selected, values);
}

void NetworkContainer::initComboBox(DuiComboBox *cb, int selected, const QStringList &values)
{
    cb->addItems(values);
    if (selected != -1) {
        toggleComboBoxSignalConnection(cb, false);
        cb->setCurrentIndex(selected);
        toggleComboBoxSignalConnection(cb);
    }
}

void NetworkContainer::toggleComboBoxSignalConnection(DuiComboBox *cb, bool toggle)
{
    if(toggle)
        connect(cb, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(networkModeChanged(QString)));
    else
        disconnect(cb, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(networkModeChanged(QString)));
}

void NetworkContainer::toggleAvailableNetworks(int selected, const QStringList &networks, bool toggle)
{
    qDebug() << "NetworkWidget::toggleAvailableNetworks(" << selected << ", " << networks << ", " << toggle << ")";
    if(toggle) {
        if(infoLabel == NULL)
            infoLabel = new DuiLabel(this); //create here to prevent the label from
        if(networkList->insertNetworks(selected, networks)) {
            infoLabel->setText(DcpNetwork::AvailableNetworksText);
            if(layoutPolicy->indexOf(infoLabel) == -1)
                layoutPolicy->addItem(infoLabel, Qt::AlignLeft);
            if(layoutPolicy->indexOf(networkList) == -1)
                layoutPolicy->addItem(networkList, Qt::AlignLeft);            
        }
        else {
            infoLabel->setText(DcpNetwork::NoAvailableNetworksText);
            if(layoutPolicy->indexOf(infoLabel) == -1)
                layoutPolicy->addItem(infoLabel, Qt::AlignLeft);
        }
    }
    else {
        if(infoLabel == NULL)
            layoutPolicy->removeItem(infoLabel);
        layoutPolicy->removeItem(networkList);
    }
}

void NetworkContainer::setDefaultSelection()
{
    selectionComboBox->setCurrentIndex(selectionDefaultIndex);
}