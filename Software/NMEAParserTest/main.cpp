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
#include <NMEAParser.h>

///
/// \class MyParser
/// \brief child class of CNMEAParser which will redefine notification calls from the parent class.
///
class MyNMEAParser : public CNMEAParser {

	///
	/// \brief This method is called whenever there is a parsing error.
	///
	/// Redefine this method to capture errors.
	///
	/// \param pCmd Pointer to NMEA command that caused the error. Please note that this parameter may be NULL of not completely defined. Use with caution.
	///
	virtual void OnError(CNMEAParserData::ERROR_E nError, char *pCmd) {
		printf("ERROR for Cmd: %s, Number: %d\n", pCmd, nError);
	}

protected:
	///
	/// \brief This method is redefined from CNMEAParserPacket::ProcessRxCommand(char *pCmd, char *pData)
	///
	/// Here we are capturing the ProcessRxCommand to print out status. We also are looking for
	/// the GPGGA message and displaying some data from it.
	///
	/// \param pCmd Pointer to the NMEA command string
	/// \param pData Comma separated data that belongs to the command
	/// \return Returns CNMEAParserData::ERROR_OK If successful
	///
	virtual CNMEAParserData::ERROR_E ProcessRxCommand(char *pCmd, char *pData) {

		// Call base class to process the command
		CNMEAParser::ProcessRxCommand(pCmd, pData);

		printf("Cmd: %s\nData: %s\n", pCmd, pData);

		// Check if this is the GPGGA command. If it is, then display some data
		if (strstr(pCmd, "GPGGA") != NULL) {
			CNMEAParserData::GGA_DATA_T ggaData;
			if (GetGPGGA(ggaData) == CNMEAParserData::ERROR_OK) {
				printf("GPGGA Parsed!\n");
				printf("   Time:                %02d:%02d:%02d\n", ggaData.m_nHour, ggaData.m_nMinute, ggaData.m_nSecond);
				printf("   Latitude:            %f\n", ggaData.m_dLatitude);
				printf("   Longitude:           %f\n", ggaData.m_dLongitude);
				printf("   Altitude:            %.01fM\n", ggaData.m_dAltitudeMSL);
				printf("   GPS Quality:         %d\n", ggaData.m_nGPSQuality);
				printf("   Satellites in view:  %d\n", ggaData.m_nSatsInView);
				printf("   HDOP:                %.02f\n", ggaData.m_dHDOP);
				printf("   Differential ID:     %d\n", ggaData.m_nDifferentialID);
				printf("   Differential age:    %f\n", ggaData.m_dDifferentialAge);
				printf("   Geoidal Separation:  %f\n", ggaData.m_dGeoidalSep);
				printf("   Vertical Speed:      %.02f\n", ggaData.m_dVertSpeed);
			}
		}

		return CNMEAParserData::ERROR_OK;
	}
};

