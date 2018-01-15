
\mainpage NMEAParser Library

# NMEA Parser Class Library
Monte Variakojis \@VisualGPS

## Introduction
The NMEA 0813 standard for interfacing marine electronics devices specifies the NMEA data sentence structure as well as general definitions of approved sentences. However, the specification does not cover implementation and design. This article will hopefully clarify some of the design tasks needed to parse through NMEA sentences robustly. It will try to show techniques in parsing and data integrity checking. The article does assume that the reader has knowledge in software design and has experience in some sort of programming language. This article will reference the NMEA 0183 specification and it is recommended that the NMEA 0813 specification be available as a reference.

This article makes no assumption of the media that the NMEA data is acquired. The techniques here can be applied on received data from a higher abstraction layer. A simple example written in C++ demonstrates this parser design. Please note that the specific NMEA sentences parsed are centric to a GPS device.

## NMEA Protocol
NMEA data is sent in 8-bit ASCII where the MSB is set to zero (0). The specification also has a set of reserved characters. These characters assist in the formatting of the NMEA data string. The specification also states valid characters and gives a table of these characters ranging from HEX 20 to HEX 7E.

As stated in the NMEA 0183 specification version 3.01 the maximum number of characters shall be 82, consisting of a maximum of 79 characters between start of message "$" or "!" and terminating delimiter <CR><LF> (HEX 0D and 0A). The minimum number of fields is one (1).

### Basic sentence format:

````
    $aaccc,c--c*hh<CR><LF>
        $           Start of sentence
        aaccc       Address field/Command
        ,	        Field delimiter (Hex 2C)
        c--c        Data sentence block
        *           Checksum delimiter (HEX 2A)
        hh          Checksum field (the hexadecimal value represented in ASCII)
        <CR><LF>    End of sentence (HEX OD OA)
````

### NMEA Parser
Most protocols have a state machine tracking the protocol state and any errors that may occur during the data transfer. The NMEA parser we are discussing is based on a simple state machine. By using a state machine the computer can easily keep track of where it is within the protocol as well as recover from any errors such as timeouts and checksum errors.

The parsing example is designed such that a buffer can be sent to the parser along with the buffer length to maximize the computer processor efficiency. This will allow the computer to parse data when it is received. Since NMEA data is typically sent at 4800 baud, computers are often waiting for data. By having a state machine in place, partial or complete sets of NMEA data may be parsed. If only a partial set of data was received and sent to the parser, then when the rest of the data is received, the parser will complete any NMEA sentence that may have been incomplete.

Below, Figure 1 illustrates the data flow to the NMEA parser. Our design will assume two abstract software layers, NMEA protocol parser and specific NMEA sentence parser. It does not matter the number of data bytes received or if there is only a partial message. The NMEA parser will manage the data and extract individual NMEA sentences.

\dot [ figure 1 ]
	digraph data_flow{
		node[fontname = Helvetica, fontsize = 10];
		edge[fontname = Helvetica, fontsize = 10];

		RECEIVE_DATA[label = "Data Received" shape="rectangle"];
		SEND_RAW_DATA[label = "Send raw data to NMEA Parser" shape="rectangle"];
		PROCESSED_ALL_DATA[label = "Processed All\nData?" shape="diamond"];
		SENTENCE_FOUND[label = "NMEA sentence\nfound?" shape="diamond"];
		PROCESS_SENTENSE[label = "Process Sentence" shape="rectangle"];
		DONE[label = "Done" shape="rectangle"];

		RECEIVE_DATA->PROCESSED_ALL_DATA[arrowhead = "normal", style = "solid"];
		PROCESSED_ALL_DATA->SEND_RAW_DATA[arrowhead = "normal", style = "solid", label="No", tailport=s];
		PROCESSED_ALL_DATA->DONE[arrowhead = "normal", style = "solid", label="Yes", tailport=e];
 		SEND_RAW_DATA->SENTENCE_FOUND[arrowhead = "normal", style = "solid", headport=n];
		SENTENCE_FOUND->PROCESSED_ALL_DATA[arrowhead = "normal", style = "solid", label="No", tailport=w, headport=w];
		SENTENCE_FOUND->PROCESS_SENTENSE[arrowhead = "normal", style = "solid", label="Yes", tailport=s];
		PROCESS_SENTENSE->PROCESSED_ALL_DATA[arrowhead = "normal", style = "solid", tailport=w, headport=w];
}
\enddot

