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
#include "AzimuthElevationWnd.h"

CAzimuthElevationWnd::CAzimuthElevationWnd(CNMEAParserQt *pNMEAParser, QWidget *parent) :
    QWidget(parent),
    m_pNMEAParser(pNMEAParser)
{

    connect(pNMEAParser, SIGNAL(NewPositionUpdateGPS()), SLOT(OnNewPositionUpdateGPS()));
}

void CAzimuthElevationWnd::OnNewPositionUpdateGPS()
{
    repaint();
}

void CAzimuthElevationWnd::paintEvent(QPaintEvent */*event*/){

    DrawScreen();

}

void CAzimuthElevationWnd::DrawScreen(){
    QPainter painter(this);
    QPalette pal;
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true );
    painter.setRenderHint(QPainter::TextAntialiasing, true );

    //
    // Get GPS information
    //
    std::map <int, SAT_INFO_T> mapSatData;
    ConsolidateSatData(mapSatData);

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

    //
    // Erase background
    //
    painter.setPen(QPalette::WindowText);
    painter.setBrush(pal.color(QPalette::Base));
    painter.drawEllipse(rcDraw);

    QRectF rc45 = rcDraw;
    rc45.adjust(dSmallSide/4, dSmallSide/4, -dSmallSide/4, -dSmallSide/4);
    painter.drawEllipse(rc45);

    painter.drawLine(rcDraw.left() + dSmallSide / 2, rcDraw.top(), rcDraw.left() + dSmallSide / 2, rcDraw.bottom());
    painter.drawLine(rcDraw.left(), rcDraw.top() + dSmallSide / 2, rcDraw.right(), rcDraw.top() + dSmallSide / 2);

    qreal dCenterX = rcDraw.left() + dSmallSide / 2.0;
    qreal dCenterY = rcDraw.top() + dSmallSide / 2.0;

    QImage imgSatEnabled;
    if( imgSatEnabled.load(":/Images/Resources/satEnabled.png") == false) {
        qDebug() << "Could load satDisabled image";
    }
    QImage imgSatDisabled;
    if( imgSatDisabled.load(":/Images/Resources/satDisabled.png") == false) {
        qDebug() << "Could load imgSatDisabled image";
    }

    int nSize = (int)( dSmallSide * 0.045 );
    QFont Font("Helvetica [Cronyx]", nSize);
    painter.setFont(Font);
    QFontMetrics fontMetrics(Font);

    for (std::map<int, SAT_INFO_T>::iterator it=mapSatData.begin(); it!=mapSatData.end(); ++it) {
        SAT_INFO_T satInfo = it->second;

        if(satInfo.nPRN != 0 && (int)satInfo.nAzimuth != 0 && (int)satInfo.nElevation != 0){
            QPoint pt = AEToPoint(dCenterX, dCenterY, (int)satInfo.nAzimuth, satInfo.nElevation, rcDraw);
            qreal dScale = dSmallSide * 0.08;
            QRectF rcImage(pt.x()-dScale/2, pt.y()-dScale/2, dScale, dScale);

            if(satInfo.bUsedForNav == true) {
                painter.drawImage(rcImage, imgSatEnabled);
            } else {
                painter.drawImage(rcImage, imgSatDisabled);
            }
            rcImage.adjust(0, 0, 0, fontMetrics.height());
            QString str = QString("%1").arg(satInfo.nPRN);
            painter.drawText(rcImage, str, Qt::AlignBottom | Qt::AlignHCenter);
        }
    }
}

QSize CAzimuthElevationWnd::sizeHint() const {
    return QSize(200,200);
}

QPoint CAzimuthElevationWnd::AEToPoint(qreal dCenterX, qreal dCenterY, int nAzimuth, int nElevation, QRectF& DrawRect)
{
    qreal x,y;
    int px,py;

    int cx = (int)(dCenterX + 0.5);
    int cy = (int)(dCenterY + 0.5);

    qreal dRad = ((qreal)nAzimuth / 180.0 * 3.14159265359);
    x = sin(dRad); y = -cos(dRad);

    qreal dRat = (DrawRect.width()/2.0) / 90.0;

    px = cx + (int)( x * (90.0 - (qreal)nElevation) * dRat );
    py = cy + (int)( y * (90.0 - (qreal)nElevation) * dRat );

    return(QPoint(px,py));
}

