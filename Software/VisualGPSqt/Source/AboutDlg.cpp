///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2014 VisualGPS, LLC.
// All rights reserved.
//
// Author: Monte Variakojis
//
// VisualGPS, LLC. licenses this source code for use within your application in
// object form. This source code is not to be distributed in any way without
// prior written permission from VisualGPS, LLC..
//
///////////////////////////////////////////////////////////////////////////////
#include "AboutDlg.h"
#include "ui_AboutDlg.h"

CAboutDlg::CAboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAboutDlg)
{
    ui->setupUi(this);
}

CAboutDlg::~CAboutDlg()
{
    delete ui;
}
