// Copyright (c) 2020 The BCZ developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINCONTROLBCZWIDGET_H
#define COINCONTROLBCZWIDGET_H

#include <QDialog>

namespace Ui {
class CoinControlBczWidget;
}

class CoinControlBczWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CoinControlBczWidget(QWidget *parent = nullptr);
    ~CoinControlBczWidget();

private:
    Ui::CoinControlBczWidget *ui;
};

#endif // COINCONTROLBCZWIDGET_H