void CAzimuthElevationWnd::ConsolidateSatData(std::map <int, SAT_INFO_T>&mapSatData) {

    mapSatData.clear();

    //
    // Get GPS information
    //
    // GPS
    CNMEAParserData::GSA_DATA_T gpgsaData;
    CNMEAParserData::GSV_DATA_T gpgsvData;
    m_pNMEAParser->GetGPGSA(gpgsaData);
    m_pNMEAParser->GetGPGSV(gpgsvData);

    // GN
    CNMEAParserData::GSA_DATA_T gngsaData;
    m_pNMEAParser->GetGNGSA(gngsaData);

    // GLONASS
    CNMEAParserData::GSA_DATA_T glgsaData;
    CNMEAParserData::GSV_DATA_T glgsvData;
    m_pNMEAParser->GetGLGSA(glgsaData);
    m_pNMEAParser->GetGLGSV(glgsvData);

    // GPS
    SAT_INFO_T satInfo;
    for(int i = 0; i < CNMEAParserData::c_nMaxGSASats; i++) {
        if(gpgsaData.pnPRN[i] != CNMEAParserData::c_nInvlidPRN) {
            memset(&satInfo, 0, sizeof(satInfo));
            satInfo.nPRN = gpgsaData.pnPRN[i];
            satInfo.nSNR = 0;
            satInfo.bUsedForNav = true;
            mapSatData[satInfo.nPRN] = satInfo;
        }
    }

    // GLONASS
    for(int i = 0; i < CNMEAParserData::c_nMaxGSASats; i++) {
        if(glgsaData.pnPRN[i] != CNMEAParserData::c_nInvlidPRN) {
            memset(&satInfo, 0, sizeof(satInfo));
            satInfo.nPRN = glgsaData.pnPRN[i];
            satInfo.nSNR = 0;
            satInfo.bUsedForNav = true;
            mapSatData[satInfo.nPRN] = satInfo;
        }
    }
    // GN
    for(int i = 0; i < CNMEAParserData::c_nMaxGSASats; i++) {
        if(gngsaData.pnPRN[i] != CNMEAParserData::c_nInvlidPRN) {
            memset(&satInfo, 0, sizeof(satInfo));
            satInfo.nPRN = gngsaData.pnPRN[i];
            satInfo.nSNR = 0;
            satInfo.bUsedForNav = true;
            mapSatData[satInfo.nPRN] = satInfo;
        }
    }


    for(int i = 0; i < CNMEAParserData::c_nMaxConstellation; i++) {
        // GPS
        if(gpgsvData.SatInfo[i].nPRN != CNMEAParserData::c_nInvlidPRN) {
            mapSatData[gpgsvData.SatInfo[i].nPRN].nSNR = gpgsvData.SatInfo[i].nSNR;
            mapSatData[gpgsvData.SatInfo[i].nPRN].nPRN = gpgsvData.SatInfo[i].nPRN;
            mapSatData[gpgsvData.SatInfo[i].nPRN].nAzimuth = (int)gpgsvData.SatInfo[i].dAzimuth;
            mapSatData[gpgsvData.SatInfo[i].nPRN].nElevation = (int)gpgsvData.SatInfo[i].dElevation;
        }
        // GLONASS
        if(glgsvData.SatInfo[i].nPRN != CNMEAParserData::c_nInvlidPRN) {
            mapSatData[glgsvData.SatInfo[i].nPRN].nSNR = glgsvData.SatInfo[i].nSNR;
            mapSatData[glgsvData.SatInfo[i].nPRN].nPRN = glgsvData.SatInfo[i].nPRN;
            mapSatData[glgsvData.SatInfo[i].nPRN].nAzimuth = (int)glgsvData.SatInfo[i].dAzimuth;
            mapSatData[glgsvData.SatInfo[i].nPRN].nElevation = (int)glgsvData.SatInfo[i].dElevation;
        }
    }
}

