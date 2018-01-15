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
#include "NMEASentenceBase.h"



CNMEASentenceBase::CNMEASentenceBase() :
	m_uRxCount(0)
{
}


CNMEASentenceBase::~CNMEASentenceBase()
{
}

CNMEAParserData::ERROR_E CNMEASentenceBase::GetField(char * pData, char * pField, int nFieldNum, int nMaxFieldLen)
{
	//
	// Validate parameters
	//
	if (pData == NULL || pField == NULL || nMaxFieldLen <= 0)
	{
		return CNMEAParserData::ERROR_FAIL;
	}

	//
	// Go to the beginning of the selected field
	//
	int i = 0;
	int nField = 0;
	while (nField != nFieldNum && pData[i])
	{
		if (pData[i] == ',')
		{
			nField++;
		}

		i++;

		if (pData[i] == 0)
		{
			pField[0] = '\0';
			return CNMEAParserData::ERROR_FAIL;
		}
	}

	if (pData[i] == ',' || pData[i] == '*')
	{
		pField[0] = '\0';
		return CNMEAParserData::ERROR_FAIL;
	}

	//
	// copy field from pData to Field
	//
	int i2 = 0;
	while (pData[i] != ',' && pData[i] != '*' && pData[i])
	{
		pField[i2] = pData[i];
		i2++; i++;

		//
		// check if field is too big to fit on passed parameter. If it is,
		// crop returned field to its max length.
		//
		if (i2 >= nMaxFieldLen)
		{
			i2 = nMaxFieldLen - 1;
			break;
		}
	}
	pField[i2] = '\0';

	return CNMEAParserData::ERROR_OK;
}
