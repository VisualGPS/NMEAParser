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
#include "NMEASentenceGSV.h"
#include <stdlib.h>
#include <string.h>


CNMEASentenceGSV::CNMEASentenceGSV()
{
}


CNMEASentenceGSV::~CNMEASentenceGSV()
{
}

CNMEAParserData::ERROR_E CNMEASentenceGSV::ProcessSentence(char * pCmd, char * pData)
{
    UNUSED_PARAM(pCmd);
    char szField[c_nMaxField];

	// Number of sentences
	if (GetField(pData, szField, 0, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.nTotalNumberOfSentences = atoi(szField);
	}

	// Number of sentences
	if (GetField(pData, szField, 1, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.nSentenceNumber = atoi(szField);
	}

	// Number of satellites in view
	if (GetField(pData, szField, 2, c_nMaxField) == CNMEAParserData::ERROR_OK) {
		m_SentenceData.nSatsInView = atoi(szField);
	}

	for (int i = 0; i < 4; i++) {
		// Calculate the index into the satellite data array base on the sentence number
		int nIndex = (m_SentenceData.nSentenceNumber - 1) * 4 + i;
		if ((nIndex + 4) >= CNMEAParserData::c_nMaxConstellation) {
			return CNMEAParserData::ERROR_TOO_MANY_SATELLITES;
		}

		// Get PRN
		if (GetField(pData, szField, i*4 + 3, c_nMaxField) == CNMEAParserData::ERROR_OK) {
			m_SentenceData.SatInfo[nIndex].nPRN = atoi(szField);
		}
		else {
			m_SentenceData.SatInfo[nIndex].nPRN = CNMEAParserData::c_nInvlidPRN;
		}
		// Elevation
		if (GetField(pData, szField, i * 4 + 4, c_nMaxField) == CNMEAParserData::ERROR_OK) {
			m_SentenceData.SatInfo[nIndex].dElevation = atof(szField);
		}
		else {
			m_SentenceData.SatInfo[nIndex].dElevation = 0.0;
		}
		// Azimuth
		if (GetField(pData, szField, i * 4 + 5, c_nMaxField) == CNMEAParserData::ERROR_OK) {
			m_SentenceData.SatInfo[nIndex].dAzimuth = atof(szField);
		}
		else {
			m_SentenceData.SatInfo[nIndex].dAzimuth = atof(szField);
		}
		// Signal to noise
		if (GetField(pData, szField, i * 4 + 6, c_nMaxField) == CNMEAParserData::ERROR_OK) {
			m_SentenceData.SatInfo[nIndex].nSNR = atoi(szField);
		}
		else {
			m_SentenceData.SatInfo[nIndex].nSNR = 0;
		}
	}

	// Check if this was the last sentence and clear the rest of the constellation data
	if (m_SentenceData.nSentenceNumber == m_SentenceData.nTotalNumberOfSentences) {
		for (int i = m_SentenceData.nTotalNumberOfSentences * 4; i < CNMEAParserData::c_nMaxConstellation; i++) {
			m_SentenceData.SatInfo[i].nPRN = CNMEAParserData::c_nInvlidPRN;
			m_SentenceData.SatInfo[i].dAzimuth = 0.0;
			m_SentenceData.SatInfo[i].dElevation = 0.0;
			m_SentenceData.SatInfo[i].nSNR = 0;
		}
	}

	m_uRxCount++;

	return CNMEAParserData::ERROR_OK;
}

void CNMEASentenceGSV::ResetData(void)
{
	m_uRxCount = 0;
	memset(&m_SentenceData.SatInfo[0], 0, sizeof(m_SentenceData.SatInfo));
	m_SentenceData.nSatsInView = 0;
	m_SentenceData.nSentenceNumber = 0;
	m_SentenceData.nTotalNumberOfSentences = 0;


}
