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
#include <qdebug.h>
#include "NMEAParserQt.h"

CNMEAParserQt::CNMEAParserQt(QObject *parent) :
    QObject(parent)
{
    ResetData();
    connect(&m_SerialPort, SIGNAL(readyRead()), SLOT(on_SerialPortReadyRead()));
}

bool CNMEAParserQt::Connect(QString strPort, quint32 nBaud)
{
    m_SerialPort.close();
    m_SerialPort.setBaudRate(nBaud);
    m_SerialPort.setFlowControl(QSerialPort::NoFlowControl);
    m_SerialPort.setDataBits(QSerialPort::Data8);
    m_SerialPort.setPortName(strPort);
    m_SerialPort.setReadBufferSize(32768);

    ResetData();
    emit NewPositionUpdateGPS();

    if( m_SerialPort.open(QIODevice::ReadWrite) == false ){
        qDebug() << "CNMEAParserQt::Connect - could not open serial port. Error:" << m_SerialPort.error();
        return false;
    } else {
        qDebug() << "CNMEAParserQt::Connect - connected successfully to port:" << strPort;
        return true;
    }
}

void CNMEAParserQt::on_SerialPortReadyRead()
{
    QByteArray array = m_SerialPort.readAll();
    //qDebug() << "Received" << array.count() << "bytes...";

    if(array.size() > 0){
        ProcessNMEABuffer(array.data(), array.size());

    }
}

CNMEAParserData::ERROR_E CNMEAParserQt::ProcessRxCommand(char *pCmd, char *pData) {
    // Let the parent process the data
    CNMEAParser::ProcessRxCommand(pCmd, pData);

    qDebug() << "CNMEAParserQt::ProcessRxCommand - Command:" << pCmd << "Data:" << pData;

    // Update position anytime we get
    if(strstr(pCmd, "GGA") != NULL) {
        emit NewPositionUpdateGPS();
    }

    emit MessageProcessed();

    return CNMEAParserData::ERROR_OK;
}

