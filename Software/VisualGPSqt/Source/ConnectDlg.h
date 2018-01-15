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
#ifndef CONNECTDLG_H
#define CONNECTDLG_H

#include <QDialog>

namespace Ui {
class ConnectDlg;
}

class ConnectDlgInfo {
public:
    QString     m_strPortName;                      ///< Port name
    quint32     m_nBaud;                            ///< Baud rate
};

class ConnectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDlg(QWidget *parent = 0);
    ~ConnectDlg();

    ///
    /// \brief GetConnectInfo
    /// \param ci - Reference to ConnectDlgInfo class. If QDialog::Accepted is
    /// returned, then the ci object is filled with data
    /// \return QDialog::Accepted or QDialog::Rejected
    ///
    int GetConnectInfo(ConnectDlgInfo &ci);

private:
    Ui::ConnectDlg *ui;
};

#endif // CONNECTDLG_H
