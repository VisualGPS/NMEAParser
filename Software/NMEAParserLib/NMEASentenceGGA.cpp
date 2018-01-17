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
#include <stdlib.h>
#include "NMEASentenceGGA.h"

CNMEASentenceGGA::CNMEASentenceGGA() :
	m_nOldVSpeedSeconds(0),
	m_dOldVSpeedAlt(0.0)
{
	ResetData();
}

CNMEASentenceGGA::~CNMEASentenceGGA() {
}

CNMEAParserData::ERROR_E CNMEASentenceGGA::ProcessSentence(char * pCmd, char * pData)
{
    UNUSED_PARAM(pCmd);
    char szField[c_nMaxField];
	char szTemp[c_nMaxField];

	if (GetField(pData, szField, 0, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.m_nHour = (szField[0] - '0') * 10 + (szField[1] - '0');
		m_SentenceData.m_nMinute = (szField[2] - '0') * 10 + (szField[3] - '0');
		m_SentenceData.m_nSecond = (szField[4] - '0') * 10 + (szField[5] - '0');
	}

	//
	// Latitude
	//
	if (GetField(pData, szField, 1, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_dLatitude = atof((char *)szField + 2) / 60.0;
		szField[2] = '\0';
		m_SentenceData.m_dLatitude += atof((char *)szField);

	}
	if (GetField(pData, szField, 2, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		if (szField[0] == 'S')
		{
			m_SentenceData.m_dLatitude = -m_SentenceData.m_dLatitude;
		}
	}

	//
	// Longitude
	//
	if (GetField(pData, szField, 3, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_dLongitude = atof((char *)szField + 3) / 60.0;
		szField[3] = '\0';
		m_SentenceData.m_dLongitude += atof((char *)szField);
	}
	if (GetField(pData, szField, 4, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		if (szField[0] == 'W')
		{
			m_SentenceData.m_dLongitude = -m_SentenceData.m_dLongitude;
		}
	}

	//
	// GPS quality
	//
	if (GetField(pData, szField, 5, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_nGPSQuality = (CNMEAParserData::GPS_QUALITY_E) ( (char)szField[0] - '0');
	}

	//
	// Satellites in use
	//
	if (GetField(pData, szField, 6, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		szTemp[0] = szField[0];
		szTemp[1] = szField[1];
		szTemp[2] = '\0';
		m_SentenceData.m_nSatsInView = atoi(szTemp);
	}

	//
	// HDOP
	//
	if (GetField(pData, szField, 7, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_dHDOP = atof((char *)szField);
	}

	//
	// Altitude, Meters, above mean sea level
	//
	if (GetField(pData, szField, 8, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_dAltitudeMSL = atof((char *)szField);
	}

	//
	// Geoidal separation, meters
	//
	if (GetField(pData, szField, 9, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_dGeoidalSep = atof((char *)szField);
	}

	//
	// Differential age
	//
	if (GetField(pData, szField, 11, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_dDifferentialAge = atof((char *)szField);
	}

	//
	// Differential ID
	//
	if (GetField(pData, szField, 13, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_nDifferentialID = atoi((char *)szField);
	}

	//
	// Derive vertical speed (bonus)
	//
	int nSeconds = (int)m_SentenceData.m_nMinute * 60 + (int)m_SentenceData.m_nSecond;
	if (nSeconds > m_nOldVSpeedSeconds)
	{
		double dDiff = (double)(m_nOldVSpeedSeconds - nSeconds);
		double dVal = dDiff / 60.0;
		if (dVal != 0.0)
		{
			m_SentenceData.m_dVertSpeed = (m_dOldVSpeedAlt - m_SentenceData.m_dAltitudeMSL) / dVal;
		}
	}
	m_dOldVSpeedAlt = m_SentenceData.m_dAltitudeMSL;
	m_nOldVSpeedSeconds = nSeconds;

	m_uRxCount++;

	return CNMEAParserData::ERROR_OK;
}

void CNMEASentenceGGA::ResetData(void)
{
	m_uRxCount = 0;
	m_SentenceData.m_dAltitudeMSL = 0.0;
	m_SentenceData.m_dDifferentialAge = 0.0;
	m_SentenceData.m_dGeoidalSep = 0.0;
	m_SentenceData.m_dHDOP = 0.0;
	m_SentenceData.m_dLatitude = 0.0;
	m_SentenceData.m_dLongitude = 0.0;
	m_SentenceData.m_dVertSpeed = 0.0;
	m_SentenceData.m_nDifferentialID = 0;
	m_SentenceData.m_nGPSQuality = CNMEAParserData::GQ_FIX_NOT_AVAILABLE;
	m_SentenceData.m_nHour = 0;
	m_SentenceData.m_nMinute = 0;
	m_SentenceData.m_nSatsInView = 0;
	m_SentenceData.m_nSecond = 0;
}
