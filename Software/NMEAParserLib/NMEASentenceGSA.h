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

#pragma once
#include <string>
#include "NMEAParserData.h"
#include "NMEASentenceBase.h"
#include "NMEAParserData.h"

///
/// \class CNMEASentenceGSA
/// \brief --GSA Data class
///
class CNMEASentenceGSA : public CNMEASentenceBase
{
private:
	CNMEAParserData::GSA_DATA_T		m_SentenceData;								///< Sentence specific data
	unsigned int					m_nOldGGACount;								///< Used to determine if we are getting more than one GSA sentence per position
	int								m_nIndexCount;								///< Index into the satellite database

public:
	CNMEASentenceGSA();
	virtual ~CNMEASentenceGSA();

	///
	/// \brief Process the --GGA command and stores the result in the specific data structure
	///
	/// \param pCmd Talker command
	/// \param pData Comma separated talker data string.
	/// \return ERROR_OK if successful
	///
	virtual CNMEAParserData::ERROR_E ProcessSentence(char *pCmd, char *pData);

	///
	/// \brief Clears the sentence specific data to a default value
	///
	/// Note: Redefined. See parent class CNMEASentenceBase
	///
	virtual void ResetData(void);

	///
	/// \brief Returns the NMEA sentence data structure
	///
	CNMEAParserData::GSA_DATA_T GetSentenceData(void) {	return m_SentenceData; 	}

	///
	/// \brief This method is called from the same constellation GGA processing to let 
	/// the GGA data know we have received a GGA message. 
	///
	/// This is a NMEA hack! Because the NMEA standard specifies that only 12 satellites 
	/// can be specified in this message. If the receiver has more than 12, it [usually] 
	/// sends another GSA sentence right after this one. We use the GGA to reset the count.
	/// 
	void FlagReceivedGGA() { m_nIndexCount = 0;	}
};

