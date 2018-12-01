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
#include <stdio.h>
#include <string.h>
#include "NMEAParser.h"

CNMEAParser::CNMEAParser()
{
	ResetData();
}

CNMEAParser::~CNMEAParser()
{
}

void CNMEAParser::ResetData(void)
{
	//
	// Lock access to data
	//
	DataAccessSemaphoreLock();

	m_GPGGA.ResetData();
	m_GPGSV.ResetData();
	m_GPGSA.ResetData();
    m_GNGSA.ResetData();
	m_GNGGA.ResetData();
	m_GNRMC.ResetData();
	m_GLGSV.ResetData();
	m_GLGSA.ResetData();
	m_QZGSV.ResetData();
	m_QZGSA.ResetData();
	m_BDGSV.ResetData();
	m_BDGSA.ResetData();
	m_GPRMC.ResetData();
	m_GAGGA.ResetData();
	m_GAGSV.ResetData();
	m_GAGSA.ResetData();
	m_GARMC.ResetData();

	//
	// Unlock access to data
	//
	DataAccessSemaphoreUnlock();
}

CNMEAParserData::ERROR_E CNMEAParser::GetGPGGA(CNMEAParserData::GGA_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GPGGA.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGNGGA(CNMEAParserData::GGA_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GNGGA.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGPGSV(CNMEAParserData::GSV_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GPGSV.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGPGSA(CNMEAParserData::GSA_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GPGSA.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGPRMC(CNMEAParserData::RMC_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GPRMC.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGNRMC(CNMEAParserData::RMC_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GNRMC.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGNGSA(CNMEAParserData::GSA_DATA_T & sentenseData)
{
    DataAccessSemaphoreLock();
    sentenseData = m_GNGSA.GetSentenceData();
    DataAccessSemaphoreUnlock();
    return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGLGSV(CNMEAParserData::GSV_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GLGSV.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGLGSA(CNMEAParserData::GSA_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GLGSA.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetQZGSV(CNMEAParserData::GSV_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_QZGSV.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetQZGSA(CNMEAParserData::GSA_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_QZGSA.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetBDGSV(CNMEAParserData::GSV_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_BDGSV.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetBDGSA(CNMEAParserData::GSA_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_BDGSA.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGAGGA(CNMEAParserData::GGA_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GAGGA.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGAGSV(CNMEAParserData::GSV_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GAGSV.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}


CNMEAParserData::ERROR_E CNMEAParser::GetGAGSA(CNMEAParserData::GSA_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GAGSA.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::GetGARMC(CNMEAParserData::RMC_DATA_T & sentenseData)
{
	DataAccessSemaphoreLock();
	sentenseData = m_GARMC.GetSentenceData();
	DataAccessSemaphoreUnlock();
	return CNMEAParserData::ERROR_OK;
}

CNMEAParserData::ERROR_E CNMEAParser::ProcessRxCommand(char * pCmd, char * pData)
{

	//
	// Grab the talker ID
	//
	uint16_t u16TalkerID = (uint16_t)((uint8_t)pCmd[0]) << 8;
	u16TalkerID |= (uint16_t)((uint8_t)pCmd[1]);

	//
	// Get the sentence ID, --XXX where XXX is the sentence ID
	//
	char *lpszSentenceID = &pCmd[2];

	printf("Cmd: %s, TalkerID:%c%c, Sentence ID: %s\n", pCmd, (u16TalkerID >> 8) & 0xFF, (u16TalkerID) & 0xFF, lpszSentenceID);


	//-----------------------------------------------------------------------------
	if (strcmp(pCmd, "GPGGA") == 0) {
		DataAccessSemaphoreLock();
		m_GPGGA.ProcessSentence(pCmd, pData);
		m_GPGSA.FlagReceivedGGA();
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GNGGA") == 0) {
		DataAccessSemaphoreLock();
		m_GNGGA.ProcessSentence(pCmd, pData);
		m_GNGSA.FlagReceivedGGA();
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GPGSV") == 0) {
		DataAccessSemaphoreLock();
		m_GPGSV.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GPGSA") == 0) {
		DataAccessSemaphoreLock();
		m_GPGSA.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GPRMC") == 0) {
		DataAccessSemaphoreLock();
		m_GPRMC.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GNRMC") == 0) {
		DataAccessSemaphoreLock();
		m_GNRMC.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GAGGA") == 0) {
		DataAccessSemaphoreLock();
		m_GAGGA.ProcessSentence(pCmd, pData);
		m_GAGSA.FlagReceivedGGA();
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GAGSV") == 0) {
		DataAccessSemaphoreLock();
		m_GAGSV.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GAGSA") == 0) {
		DataAccessSemaphoreLock();
		m_GAGSA.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GARMC") == 0) {
		DataAccessSemaphoreLock();
		m_GARMC.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "GNGSA") == 0) {
        DataAccessSemaphoreLock();
        m_GNGSA.ProcessSentence(pCmd, pData);
        DataAccessSemaphoreUnlock();
    }
    else if (strcmp(pCmd, "GLGSV") == 0) {
		DataAccessSemaphoreLock();
		m_GLGSV.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
    else if (strcmp(pCmd, "GLGSA") == 0) {
		DataAccessSemaphoreLock();
		m_GLGSA.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}

	else if (strcmp(pCmd, "QZGSV") == 0) {
		DataAccessSemaphoreLock();
		m_QZGSV.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "QZGSA") == 0) {
		DataAccessSemaphoreLock();
		m_QZGSA.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}

	else if (strcmp(pCmd, "BDGSV") == 0) {
		DataAccessSemaphoreLock();
		m_BDGSV.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}
	else if (strcmp(pCmd, "BDGSA") == 0) {
		DataAccessSemaphoreLock();
		m_BDGSA.ProcessSentence(pCmd, pData);
		DataAccessSemaphoreUnlock();
	}


	return CNMEAParserData::ERROR_OK;
}

