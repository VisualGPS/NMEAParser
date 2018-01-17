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

///
/// \class CNMEASentenceBase
/// \brief This is the base class for every NMEA sentence
///
class CNMEASentenceBase
{
public:
	static const int				c_nMaxField = 256;							///< Maximum field length

private:
	std::string						m_strSentenceID;							///< Sentence ID, ie: GGA, RMC, etc...
	CNMEAParserData::TALKER_ID_E	m_nTalkerID;								///< Talker ID, ie: GP, GN, etc...

protected:
	unsigned int					m_uRxCount;									///< Receive count

public:
	CNMEASentenceBase();
	~CNMEASentenceBase();

	///
	/// \brief Process the data from the specific NMEA sentence. 
	///
	/// This method was made into a pure virtual to force the child class to define 
	/// the sentence processor. You must redefine this method to process the
	/// specific data. See CNMEASentenceGGA::ProcessSentence() child class method
	/// for an example.
	///
	/// \param pCmd Talker command
	/// \param pData Comma separated talker data string.
	/// \return ERROR_OK if successful
	///
	virtual CNMEAParserData::ERROR_E ProcessSentence(char *pCmd, char *pData) = 0;

	///
	/// \brief Clears the sentence specific data to a default value
	///
	/// This method was made into a pure virtual to force the child class to define 
	/// the sentence processor. You must redefine this method to process the
	/// specific data. See CNMEASentenceGGA::ResetData() child class method
	/// for an example.
	///
	virtual void ResetData(void) = 0;

	///
	/// \brief Returns the receive count for this sentence
	/// \return unsigned int - receive count
	///
	unsigned int GetRxCount(void) {	return m_uRxCount;	}
protected:
	///
	/// \brief
	/// This function will get the specified field in a NMEA string.
	///
	/// \param	pData -		Pointer to NMEA string
	///	\param	pField -		pointer to returned field
	///	\param	nFieldNum -		Field offset to get
	///	\param	nMaxFieldLen -	Maximum of bytes pFiled can handle
	///
	CNMEAParserData::ERROR_E GetField(char * pData, char * pField, int nFieldNum, int nMaxFieldLen);

};

