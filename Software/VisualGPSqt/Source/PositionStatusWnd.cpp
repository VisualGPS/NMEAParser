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
#include <QtGui>
#include "PositionStatusWnd.h"
#include "NMEAParserQt.h"

CPositionStatusWnd::CPositionStatusWnd(CNMEAParserQt *pNMEAParser, QWidget *parent) :
    QWidget(parent),
    m_pNMEAParser(pNMEAParser)
{
    connect(pNMEAParser, SIGNAL(NewPositionUpdateGPS()), SLOT(OnNewPositionUpdateGPS()));
}

void CPositionStatusWnd::paintEvent(QPaintEvent */*event*/){
    DrawScreen();

}void CPositionStatusWnd::DrawScreen(){
    QPainter painter(this);
    QPalette pal;
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true );

    //
    // Scale the text based on width
    int nSize = (int)( width() * 0.06 );
    QFont Font("Helvetica [Cronyx]", nSize);
    painter.setFont(Font);
    QFontMetrics fontMetrics(Font);

    painter.setPen(QPalette::WindowText);
    painter.setBrush(pal.color(QPalette::Base));

    QRectF rcDraw(0, 0, width(), height());

    //
    // Draw up position status
    //
    // Get GGA data (has the position)
    //
    CNMEAParserData::GGA_DATA_T ggaData;
    m_pNMEAParser->GetGPGGA(ggaData);
    // If no position information in the GPGGA sentence, then try the GNGGA..
    if(ggaData.m_dLatitude == 0.0 && ggaData.m_dLongitude == 0.0 && ggaData.m_dLatitude == 0.0) {
        m_pNMEAParser->GetGNGGA(ggaData);
    }
    // Again, no information, try Galileo
    else if(ggaData.m_dLatitude == 0.0 && ggaData.m_dLongitude == 0.0 && ggaData.m_dLatitude == 0.0) {
        m_pNMEAParser->GetGAGGA(ggaData);
    }

    CNMEAParserData::RMC_DATA_T rmcData;
    m_pNMEAParser->GetGPRMC(rmcData);
    if(rmcData.m_dLatitude == 0.0 && rmcData.m_dLongitude == 0.0 && rmcData.m_dLatitude == 0.0) {
        m_pNMEAParser->GetGNRMC(rmcData);
    }

    QString str =
            "Latitude:  \n"
            "Longitude: \n"
            "Altitude:  \n"
            "Speed:     \n"
            ;
    painter.drawText(rcDraw, str);
    QRectF rcTextBounds = painter.boundingRect(rcDraw, Qt::AlignLeft, str);

    str = QString(
            "%1 \n"
            "%2\n"
            "%3\n"
            "%4 Kts"
            ).arg(ggaData.m_dLatitude)
            .arg(ggaData.m_dLongitude)
            .arg(ggaData.m_dAltitudeMSL)
            .arg(rmcData.m_dSpeedKnots)
            ;
    rcTextBounds.setLeft(rcTextBounds.right() + fontMetrics.averageCharWidth());
    rcTextBounds.setRight(rcDraw.right());
    painter.drawText(rcTextBounds, str);
}

QSize CPositionStatusWnd::sizeHint() const {
    return QSize(200,200);
}

void CPositionStatusWnd::OnNewPositionUpdateGPS() {
    repaint();
}
