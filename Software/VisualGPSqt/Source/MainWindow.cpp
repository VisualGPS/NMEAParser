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
#include <QtCore>
#include <QPushButton>
#include <QHBoxLayout>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ConnectDlg.h"
#include "AboutDlg.h"
#include "GPSStatusWnd.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_NMEAParser(parent),
    m_AppSettings("VisualGPSLLC", "VisualGPSqt"),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("VisualGPSLLC");
    QCoreApplication::setOrganizationDomain("VisualGPS.com");
    QCoreApplication::setApplicationName("VisualGPSqt");

    CreateWidgets();

    //
    // Create LEDs in the status bar
    //
    // Tx
    m_pTxLed = new CLEDWnd(this);
    m_pTxLed->SetColor(QColor(255, 0, 0), QColor(128, 0, 0));
    ui->statusBar->addPermanentWidget(m_pTxLed);
    // Rx
    m_pRxLed = new CLEDWnd(this);
    m_pRxLed->SetColor(QColor(0, 255, 0), QColor(0, 128, 0));
    ui->statusBar->addPermanentWidget(m_pRxLed);

    connect(&m_NMEAParser, SIGNAL(MessageProcessed()), SLOT(OnNMEAMessageProcessed()) );

    ui->statusBar->showMessage("Ready.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Connect_triggered()
{
    int baud = m_AppSettings.value("settings/baud", 4800).toInt();
    QString port = m_AppSettings.value("settings/port", "COM1").toString();

    ConnectDlg dlg(this);
    dlg.setModal(true);
    ConnectDlgInfo ci;
    ci.m_nBaud = baud;
    ci.m_strPortName = port;
    if( dlg.GetConnectInfo(ci) == QDialog::Accepted) {
        qDebug() << "Port:" << ci.m_strPortName;
        qDebug() << "Baud:" << ci.m_nBaud;
        m_NMEAParser.Connect(ci.m_strPortName, ci.m_nBaud);

        m_AppSettings.setValue("settings/baud", ci.m_nBaud);
        m_AppSettings.setValue("settings/port", ci.m_strPortName);
    }
}

void MainWindow::on_action_About_triggered()
{
    CAboutDlg dlg;
    dlg.setModal(true);
    dlg.exec();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::CreateWidgets(){

    m_pTabMain = new QTabWidget(this);
    setCentralWidget(m_pTabMain);

    CGPSStatusWnd *pStatusWnd = new CGPSStatusWnd(&m_NMEAParser); // do not add parent
    m_pTabMain->addTab(pStatusWnd, "GPS Status");

    //QVBoxLayout *pTabLayout = new QVBoxLayout(this);
    //m_pTabMain->setLayout(this);
}

void MainWindow::OnNMEAMessageProcessed(){
    m_pRxLed->Ping(200);
}
