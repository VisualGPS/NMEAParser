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
#include <string.h>
#include "NMEASentenceGSA.h"

CNMEASentenceGSA::CNMEASentenceGSA() 
{
	ResetData();
}

CNMEASentenceGSA::~CNMEASentenceGSA() {
}

CNMEAParserData::ERROR_E CNMEASentenceGSA::ProcessSentence(char * pCmd, char * pData)
{
    UNUSED_PARAM(pCmd);

	char szField[c_nMaxField];

	// Auto mode
	if (GetField(pData, szField, 0, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.nAutoMode = (szField[0] == 'A') ? CNMEAParserData::ASAM_AUTO : CNMEAParserData::ASAM_MANUAL;
	}
	else {
		m_SentenceData.nAutoMode = CNMEAParserData::ASAM_MANUAL;
	}
	// Fix mode
	if (GetField(pData, szField, 1, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.nMode = (CNMEAParserData::ACTIVE_SAT_MODE_E)atoi(szField);
	}
	else {
		m_SentenceData.nMode = CNMEAParserData::ASM_FIX_NOT_AVAILABLE;
	}

	// Grab the satellite data
	int nIndexCount = 0;
	for (int i = 0; i < CNMEAParserData::c_nMaxGSASats; i++) {
		if (GetField(pData, szField, 2 + i, c_nMaxField) == CNMEAParserData::ERROR_OK) {
			m_SentenceData.pnPRN[i + m_nIndexCount] = atoi(szField);
			nIndexCount++;
		}
		else {
			m_SentenceData.pnPRN[i + m_nIndexCount] = CNMEAParserData::c_nInvlidPRN;
		}
	}
	m_nIndexCount = nIndexCount;

	// PDOP
	if (GetField(pData, szField, 14, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.dPDOP = atof(szField);
	}
	else {
		m_SentenceData.dPDOP = 0.0;
	}

	// HDOP
	if (GetField(pData, szField, 15, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.dHDOP = atof(szField);
	}
	else {
		m_SentenceData.dHDOP = 0.0;
	}

	// VDOP
	if (GetField(pData, szField, 16, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.dVDOP = atof(szField);
	}
	else {
		m_SentenceData.dVDOP = 0.0;
	}

	m_uRxCount++;

	return CNMEAParserData::ERROR_OK;
}

void CNMEASentenceGSA::ResetData(void)
{
	m_uRxCount = 0;
	m_SentenceData.dHDOP = 0.0;
	m_SentenceData.dPDOP = 0.0;
	m_SentenceData.dVDOP = 0.0;
	m_SentenceData.nAutoMode = CNMEAParserData::ASAM_MANUAL;
	m_SentenceData.nMode = CNMEAParserData::ASM_FIX_NOT_AVAILABLE;
    memset(&m_SentenceData.pnPRN[0], 0, sizeof(m_SentenceData.pnPRN));

	m_nOldGGACount = 0;
	m_nIndexCount = 0;
}
