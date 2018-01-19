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
#include <map>
#include "SignalQuality.h"

CSignalQuality::CSignalQuality(CNMEAParserQt *pNMEAParser, QWidget *parent) :
    QWidget(parent),
    m_pNMEAParser(pNMEAParser)
{
    connect(pNMEAParser, SIGNAL(NewPositionUpdateGPS()), SLOT(OnNewPositionUpdateGPS()));
}

void CSignalQuality::OnNewPositionUpdateGPS()
{
    repaint();
}

void CSignalQuality::paintEvent(QPaintEvent */*event*/){
    DrawScreen();
}

void CSignalQuality::ConsolidateSatData(std::map <int, SAT_INFO_T>&mapSatData) {

    mapSatData.clear();

    //
    // Get GPS information
    //
    CNMEAParserData::GSA_DATA_T gpgsaData;
    CNMEAParserData::GSV_DATA_T gpgsvData;
    m_pNMEAParser->GetGPGSA(gpgsaData);
    m_pNMEAParser->GetGPGSV(gpgsvData);

    CNMEAParserData::GSA_DATA_T gngsaData;
    m_pNMEAParser->GetGNGSA(gngsaData);

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
    for(int i = 0; i < CNMEAParserData::c_nMaxConstellation; i++) {
        if(glgsaData.pnPRN[i] != CNMEAParserData::c_nInvlidPRN) {
            memset(&satInfo, 0, sizeof(satInfo));
            satInfo.nPRN = glgsaData.pnPRN[i];
            satInfo.nSNR = 0;
            satInfo.bUsedForNav = true;
            mapSatData[satInfo.nPRN] = satInfo;
        }
    }
    // GN
    for(int i = 0; i < CNMEAParserData::c_nMaxConstellation; i++) {
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
        }
        // GLONASS
        if(glgsvData.SatInfo[i].nPRN != CNMEAParserData::c_nInvlidPRN) {
            mapSatData[glgsvData.SatInfo[i].nPRN].nSNR = glgsvData.SatInfo[i].nSNR;
            mapSatData[glgsvData.SatInfo[i].nPRN].nPRN = glgsvData.SatInfo[i].nPRN;
        }
    }
}

void CSignalQuality::DrawScreen() {

    QPainter painter(this);
    QPalette pal;

    std::map <int, SAT_INFO_T> mapSatData;
    ConsolidateSatData(mapSatData);

    //
    // Erase background
    //
    painter.setPen(Qt::NoPen);
    painter.setBrush(pal.color(QPalette::Dark));
    painter.drawRect(0, 0, width(), height());

    int nTotalSats = (int)mapSatData.size();

    //
    // Creaate used in nav database. Use PRN as index
    //
    //bool bUsedForNavagation[c_nMaxChannels];
    std::map <int, bool> bUsedForNavagation;

    //
    // Figure out signal quality bar width
    //
    qreal dWidth = 0;
    if(nTotalSats > 0) {
        dWidth = (qreal)width() / (qreal)nTotalSats;
    }

    //
    // Create font - size it to the sig qual bar width
    //
    int nSize = (int)( dWidth * 0.3 );
    if(nSize > 20) nSize = 20;
    QFont Font("Helvetica [Cronyx]", nSize);
    painter.setFont(Font);

    //
    // Create the background sig qual rectangle. Leave room at bottom for
    // PRN
    //
    QFontMetrics fontMetrics(Font);
    qreal dPRNTextBoxHeight = (qreal)fontMetrics.height() * 1.25;
    QRectF rect(0, 0, dWidth, height() - dPRNTextBoxHeight);
    qreal x = 0;

    for (std::map<int, SAT_INFO_T>::iterator it=mapSatData.begin(); it!=mapSatData.end(); ++it) {
        SAT_INFO_T satInfo = it->second;
        //
        // Draw background of signal quality
        //
        painter.setPen(pal.color(QPalette::Dark));
        painter.setBrush(pal.color(QPalette::Light));
        painter.drawRect(rect);

        //CNMEAParserData::SAT_INFO_T satInfo = GetGSVRecordFromPRN(nPRN, CNMEAParserData::GSV_DATA_T& gsvData)

        //
        // Draw actual signal strength
        //
        // Setup color
        if( satInfo.bUsedForNav == true) {
            if( satInfo.nPRN >= NP_GPS_MIN_PRN && satInfo.nPRN <= NP_WAAS_MAX_PRN) {
                painter.setBrush(pal.color(QPalette::Highlight));
            }
            else if( satInfo.nPRN >= NP_GLONASS_MIN_PRN && satInfo.nPRN <= NP_GLONASS_MAX_PRN){
                painter.setBrush(QColor(128, 0, 0));
            }
        } else {

            if( satInfo.nPRN >= NP_WAAS_MIN_PRN && satInfo.nPRN <= NP_WAAS_MAX_PRN) {
                painter.setBrush(QColor(0, 128, 0));
            }
            else {
                painter.setBrush(pal.color(QPalette::Shadow));
            }
        }

        qreal dRatio = (qreal)rect.height() / 50.0;
        QRectF rcSignal = rect;
        qreal dSigQual = (qreal)satInfo.nSNR;

        qreal dHeight = dSigQual * dRatio;
        rcSignal.setHeight(dHeight);
        rcSignal.moveTop(rect.bottom() - dHeight);

        painter.drawRect(rcSignal);

        //
        // Draw text
        //
        // First the signal strength
        //
        painter.setPen(pal.color(QPalette::HighlightedText));
        QString str = QString("%1").arg(dSigQual);
        painter.drawText(rcSignal, str, Qt::AlignTop | Qt::AlignHCenter);
        //
        // Now draw the PRN
        //
        QRect rcPRNText(rcSignal.left(), rect.bottom(), rcSignal.width(), dPRNTextBoxHeight);
        painter.setPen(pal.color(QPalette::BrightText));
        str = QString("%1").arg(satInfo.nPRN);
        painter.drawText(rcPRNText, str, Qt::AlignVCenter | Qt::AlignHCenter);

        //
        // Move to next channel
        //
        x += dWidth;
        rect.moveTo(x, 0.0);
    }
}

QSize CSignalQuality::sizeHint () const{
    return QSize(200, 200);
}

CNMEAParserData::SAT_INFO_T CSignalQuality::GetGSVRecordFromPRN(int nPRN, CNMEAParserData::GSV_DATA_T& gsvData) {
    CNMEAParserData::SAT_INFO_T satInfo;
    memset(&satInfo, 0 , sizeof(CNMEAParserData::SAT_INFO_T));
    for(int i = 0; i < CNMEAParserData::c_nMaxConstellation; i++) {
        if(nPRN == gsvData.SatInfo[i].nPRN) {
            return gsvData.SatInfo[i];
        }
    }

    return satInfo;
}