## How it Works
The NMEAParser class is composed of two main classes and individual NMEA sentence classes.

\dot Figure 2 - Class Hierarchy
	digraph packet_state_machine {
		node[fontname = Helvetica, fontsize = 10];
		edge[fontname = Helvetica, fontsize = 10];

		CNMEAParser[label = "CNMEAParser" shape="rectangle" URL = "\ref CNMEAParser"];
		CNMEAParserPacket[label = "CNMEAParserPacket" shape="rectangle" URL = "\ref CNMEAParserPacket"];
		CNMEAParserPacket->CNMEAParser[arrowhead = "normal", dir="back", style = "solid"];
	}
\enddot

The CNMEAParserPacket does the work of parsing the actual packet from a buffer of data. The data is feed into the state machine using the CNMEAParserPacket::ProcessNMEABuffer() method. This method will parse through the data, verify the checksum (if present) and call the virtual method CNMEAParserPacket::ProcessRxCommand(). The CNMEAParser class is based from the CNMEAParserPacket, redefines the ProcessRxCommand() method, and actual parse the specific NMEA sentence.

## Packet Parser State Machine - CNMEAParserPacket::ProcessNMEABuffer()

Located in the CNMEAParserPacket::ProcessNMEABuffer() method, the state machine will parse the incoming buffer weather it’s one byte or contains multiple NMEA sentences. Below describes the packet parsing states for a NMEA sentence.

\dot Receive Packet State Machine
	digraph packet_state_machine{
		node[fontname = Helvetica, fontsize = 10];
		edge[fontname = Helvetica, fontsize = 10];

		PARSE_STATE_SOM[label = "1\nPARSE_STATE_SOM" URL = "\ref CNMEAParserPacket::PARSE_STATE"];
		PARSE_STATE_CMD[label = "2\nPARSE_STATE_CMD" URL = "\ref CNMEAParserPacket::PARSE_STATE"];
		PARSE_STATE_DATA[label = "3\nPARSE_STATE_DATA" URL = "\ref CNMEAParserPacket::PARSE_STATE"];
		PARSE_STATE_CHECKSUM_1[label = "4\nPARSE_STATE_CHECKSUM_1" URL = "\ref CNMEAParserPacket::PARSE_STATE"];
		PARSE_STATE_CHECKSUM_2[label = "5\nPARSE_STATE_CHECKSUM_2" URL = "\ref CNMEAParserPacket::PARSE_STATE"];

		ProcessRxCommand[label = "Call Virtual ProcessRxCommand" shape="rectangle" URL = "\ref CNMEAParserPacket::ProcessRxCommand"];

		PARSE_STATE_SOM->PARSE_STATE_CMD[arrowhead = "normal", style = "solid"];
		PARSE_STATE_CMD->PARSE_STATE_DATA[arrowhead = "normal", style = "solid"];

		PARSE_STATE_DATA->PARSE_STATE_CHECKSUM_1[arrowhead = "normal", style = "solid"];
		PARSE_STATE_DATA->PARSE_STATE_SOM[arrowhead = "normal", style = "solid", color = red, label="Checksum\nMismatch"];
		PARSE_STATE_DATA->ProcessRxCommand [arrowhead = "normal", style = "solid", label="Sentence With\nNo Checksum"];
		PARSE_STATE_DATA->PARSE_STATE_SOM[arrowhead = "normal", style = "solid", color = red, label="Buffer\nOverflow"];

		PARSE_STATE_CHECKSUM_1->PARSE_STATE_CHECKSUM_2[arrowhead = "normal", style = "solid"];
		PARSE_STATE_CHECKSUM_2->ProcessRxCommand [arrowhead = "normal", style = "solid"];
		PARSE_STATE_CHECKSUM_2->PARSE_STATE_SOM [arrowhead = "normal", style = "solid", color = red, label="Checksum\nMismatch"];
		ProcessRxCommand->PARSE_STATE_SOM [arrowhead = "normal", style = "solid"];
	}
\enddot

