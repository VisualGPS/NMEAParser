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
#ifndef AZIMUTHELEVATIONWND_H
#define AZIMUTHELEVATIONWND_H

#include <QWidget>
#include "NMEAParserQt.h"

class CAzimuthElevationWnd : public QWidget
{
private:
    Q_OBJECT
    CNMEAParserQt *             m_pNMEAParser;              ///< The main NMEA parser

    typedef struct _SAT_INFO_T {
        int     nPRN;                                                           ///< Satellite pseudo random number (ID)
        int     nSNR;                                                           ///< Signal to noise ratio
        int     nAzimuth;                                                       ///< Azimuth
        int     nElevation;                                                     ///< Elevation
        bool    bUsedForNav;                                                    ///< Used in nav/timing solution
    } SAT_INFO_T;

public:
    explicit CAzimuthElevationWnd(CNMEAParserQt *pNMEAParser, QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const;

signals:

public slots:

protected slots:
    void OnNewPositionUpdateGPS();

protected:
    void DrawScreen();
    QPoint AEToPoint(qreal dCenterX, qreal dCenterY, int nAzimuth, int nElevation, QRectF &DrawRect);

    ///
    /// \brief ConsolidateSatData This method will make an attempt to consolidate multiple GNSS systems into a single stucture
    /// \param mapSatData Reference to a satellate info map
    ///
    void ConsolidateSatData(std::map <int, SAT_INFO_T>&mapSatData);

};

#endif // AZIMUTHELEVATIONWND_H
