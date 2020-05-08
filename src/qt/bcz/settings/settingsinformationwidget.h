// Copyright (c) 2020 The BCZ developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SETTINGSINFORMATIONWIDGET_H
#define SETTINGSINFORMATIONWIDGET_H

#include <QWidget>
#include "qt/bcz/pwidget.h"
#include "rpcconsole.h"

namespace Ui {
class SettingsInformationWidget;
}

class SettingsInformationWidget : public PWidget
{
    Q_OBJECT

public:
    explicit SettingsInformationWidget(BCZGUI* _window, QWidget *parent = nullptr);
    ~SettingsInformationWidget();

    void loadClientModel() override;

private Q_SLOTS:
    void setNumConnections(int count);
    void setMasternodeCount(const QString& strMasternodes);
    void setmnroiCount(const QString& strMasternodes);
    void setMempoolSize(long numberOfTxs, size_t dynUsage);
    void setNumBlocks(int count);
    void openNetworkMonitor();
    void updateTrafficStats(quint64 totalBytesIn, quint64 totalBytesOut);

private:
    Ui::SettingsInformationWidget *ui;
    RPCConsole *rpcConsole = nullptr;
};

#endif // SETTINGSINFORMATIONWIDGET_H
