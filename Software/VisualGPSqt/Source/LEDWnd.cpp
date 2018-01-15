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
#include "LEDWnd.h"

CLEDWnd::CLEDWnd(QWidget *parent) :
    QWidget(parent),
    m_crOnColor(QColor(0, 255, 0)),
    m_crOffColor(QColor(0, 128, 0)),
    m_nLEDState(LED_STATE_OFF),
    m_nPingCascadeCount(0)
{
}

void CLEDWnd::paintEvent(QPaintEvent */*event*/){
    QPainter painter(this);

    //painter.setPen(Qt::NoPen);
    switch(m_nLEDState) {
    case LED_STATE_ON : painter.setBrush(m_crOnColor); break;
    case LED_STATE_OFF : default : painter.setBrush(m_crOffColor); break;
    }

    //
    // Create the draw rect area and center it
    //
    qreal dSmallSide = (qreal)qMin(width(), height()) - 1;
    QRectF rcDraw(dSmallSide, dSmallSide, dSmallSide, dSmallSide);
    if(width() > height()) {
        // center horizontally
        rcDraw.moveTo(width()/2 - dSmallSide/2, 0);
    } else {
        rcDraw.moveTo(0,height()/2 - dSmallSide/2);
    }

    painter.drawEllipse(rcDraw);

}

QSize CLEDWnd::sizeHint() const {
    return QSize(16, 16);
}

void CLEDWnd::SetColor(QColor crOn, QColor crOff) {
    m_crOnColor = crOn;
    m_crOffColor = crOff;
    repaint();
}

void CLEDWnd::Ping(int mSec/*=1000*/) {
    Enable();
    m_nPingCascadeCount++;
    QTimer::singleShot(mSec, this, SLOT(Disable()));
    repaint();
}

void CLEDWnd::Disable() {
    m_nPingCascadeCount--;
    if(m_nPingCascadeCount <= 0) {
        m_nLEDState = CLEDWnd::LED_STATE_OFF;
    }
    repaint();
}

void CLEDWnd::Enable() {
    m_nLEDState = CLEDWnd::LED_STATE_ON;
    repaint();
}
