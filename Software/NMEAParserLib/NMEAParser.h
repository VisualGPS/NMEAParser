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
#include <cstddef>
#include <stdint.h>
#include <map>

#include "NMEAParserData.h"
#include "NMEAParserPacket.h"
#include "NMEASentenceGGA.h"
#include "NMEASentenceGSV.h"
#include "NMEASentenceGSA.h"
#include "NMEASentenceRMC.h"

///
/// \class CNMEAParser
/// \brief This class will parse NMEA data, store its data and report that it has received data
///
class CNMEAParser : public CNMEAParserPacket {

private:

	// GPS
	CNMEASentenceGGA	m_GPGGA;												///< GPGGA Specific sentence data
	CNMEASentenceGSV	m_GPGSV;												///< GPGSV Satellite message (GPS)
	CNMEASentenceGSA	m_GPGSA;												///< GPGSA GNSS DOP and active satellites
	CNMEASentenceRMC	m_GPRMC;												///< GPRMC Recommended minimum data for GPS

	// Galileo
	CNMEASentenceGGA	m_GAGGA;												///< GAGGA Specific sentence data
	CNMEASentenceGSV	m_GAGSV;												///< GAGSV Satellite message (GPS)
	CNMEASentenceGSA	m_GAGSA;												///< GAGSA GNSS DOP and active satellites
	CNMEASentenceRMC	m_GARMC;												///< GARMC Recommended minimum data for GPS

	// GNSS
	CNMEASentenceGGA	m_GNGGA;												///< GAGGA Specific sentence data
	CNMEASentenceGSA	m_GNGSA;												///< GNGSA GNSS DOP and active satellites (usually GPS and GLONASS but can be GLONASS only)
	CNMEASentenceRMC	m_GNRMC;												///< GNRMC Recommended minimum data for GPS

	// GLONASS
	CNMEASentenceGSV	m_GLGSV;												///< GLGSV Satellite message (GLONASS)
    CNMEASentenceGSA	m_GLGSA;												///< GLGSA GNSS DOP and active satellites

	// GeiDou
	CNMEASentenceGSV	m_QZGSV;												///< QZGSV Satellite message (BeiDou)
	CNMEASentenceGSA	m_QZGSA;												///< QZGSA GNSS DOP and active satellites

	// QZSS
	CNMEASentenceGSV	m_BDGSV;												///< BDGSV Satellite message (QZSS)
	CNMEASentenceGSA	m_BDGSA;												///< BDGSA GNSS DOP and active satellites

public:
	CNMEAParser();
	virtual ~CNMEAParser();

	///
	/// \brief Resets or clears all NMEA data to a known default value
	///
	void ResetData(void);

	///
	/// \brief Places a copy of the GPGGA data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGGA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGPGGA(CNMEAParserData::GGA_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GNGGA data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGGA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGNGGA(CNMEAParserData::GGA_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GPGSV data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSV object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGPGSV(CNMEAParserData::GSV_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GPGSA data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGPGSA(CNMEAParserData::GSA_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GPRMC data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGPRMC(CNMEAParserData::RMC_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GNRMC data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGNRMC(CNMEAParserData::RMC_DATA_T & sentenseData);

	///
    /// \brief Places a copy of the GNGSA data into sentenseData
    /// \param sentenseData reference to a CNMEASentenceGSA object to place the data into.
    /// \return Returns ERROR_OK if successful.
    ///
    CNMEAParserData::ERROR_E GetGNGSA(CNMEAParserData::GSA_DATA_T & sentenseData);

    ///
	/// \brief Places a copy of the GLGSV data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSV object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGLGSV(CNMEAParserData::GSV_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GLGSA data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGLGSA(CNMEAParserData::GSA_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the QZGSV data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSV object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetQZGSV(CNMEAParserData::GSV_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the QZGSA data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetQZGSA(CNMEAParserData::GSA_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the BDGSV data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSV object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetBDGSV(CNMEAParserData::GSV_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the QZGSA data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetBDGSA(CNMEAParserData::GSA_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GAGGA data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGGA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGAGGA(CNMEAParserData::GGA_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GAGSV data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSV object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGAGSV(CNMEAParserData::GSV_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GAGSA data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGAGSA(CNMEAParserData::GSA_DATA_T & sentenseData);

	///
	/// \brief Places a copy of the GARMC data into sentenseData
	/// \param sentenseData reference to a CNMEASentenceGSA object to place the data into.
	/// \return Returns ERROR_OK if successful.
	///
	CNMEAParserData::ERROR_E GetGARMC(CNMEAParserData::RMC_DATA_T & sentenseData);

protected:
	///
	/// \brief This method is redefined from CNMEAParserPacket::ProcessRxCommand(char *pCmd, char *pData)
	///
	/// \param pCmd Pointer to the NMEA command string
	/// \param pData Comma separated data that belongs to the command
	/// \return Returns CNMEAParserData::ERROR_OK If successful
	///
	virtual CNMEAParserData::ERROR_E ProcessRxCommand(char *pCmd, char *pData);

	///
	/// \brief This method will invoke a semaphore lock (mutex) for data access.
	///
	///	Redefine this method if you will be access this class from different threads.
	///
	virtual void DataAccessSemaphoreLock(void) {}

	///
	/// \brief This method will invoke a semaphore unlock (mutex) for data access.
	///
	///	Redefine this method if you will be access this class from different threads.
	///
	virtual void DataAccessSemaphoreUnlock(void) {}

};
