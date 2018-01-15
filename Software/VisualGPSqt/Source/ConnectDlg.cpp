/*
* MIT License
*
*  Copyright (c) 2018 VisualGPS, LLC
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*  SOFTWARE.
*
*/
#include "ConnectDlg.h"
#include "ui_ConnectDlg.h"

#include <QtSerialPort/QSerialPortInfo>

ConnectDlg::ConnectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDlg)
{
    ui->setupUi(this);

    //
    // Go check out all of the serial ports and only grab cobalt stuff
    //
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {

        QString str = QString("%1 - %2").arg(info.portName()).arg(info.description());
        ui->cbPortList->addItem(str, info.systemLocation());
    }

    //
    // Fill in the baud
    //
    ui->cbBaudList->addItem("4800", 4800);
    ui->cbBaudList->addItem("9600", 9600);
    ui->cbBaudList->addItem("19200", 19200);
    ui->cbBaudList->addItem("38400", 38400);
    ui->cbBaudList->addItem("57600", 57600);
    ui->cbBaudList->addItem("115200", 115200);
}

int ConnectDlg::GetConnectInfo(ConnectDlgInfo& ci) {
    int nResult = QDialog::Rejected;

    //
    // Set the previous baud
    //
    QString strBaud = QString("%1").arg(ci.m_nBaud);
    ui->cbBaudList->setCurrentIndex( ui->cbBaudList->findText(strBaud));

    //
    // Set the previous port
    //
    int nIndex = ui->cbPortList->findData(ci.m_strPortName);
    if(nIndex == -1) nIndex = 0;
    ui->cbPortList->setCurrentIndex(nIndex);

    nResult = exec();
    if( nResult == QDialog::Accepted ) {
        ci.m_strPortName = ui->cbPortList->currentData().toString();
        ci.m_nBaud = ui->cbBaudList->currentText().toInt();
    }
    return nResult;
}

ConnectDlg::~ConnectDlg()
{
    delete ui;
}