### State machine state definitions:

  1.  Search for Start of Message (SOM). This state will look for the '$' (HEX 24) character. When the SOM is found, then the only transition is to get the NMEA address field or what I call the command.
  2.  Retrieve address field (command). The parser will collect characters until it encounters a ',' (comma HEX 2C) character. We are leaving the address field flexible length. This will allow parsing of any strange or proprietary sentences.
  3.  Retrieve sentence data. This is where all of the data associated with the NMEA address is collected for further parsing. This state will continue to collect data and perform a checksum calculation until it receives a checksum delimiter '*' (HEX 2A) or sentence terminator <CR><LF> (HEX 0D 0A).
  4.  Receive first checksum ASCII character. This state simply waits for the first checksum character.
  5.  Receive second checksum ASCII character. When the second checksum character is received, there should be enough information to verify the received checksum to the calculated checksum that was preformed in state 3. If the checksums match, then only other task to do is check for the sentence terminator.

When a complete NMEA command sentence is received, the virtual method CNMEAParserPacket::ProcessRxCommand() 
is called. This method is redefined in the CNMEAParser class and will parse the command, data, and stuff
them into the correct CNMESentence object.

#### Timeouts
Timeouts can be handled at a higher level. If for some reason data has not been received for a specified period of time, the software can reset the NMEA Parser by calling the CNMEAParserPacket::Reset() method.

## Source Code
NMEAParserLib is a C++ class library that will accept variable length data and parse a NMEA stream. Even though the code is written in C++, it only uses inherencies and virtual methods. 

## Features

  - No outside library dependencies.
  - OS independent (no OS specific calls)
  - Abstract software synchronization. You can redefine the data lock virtual method to support your OS
    specific semaphore or mutex functions.
  - NMEA sentence notification. Redefine method to get notified that a new NMEA sentence was
    processed.

## Required and Optional Tools
  - cmake - Build environment. See https://cmake.org
  - C++ 11 complaint compiler (Linux gcc or MS Windows VisualStudio)
  - doxygen (for documentation, optional but recommended) - http://www.stack.nl/~dimitri/doxygen/
  - graphviz (for documentation, optional but recommended) - http://graphviz.org/
  - Qt 5.8 (optional) - https://www.qt.io/ Qt is a cross platform graphical interface tool that will allow you to build comprehensive GUI application.  The source code also includes a Qt project that uses the NMEAParser, displays position status, satellite azimuth/elevation, and signal quality graphically.

## Build

  - Linux:
	\code{.sh}
	   $ mkdir ./NMEAParserBuild
	   $ cd NMEAParserBuild
	   $ cmake ../Software
	   $ make
	   $ make doc
	\endcode
  - Windows:
	\code{.sh}
	   [code was cloned to c:\Source\NMEAParser]
       c:\Source\NMEAParser>mkdir ./NMEAParserBuild
       c:\Source\NMEAParser>cd NMEAParserBuild
       c:\Source\NMEAParser\NMEAParserBuild>cmake ../Software
	   [Run VisualStudio and build as normal]
	\endcode

## Usage

Below is a basic program that defines a CNMEAParser object and streams NMEA data from a text
file.

\code{.cpp}
	#include <stdio.h>
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
		/// \param pCmd Pointer to NMEA command that caused the error. Please note that this
		/// parameter may be NULL of not completely defined. Use with caution.
		///
		virtual void OnError(CNMEAParserData::ERROR_E nError, char *pCmd) {
			printf("ERROR for Cmd: %s, Number: %d\n", pCmd, nError);
		}

	protected:
		///
		/// \brief This method is redefined from CNMEAParserPacket::ProcessRxCommand(char *pCmd, char *pData)
		///
		/// \param pCmd Pointer to the NMEA command string
		/// \param pData Comma separated data that belongs to the command
		/// \return Returns CNMEAParserData::ERROR_OK If successful
		///
		virtual CNMEAParserData::ERROR_E ProcessRxCommand(char *pCmd, char *pData) {

			// Call base class to process the command
			CNMEAParser::ProcessRxCommand(pCmd, pData);

			printf("Cmd: %s\nData: %s\n", pCmd, pData);
			return CNMEAParserData::ERROR_OK;
		}
	};

	int main(int argc, char *argv[], char *envp[]) {

		FILE *fp;
		MyNMEAParser	NMEAParser;

		//
		// Make sure that we have enough parameters
		//
		if (argc < 2) {
			printf("Usage: NMEAParserTest [file]\n");
			return -1;
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
\endcode
