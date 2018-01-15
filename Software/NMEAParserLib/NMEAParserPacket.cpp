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
#include "NMEAParser.h"

CNMEAParserPacket::CNMEAParserPacket() :
	m_nState(PARSE_STATE_SOM),
	m_u8Checksum(0),
	m_u8ReceivedChecksum(0),
	m_nIndex(0)
{
	Reset();
}

CNMEAParserPacket::~CNMEAParserPacket()
{
}

CNMEAParserData::ERROR_E CNMEAParserPacket::ProcessNMEABuffer(char * pData, size_t nBufferSize)
{
	for (size_t i = 0; i < nBufferSize; i++) {
		char cData = pData[i];
		switch (m_nState)
		{
			///////////////////////////////////////////////////////////////////////
			// Search for start of message '$'
		case PARSE_STATE_SOM:
			//
			// Data byte Start of Message?
			//
			if (cData == '$')
			{
				//
				// Time tag this message
				//
				TimeTag();

				m_u8Checksum = 0;			// reset checksum
				m_nIndex = 0;				// reset index
				m_nState = PARSE_STATE_CMD;
			}
			break;

			///////////////////////////////////////////////////////////////////////
			// Retrieve command (NMEA Address)
		case PARSE_STATE_CMD:
			if (cData != ',' && cData != '*')
			{
				m_pCommand[m_nIndex++] = cData;
				m_u8Checksum ^= cData;

				// Check for command overflow
				if (m_nIndex >= CNMEAParserData::c_uMaxCmdLen)
				{
					OnError(CNMEAParserData::ERROR_CMD_BUFFER_OVERFLOW, m_pCommand);
					m_nState = PARSE_STATE_SOM;
				}
			}
			else
			{
				m_pCommand[m_nIndex] = '\0';	// terminate command
				m_u8Checksum ^= cData;
				m_nIndex = 0;
				m_nState = PARSE_STATE_DATA;	// goto get data state
			}
			break;

			///////////////////////////////////////////////////////////////////////
			// Store data and check for end of sentence or checksum flag
		case PARSE_STATE_DATA:
			if (cData == '*') // checksum flag?
			{
				m_pData[m_nIndex] = '\0';
				m_nState = PARSE_STATE_CHECKSUM_1;
			}
			else // no checksum flag, store data
			{
				//
				// Check for end of sentence with no checksum
				//
				if (cData == '\r')
				{
					m_pData[m_nIndex] = '\0';
					ProcessRxCommand(m_pCommand, m_pData);
					m_nState = PARSE_STATE_SOM;
					return CNMEAParserData::ERROR_OK;
				}

				//
				// Store data and calculate checksum
				//
				m_u8Checksum ^= cData;
				m_pData[m_nIndex] = cData;
				// Increment and check next position in buffer
				if (++m_nIndex >= CNMEAParserData::c_uMaxDataLen) // Check for buffer overflow
				{
					OnError(CNMEAParserData::ERROR_RX_BUFFER_OVERFLOW, m_pCommand);
					m_nState = PARSE_STATE_SOM;
				}
			}
			break;

			///////////////////////////////////////////////////////////////////////
		case PARSE_STATE_CHECKSUM_1:
			if ((cData - '0') <= 9)
			{
				m_u8ReceivedChecksum = (cData - '0') << 4;
			}
			else
			{
				m_u8ReceivedChecksum = (cData - 'A' + 10) << 4;
			}

			m_nState = PARSE_STATE_CHECKSUM_2;

			break;

			///////////////////////////////////////////////////////////////////////
		case PARSE_STATE_CHECKSUM_2:
			if ((cData - '0') <= 9)
			{
				m_u8ReceivedChecksum |= (cData - '0');
			}
			else
			{
				m_u8ReceivedChecksum |= (cData - 'A' + 10);
			}

			if (m_u8Checksum == m_u8ReceivedChecksum)
			{
				ProcessRxCommand(m_pCommand, m_pData);
			}
			// Checksum error
			else {
				OnError(CNMEAParserData::ERROR_CHECKSUM, m_pCommand);
			}

			m_nState = PARSE_STATE_SOM;
			break;

			///////////////////////////////////////////////////////////////////////
		default: m_nState = PARSE_STATE_SOM;
		}
	}
	return CNMEAParserData::ERROR_OK;
}

void CNMEAParserPacket::Reset(void) {
	m_nState = PARSE_STATE_SOM;
	m_u8Checksum = m_u8ReceivedChecksum = 0;
	m_nIndex = 0;
}