void Test(void) {
	// Create a NMEA parser object
	MyNMEAParser NMEAParser;

	NMEAParser.ResetData();

	//
	// Define some NMEA test strings.
	//
 	const char * szGGASample = "$GPGGA,145416.00,3350.10959,N,11751.22870,W,1,09,0.85,70.3,M,-32.7,M,,*5B";
 	const char * szGSASample = "$GPGSA,A,3,06,25,12,19,05,24,02,29,,,,,1.79,0.94,1.52*06";
	const char * szGGASampleNoCS = "$GPGGA,145416.00,3350.10959,N,11751.22870,W,1,09,0.85,70.3,M,-32.7,M,,\r";
	const char * szGSVSample = \
			"$GPGSV,3,1,12,01,12,040,26,02,07,190,19,06,31,163,26,13,34,233,32*7D"\
			"$GPGSV,3,2,12,15,25,270,21,17,74,020,39,19,76,246,34,24,23,316,30*7D"\
			"$GPGSV,3,3,12,28,43,059,37,30,25,139,28,46,47,145,,51,49,161,28*7E";
	const char * szGSVTestTrash = \
			"$GPGSV,3,1,12,01,12,040,26,02,07,190,19,06,31,163,26,13,34,233,32*7D"\
			"This is trash and should be ignored by the parser"\
			"$GPGSV,3,2,12,15,25,270,21,17,74,020,39,19,76,246,34,24,23,316,30*7D"\
			"$GPGSV,3,3,12,28,43,059,37,30,25,139,28,46,47,145,,51,49,161,28*7E";
	const char * szGGASampleBadCS = "$GPGGA,013439.00,3350.11301,N,117.22904,W,1,08,1.03,59.0,M,-32.7,M,,*56";
	const char * szPartial1 = "$GPGGA,182324.00,3350.10805,N,11751.22832,";
	const char * szPartial2 = "W,1,08,0.94,56.2,M,-32.7,M,,*5C";
	const char * szGLONASSSample = \
			"$GPGGA,033430.000,3351.8548,N,11744.4380,W,2,14,0.7,153.7,M,-33.7,M,0000,0000*6F" \
			"$GPGSA,A,3,09,07,30,08,05,27,23,28,,,,,1.1,0.7,0.8*33" \
			"$GNGSA,A,3,65,75,72,85,76,66,,,,,,,1.1,0.7,0.8*2B" \
			"$GPGSV,3,1,11,09,71,151,24,07,62,350,42,51,49,161,34,30,45,305,38*73" \
			"$GPGSV,3,2,11,08,42,089,30,28,40,228,32,23,36,142,31,27,22,051,26*77" \
			"$GPGSV,3,3,11,05,14,308,26,11,08,136,,16,06,043,*4C" \
			"$GLGSV,3,1,09,65,57,277,24,75,52,320,42,72,41,196,35,74,33,036,26*63" \
			"$GLGSV,3,2,09,84,22,072,,76,19,272,29,85,16,127,30,66,14,331,28*62" \
			"$GLGSV,3,3,09,83,04,022,*53" \
			"$GPRMC,033430.000,A,3351.8548,N,11744.4380,W,0.00,173.73,051217,,*1A";
	const char * szDoubleGSATest = \
		"$GNGGA,052105.00,3351.85350,N,11744.43680,W,1,12,0.73,154.7,M,-32.6,M,,*78" \
		"$GNGSA,A,3,19,28,13,15,17,24,06,02,,,,,1.34,0.73,1.12*1C" \
		"$GNGSA,A,3,73,74,75,83,84,85,,,,,,,1.34,0.73,1.12*13" \
		"$GLGSV,2,1,06,73,37,123,25,74,71,022,21,75,24,327,28,83,14,031,22*65" \
		"$GLGSV,2,2,06,84,70,020,22,85,49,220,24*6C" \
		"$GNGGA,052105.00,3351.85350,N,11744.43680,W,1,12,0.73,154.7,M,-32.6,M,,*78" \
		"$GNGSA,A,3,19,28,13,15,17,24,06,02,,,,,1.34,0.73,1.12*1C" \
		"$GNGSA,A,3,73,74,75,83,84,85,,,,,,,1.34,0.73,1.12*13";


	const char * szGalileoTest = \
		"$GAGSV,2,1,08,02,21,256,41,03,40,275,44,05,28,339,39,08,14,224,42,0*7E" \
		"$GAGSV,2,2,08,11,36,052,37,12,28,115,34,24,39,122,46,25,59,203,44,0*74";


	// Test Individual sentences
	NMEAParser.ProcessNMEABuffer((char *)szGGASample, (int)strlen(szGGASample));
	NMEAParser.ProcessNMEABuffer((char *)szGSASample, (int)strlen(szGSASample));
	NMEAParser.ProcessNMEABuffer((char *)szGSVSample, (int)strlen(szGSVSample));

	// Test GLONASS
	NMEAParser.ProcessNMEABuffer((char *)szGLONASSSample, (int)strlen(szGLONASSSample));

	// Double GSA test
	NMEAParser.ProcessNMEABuffer((char *)szDoubleGSATest, (int)strlen(szDoubleGSATest));


	// Test partial sentences
	NMEAParser.ProcessNMEABuffer((char *)szPartial1, (int)strlen(szPartial1)); // Should see no output
	NMEAParser.ProcessNMEABuffer((char *)szPartial2, (int)strlen(szPartial2)); // Sentence is complete here and you should see an output

	// Test a sentence with no Checksum - As per NMEA 0813 this is allowed if the '*' checksum indicator is not included
	NMEAParser.ProcessNMEABuffer((char *)szGGASampleNoCS, (int)strlen(szGGASampleNoCS));

	// Test GSV with trash between the sentences and not _in_ the sentence. (should still work)
	NMEAParser.ProcessNMEABuffer((char *)szGSVTestTrash, (int)strlen(szGSVTestTrash));

	// Test GGA bad checksum
	NMEAParser.ProcessNMEABuffer((char *)szGGASampleBadCS, (int)strlen(szGGASampleBadCS));

	// Galileo test
	NMEAParser.ProcessNMEABuffer((char *)szGalileoTest, (int)strlen(szGalileoTest));

}

int main(int argc, char *argv[], char *envp[]) {

	FILE *fp;
	MyNMEAParser	NMEAParser;

	//
	// Make sure that we have enough parameters. If we don't have enough parameters, then
	// we'll just call the built-in-test function. If we do have a parameter, then assume
	// that is a NMEA file and try to open it.
	//
	if (argc < 2) {
		printf("Usage: NMEAParserTest [NMEA file]\n");
		printf("If [NMEA file] is omitted, then the built in test function will be called\n");
		printf("Running built-in-test...\n");
		Test();
		return 0;
	}

	//
	// Open the NMEA file, fail if cannot open
	//
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("Could not open file: %s\n", argv[1]);
		return -1;
	}

	//
	// Read the file and process
	//
	char pBuff[1024];
	while (feof(fp) == 0) {
		size_t nBytesRead = fread(pBuff, 1, 512, fp);

		CNMEAParserData::ERROR_E nErr;
		if ((nErr = NMEAParser.ProcessNMEABuffer(pBuff, nBytesRead)) != CNMEAParserData::ERROR_OK) {
			printf("NMEA Parser ProcessNMEABuffer Failed and returned error: %d\n", nErr);
			fclose(fp);
			return -1;
		}
	}

	fclose(fp);
	return 0;
}

