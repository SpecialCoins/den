// Copyright (c) 2020 The BCZ developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/bcz/settings/settingsinformationwidget.h"
#include "qt/bcz/settings/forms/ui_settingsinformationwidget.h"
#include "clientmodel.h"
#include "rpcconsole.h"
#include "chainparams.h"
#include "db.h"
#include "util.h"
#include "guiutil.h"
#include "qt/bcz/qtutils.h"
#include <QDir>

SettingsInformationWidget::SettingsInformationWidget(BCZGUI* _window,QWidget *parent) :
    PWidget(_window,parent),
    ui(new Ui::SettingsInformationWidget)
{
    ui->setupUi(this);

    this->setStyleSheet(parent->styleSheet());

    // Containers
    setCssProperty(ui->left, "container");
    ui->left->setContentsMargins(10,10,10,10);
    setCssProperty({ui->layoutOptions1, ui->layoutOptions2, ui->layoutOptions3}, "container-options");

    // Title
    ui->labelTitle->setText(tr("Information"));
    setCssTitleScreen(ui->labelTitle);

    ui->labelTitleGeneral->setText(tr("General"));
    ui->labelTitleClient->setText(tr("Client Version: "));
    ui->labelTitleAgent->setText(tr("User Agent:"));
    ui->labelTitleBerkeley->setText(tr("BerkeleyDB version:"));
    ui->labelTitleDataDir->setText(tr("Datadir: "));
    ui->labelTitleTime->setText(tr("Startup time:  "));
    ui->labelTitleNetwork->setText(tr("Network"));
    ui->labelTitleName->setText(tr("Name:"));
    ui->labelTitleConnections->setText(tr("Connections:"));
    ui->labelTitlemnroi->setText(tr("Masternode ROI::"));
    ui->labelTitleMasternode->setText(tr("Masternodes:"));
    ui->labelTitleMemory->setText(tr("Memory Pool"));
    ui->labelTitleNumberTransactions->setText(tr("Current number of transactions:"));
    ui->labelTitleMem->setText(tr("Memory usage:"));


    setCssProperty({
        ui->labelTitleDataDir,
        ui->labelTitleBerkeley,
        ui->labelTitleAgent,
        ui->labelTitleClient,
        ui->labelTitleTime,
        ui->labelTitleName,
        ui->labelTitlemnroi,
        ui->labelTitleConnections,
        ui->labelTitleMasternode,
        ui->labelTitleNumberTransactions,
        ui->labelTitleMem,
        ui->labelTitleBlockNumber,
        ui->labelTitleBlockTime,
        ui->labelTitleNumberTransactions,
        ui->labelInfoNumberTransactions,
        ui->labelInfoMem,
        ui->labelInfoClient,
        ui->labelInfoAgent,
        ui->labelInfoBerkeley,
        ui->labelInfoDataDir,
        ui->labelInfoTime,
        ui->labelInfoConnections,
        ui->labelInfoMasternodeCount,
        ui->labelInfomnroi,
        ui->labelInfoBlockNumber
        }, "text-main-settings");

    setCssProperty({
        ui->labelTitleGeneral,
        ui->labelTitleNetwork,
        ui->labelTitleBlockchain,
        ui->labelTitleMemory,

    },"text-title");

    ui->labelTitleBlockchain->setText(tr("Blockchain"));
    ui->labelTitleBlockNumber->setText(tr("Current number of blocks:"));
    ui->labelTitleBlockTime->setText(tr("Last block time:"));

    // Information Network
    ui->labelInfoName->setText(tr("Main"));
    ui->labelInfoName->setProperty("cssClass", "text-main-settings");
    ui->labelInfoConnections->setText("0 (In: 0 / Out:0)");


    // Information Blockchain
    ui->labelInfoBlockNumber->setText("0");
    ui->labelInfoBlockTime->setText("Sept 6, 2018. Thursday, 8:21:49 PM");
    ui->labelInfoBlockTime->setProperty("cssClass", "text-main-grey");

    // Buttons
    ui->pushButtonFile->setText(tr("Wallet Conf"));
    ui->pushButtonNetworkMonitor->setText(tr("Network Monitor"));
    ui->pushButtonBackups->setText(tr("Backups"));
    setCssBtnSecondary(ui->pushButtonBackups);
    setCssBtnSecondary(ui->pushButtonFile);
    setCssBtnSecondary(ui->pushButtonNetworkMonitor);

    // Data
#ifdef ENABLE_WALLET
    // Wallet data -- remove it with if it's needed
    ui->labelInfoBerkeley->setText(DbEnv::version(0, 0, 0));
    ui->labelInfoDataDir->setText(QString::fromStdString(GetDataDir().string() + QDir::separator().toLatin1() + GetArg("-wallet", "wallet.dat")));
#else
    ui->labelInfoBerkeley->setText(tr("No information"));
#endif

    connect(ui->pushButtonBackups, &QPushButton::clicked, [this](){
        if (!GUIUtil::showBackups())
            inform(tr("Unable to open backups folder"));
    });
    connect(ui->pushButtonFile, &QPushButton::clicked, [this](){
        if (!GUIUtil::openConfigfile())
            inform(tr("Unable to open bcz.conf with default application"));
    });
    connect(ui->pushButtonNetworkMonitor, &QPushButton::clicked, this, &SettingsInformationWidget::openNetworkMonitor);
}


