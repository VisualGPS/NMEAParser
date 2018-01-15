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
#ifndef SIGNALQUALITY_H
#define SIGNALQUALITY_H

#include <QWidget>
#include "NMEAParserQt.h"

class CSignalQuality : public QWidget
{
    Q_OBJECT

private:
    typedef struct _SAT_INFO_T {
        int     nPRN;                                                           ///< Satellite pseudo random number (ID)
        int     nSNR;                                                           ///< Signal to noise ratio
        bool    bUsedForNav;                                                    ///< Used in nav/timing solution
    } SAT_INFO_T;

    const static int            c_nMaxChannels = 256;       ///< Max number of channels this class can handle
    CNMEAParserQt *             m_pNMEAParser;              ///< The main NMEA parser

public:
    explicit CSignalQuality(CNMEAParserQt *pNMEAParser, QWidget *parent = 0);

signals:

public slots:
    void OnNewPositionUpdateGPS();
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    void ConsolidateSatData(std::map<int, SAT_INFO_T> &mapSatData);
    void DrawScreen();
    CNMEAParserData::SAT_INFO_T GetGSVRecordFromPRN(int nPRN, CNMEAParserData::GSV_DATA_T &gsvData);
};

#endif // SIGNALQUALITY_H
