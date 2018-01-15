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
#include "GPSStatusWnd.h"
#include <QPushButton>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>

CGPSStatusWnd::CGPSStatusWnd(CNMEAParserQt *pNMEAParser, QWidget *parent) :
    QWidget(parent),
    m_pNMEAParser(pNMEAParser)
{
    CreateWidgets();
}

void CGPSStatusWnd::CreateWidgets() {

    m_pSignalQualityWnd = new CSignalQuality(m_pNMEAParser, this);
    m_pPositionStatusWnd = new CPositionStatusWnd(m_pNMEAParser, this);
    m_pAzimuthElevationWnd = new CAzimuthElevationWnd(m_pNMEAParser, this);

    QSplitter *pBottomGroupSplit = new QSplitter(Qt::Horizontal, this);
    pBottomGroupSplit->addWidget(m_pPositionStatusWnd);
    pBottomGroupSplit->addWidget(m_pAzimuthElevationWnd);

    QSplitter *pTopGroupSplit = new QSplitter(Qt::Vertical, this);
    pTopGroupSplit->addWidget(m_pSignalQualityWnd);
    pTopGroupSplit->addWidget(pBottomGroupSplit);

    QVBoxLayout *pVertLayout = new QVBoxLayout(this);
    pVertLayout->addWidget(pTopGroupSplit);
    setLayout(pVertLayout);
}