void SettingsInformationWidget::loadClientModel(){
    if (clientModel && clientModel->getPeerTableModel() && clientModel->getBanTableModel()) {
        // Provide initial values
        ui->labelInfoClient->setText(clientModel->formatFullVersion());
        ui->labelInfoAgent->setText(clientModel->clientName());
        ui->labelInfoTime->setText(clientModel->formatClientStartupTime());
        ui->labelInfoName->setText(QString::fromStdString(Params().NetworkIDString()));

        setNumConnections(clientModel->getNumConnections());
        connect(clientModel, &ClientModel::numConnectionsChanged, this, &SettingsInformationWidget::setNumConnections);

        setMasternodeCount(clientModel->getMasternodeCountString());
        connect(clientModel, SIGNAL(strMasternodesChanged(QString)), this, SLOT(setMasternodeCount(QString)));

        setmnroi(clientModel->getMasternodeCountString());
        connect(clientModel, SIGNAL(strMasternodesChanged(QString)), this, SLOT(setMasternodeCount(QString)));

        setNumBlocks(clientModel->getNumBlocks());
        connect(clientModel, &ClientModel::numBlocksChanged, this, &SettingsInformationWidget::setNumBlocks);

        updateTrafficStats(clientModel->getTotalBytesRecv(), clientModel->getTotalBytesSent());
        connect(clientModel, SIGNAL(bytesChanged(quint64, quint64)), this, SLOT(updateTrafficStats(quint64, quint64)));

        connect(clientModel, SIGNAL(mempoolSizeChanged(long,size_t)), this, SLOT(setMempoolSize(long,size_t)));
    }
}

void SettingsInformationWidget::setNumConnections(int count){
    if (!clientModel)
        return;

    QString connections = QString::number(count) + " (";
    connections += tr("In:") + " " + QString::number(clientModel->getNumConnections(CONNECTIONS_IN)) + " / ";
    connections += tr("Out:") + " " + QString::number(clientModel->getNumConnections(CONNECTIONS_OUT)) + ")";

    ui->labelInfoConnections->setText(connections);
}

void SettingsInformationWidget::setMasternodeCount(const QString& strMasternodes)
{
    ui->labelInfoMasternodeCount->setText(strMasternodes);
}

void SettingsInformationWidget::setmnroi(const QString& strMasternodes)
{
    ui->labelInfomnroi->setText(strMasternodes);
}

void SettingsInformationWidget::updateTrafficStats(quint64 totalBytesIn, quint64 totalBytesOut)
{
    //ui->lblBytesIn->setText(FormatBytes(totalBytesIn));
    //ui->lblBytesOut->setText(FormatBytes(totalBytesOut));
}


void SettingsInformationWidget::setMempoolSize(long numberOfTxs, size_t dynUsage)
{
    ui->labelInfoNumberTransactions->setText(QString::number(numberOfTxs));

    if (dynUsage < 1000000)
        ui->labelInfoMem->setText(QString::number(dynUsage/1000.0, 'f', 2) + " KB");
    else
        ui->labelInfoMem->setText(QString::number(dynUsage/1000000.0, 'f', 2) + " MB");
}

void SettingsInformationWidget::setNumBlocks(int count){
    ui->labelInfoBlockNumber->setText(QString::number(count));
    if (clientModel)
        ui->labelInfoBlockTime->setText(clientModel->getLastBlockDate().toString());
}

void SettingsInformationWidget::openNetworkMonitor(){
    if(!rpcConsole){
        rpcConsole = new RPCConsole(0);
        rpcConsole->setClientModel(clientModel);
    }
    rpcConsole->showNetwork();
}

SettingsInformationWidget::~SettingsInformationWidget(){
    delete ui;
}
