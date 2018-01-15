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
#ifndef LEDWND_H
#define LEDWND_H

#include <QWidget>

class CLEDWnd : public QWidget
{
    enum LED_STATE {
        LED_STATE_OFF = 0,
        LED_STATE_ON = 1
    };

    Q_OBJECT

private:
    QColor      m_crOnColor;                ///< On color
    QColor      m_crOffColor;               ///< Off color
    LED_STATE   m_nLEDState;                ///< Led state on or off, default off
    int         m_nPingCascadeCount;        ///< Cascase level

public:
    explicit CLEDWnd(QWidget *parent = 0);
    QSize sizeHint() const;

    ///
    /// \brief SetColor - Sets the LED on and off color
    /// \param crOn QColor On color
    /// \param crOff QColor off color
    ///
    void SetColor(QColor crOn, QColor crOff);

    ///
    /// \brief Ping - Use Ping to momentarily enable (turn on) the LED.
    ///
    /// The Ping method will turn on the led for mSec milliseconds.
    ///
    /// \param mSec - int - number of milliseconds to enable the LED
    ///
    void Ping(int mSec = 1000);

    ///
    /// \brief Enable - Manually enables (turn on) the LED
    ///
    void Enable();
signals:

public slots:

    ///
    /// \brief Disable - Manually disables (turn off) the LED
    ///
    void Disable();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // LEDWND_H
