/*
 * NMEASentenceRMC.h
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


#ifndef NMEAPARSERLIB_NMEASENTENCERMC_H_
#define NMEAPARSERLIB_NMEASENTENCERMC_H_

#include "NMEASentenceBase.h"
#include "NMEAParserData.h"

class CNMEASentenceRMC : public CNMEASentenceBase {
private:
	CNMEAParserData::RMC_DATA_T		m_SentenceData;								///< Sentence specific data

public:
	CNMEASentenceRMC();
	virtual ~CNMEASentenceRMC();

	///
	/// \brief Process the data from the specific NMEA sentence.
	///
	/// This method was made into a pure virtual to force the child class to define
	/// the sentense processor. You must redefine this method to process the
	/// specific data. See CNMEASentenceGGA::ProcessSentence() child class method
	/// for an example.
	///
	/// \param pCmd Talker command
	/// \param pData Comma separated talker data string.
	/// \return ERROR_OK if successful
	///
	virtual CNMEAParserData::ERROR_E ProcessSentence(char *pCmd, char *pData);

	///
	/// \brief Clears the sentense specific data to a default value
	///
	/// This method was made into a pure virtual to force the child class to define
	/// the sentense processor. You must redefine this method to process the
	/// specific data. See CNMEASentenceGGA::ResetData() child class method
	/// for an example.
	///
	virtual void ResetData(void);

	///
	/// \brief Returns the NMEA sentence data structure
	///
	CNMEAParserData::RMC_DATA_T GetSentenceData(void) { return m_SentenceData; }
};

#endif /* NMEAPARSERLIB_NMEASENTENCERMC_H_ */
