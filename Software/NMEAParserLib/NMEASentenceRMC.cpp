/*
 * NMEASentenceRMC.cpp
 *
 *  Created on: Sep 21, 2017
 *      Author: monte
 */
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


#include "NMEASentenceRMC.h"
#include <stdlib.h>

CNMEASentenceRMC::CNMEASentenceRMC() {
	// TODO Auto-generated constructor stub

}

CNMEASentenceRMC::~CNMEASentenceRMC() {
	// TODO Auto-generated destructor stub
}

CNMEAParserData::ERROR_E CNMEASentenceRMC::ProcessSentence(char *pCmd, char *pData) {

    UNUSED_PARAM(pCmd);
    char szField[c_nMaxField];

	// Time
	if (GetField(pData, szField, 0, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.m_nHour = (szField[0] - '0') * 10 + (szField[1] - '0');
		m_SentenceData.m_nMinute = (szField[2] - '0') * 10 + (szField[3] - '0');
		m_SentenceData.m_nSecond = (szField[4] - '0') * 10 + (szField[5] - '0');
	}

	// Status
	if (GetField(pData, szField, 1, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.m_nStatus = (CNMEAParserData::RMC_STATUS_E)((char)szField[0]);
	}
	else {
		m_SentenceData.m_nStatus = (CNMEAParserData::RMC_STATUS_VOID);
	}

	//
	// Latitude
	//
	if (GetField(pData, szField, 2, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_dLatitude = atof((char *)szField + 2) / 60.0;
		szField[2] = '\0';
		m_SentenceData.m_dLatitude += atof((char *)szField);

	}
	if (GetField(pData, szField, 3, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		if (szField[0] == 'S')
		{
			m_SentenceData.m_dLatitude = -m_SentenceData.m_dLatitude;
		}
	}

	//
	// Longitude
	//
	if (GetField(pData, szField, 4, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		m_SentenceData.m_dLongitude = atof((char *)szField + 3) / 60.0;
		szField[3] = '\0';
		m_SentenceData.m_dLongitude += atof((char *)szField);
	}
	if (GetField(pData, szField, 5, c_nMaxField) == CNMEAParserData::ERROR_OK)
	{
		if (szField[0] == 'W')
		{
			m_SentenceData.m_dLongitude = -m_SentenceData.m_dLongitude;
		}
	}

	// Speed over ground knots
	if (GetField(pData, szField, 6, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.m_dSpeedKnots = atol(szField);
	}
	else {
		m_SentenceData.m_dSpeedKnots = 0.0;
	}

	// Track Angle
	if (GetField(pData, szField, 7, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.m_dTrackAngle = atof(szField);
	}
	else {
		m_SentenceData.m_dTrackAngle = 0.0;
	}


	// Date
	if (GetField(pData, szField, 8, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		// 23 03 94       Date - 23rd of March 1994
		m_SentenceData.m_nDay = (szField[0] - '0') * 10 + (szField[1] - '0');
		m_SentenceData.m_nMonth = (szField[2] - '0') * 10 + (szField[3] - '0');
		m_SentenceData.m_nYear = (szField[4] - '0') * 10 + (szField[4] - '0');
		m_SentenceData.m_nYear += 2000;
	}
	else {
		m_SentenceData.m_nMonth = 0;
		m_SentenceData.m_nDay = 0;
		m_SentenceData.m_nYear = 0;
	}


	// Magnetic Variation
	if (GetField(pData, szField, 9, c_nMaxField) == CNMEAParserData::ERROR_OK) {

		m_SentenceData.m_dMagneticVariation = atof(szField);

		if (GetField(pData, szField, 10, c_nMaxField) == CNMEAParserData::ERROR_OK) {
			if(szField[0] == 'W') {
				m_SentenceData.m_dMagneticVariation *= -1.0;
			}
		}
	}
	else {
		m_SentenceData.m_dMagneticVariation = 0.0;
	}


	m_uRxCount++;

	return CNMEAParserData::ERROR_OK;
}

void CNMEASentenceRMC::ResetData(void) {
	m_uRxCount = 0;
	m_SentenceData.m_dAltitudeMSL = 0.0;
	m_SentenceData.m_dLatitude = 0.0;
	m_SentenceData.m_dLongitude = 0.0;
	m_SentenceData.m_dMagneticVariation = 0.0;
	m_SentenceData.m_dSecond = 0;
	m_SentenceData.m_dSpeedKnots = 0.0;
	m_SentenceData.m_dTrackAngle = 0.0;
	m_SentenceData.m_nDay = 0;
	m_SentenceData.m_nHour = 0;
	m_SentenceData.m_nMinute = 0;
	m_SentenceData.m_nMonth = 0;
	m_SentenceData.m_nSecond = 0;
	m_SentenceData.m_nStatus = CNMEAParserData::RMC_STATUS_VOID;
	m_SentenceData.m_nYear = 0;
}
