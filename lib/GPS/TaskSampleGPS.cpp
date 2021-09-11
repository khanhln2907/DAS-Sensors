#include "TaskSampleGPS.h"
#include "HardwareSerial.h"
#include "ArduinoLog.h"

#include<string>

#include<vector>
using namespace std;

#include "CommonFunction.h" // Function for conversion ASCII to uint8_t

//static const char* LOG_TAG = "TaskSampleGPS ";


#define UTCTIME_MACRO _timeUTC.hour, _timeUTC.minute, _timeUTC.second

inline void Tokenize(const string& str, vector<string>& tokens, const string& delimiters)
{
	// Start at the beginning
	string::size_type lastPos = 0;
	// Find position of the first delimiter
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	// While we still have string to read
	while (string::npos != pos && string::npos != lastPos)
	{
		// Found a token, add it to the vector
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Look at the next token instead of skipping delimiters
		lastPos = pos + 1;
		// Find the position of the next delimiter
		pos = str.find_first_of(delimiters, lastPos);
	}

	// Push the last token for the last value
	tokens.push_back(str.substr(lastPos, pos - lastPos));
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

TaskSampleGPS::TaskSampleGPS(uint16_t packetSize) : TaskSampleUARTBase(packetSize)
{
}

uint16_t TaskSampleGPS::getTaskRate()
{
	return 100;
}

uint8_t TaskSampleGPS::getNHeaderBytes()
{
	return 1;
}

const uint8_t* const TaskSampleGPS::getHeaderBytes(uint8_t* nHeader)
{
	*nHeader = NUMEL(_HB);
	return _HB;
}

bool TaskSampleGPS::parse()
{
	bool isValid = false;

	// Parse iteratively to clear the fifo buffer
	while (parserFIFO[0] == 0x24 && parserFIFO.size() > _packetSize) {
		//String mes = String();
		//char* msgPtr = new char[50];
		//std::vector<char> msgPtr;
		std::string msgPtr;
		int nBytesGet = 0;
		uint8_t prevPopByte = 0x00, preprevPopByte = 0x00;
		bool csFlag = true; uint8_t calCS = 0x00;
		uint8_t rxCS[2] = { 0x00, 0x00 };
		while ((preprevPopByte != 0x0D && prevPopByte != 0x0A) && !parserFIFO.isEmpty()) { // Parse until CR LF
			uint8_t popByte = parserFIFO.dequeue();

			// Add the relevant bytes to the parser until we detect the CS byte
			// Deactivate the CS XORer if the '*' character is detected
			if (popByte == 0x2A) {
				csFlag = false;
			}
			else if (csFlag && (popByte != 0x24)) { // Dont add the header $ into the CS
				calCS ^= popByte;
				//mes += String((char)popByte);
				
				msgPtr.push_back((char)popByte);
				//msgPtr[nBytesGet] = (char)popByte;
				nBytesGet++;		// Increment the data index
			}

			// Get the real CS from sensor. Converse from the ASCII character to integer value
			if (preprevPopByte == 0x2A) {
				rxCS[0] = asciiHexToInt(prevPopByte);
				rxCS[1] = asciiHexToInt(popByte);
			}

			// Update parse tmp var
			preprevPopByte = prevPopByte;
			prevPopByte = popByte;
		}

		isValid = ((calCS & 0xF0) >> 4) == rxCS[0] && (calCS & 0x0F) == rxCS[1];
		if (isValid) {
			//Log.notice("\n %d: %d bytes GPS Parsed\n", (unsigned long)millis(), nBytesGet);
			Log.notice("%s \n", msgPtr.c_str());
			parseNMEA_GPS_Message(msgPtr);
		}
		else {
			Log.warning("Wrong checksum GPS \n");
			Log.warning("calCS:% X ->% d% d, rxCS : %d %d\n", calCS, (calCS & 0xF0) >> 4, (calCS & 0x0F), rxCS[0], rxCS[1], isValid);
		}

		// Pass the whole String to the protocol -> generic method

		//delete[]msgPtr; // Free the memory
	}
	
	return isValid;
}

void TaskSampleGPS::parseNMEA_GPS_Message(std::string msg)
{
	// Parse the message into token vector
	vector<string> tokens;
	string::size_type lastPos = 0;
	
	/*	Start Parsing	*/
	// Find position of the first delimiter
	string::size_type pos = msg.find_first_of((std::string)",", lastPos);

	// While we still have string to read
	while (string::npos != pos && string::npos != lastPos)
	{
		// Found a token, add it to the vector
		tokens.push_back(msg.substr(lastPos, pos - lastPos));
		// Look at the next token instead of skipping delimiters
		lastPos = pos + 1;
		// Find the position of the next delimiter
		pos = msg.find_first_of((std::string)",", lastPos);
	}

	// Push the last token for the last value
	tokens.push_back(msg.substr(lastPos, pos - lastPos));

//#define DEBUG_TOKENIZE
#ifdef DEBUG_TOKENIZE
	Log.notice("Parsed begin: \n");
	for (int i = 0; i < tokens.size(); i++) {
		Log.notice("%s \n", tokens.at(i).c_str());
	}
	Log.notice("Msg End !\n \n");
#endif // 

	/*	End Parsing	*/

	/*	Translate the parsed tokens	*/
	// The first token contains the information of the message, such as "GPRMC, GPGSA, ..."
	std::string msgCmd = tokens.at(0).c_str();

	// Parse the msg accordingly to the MSG ID
	// GLL(Lat, Lon with time of PosFix and Status)[0]	lat[1]		NS[2]		lon[3]		EW[4]		time[5]		status[6]		posMode[7]		  
	if (msgCmd == ("GPGLL")) {
		std::string latStr = tokens.at(1);
		std::string NS = tokens.at(2);
		std::string lonStr = tokens.at(3);
		std::string EW = tokens.at(4);
		std::string timeStr = tokens.at(5);
		char statusStr = *tokens.at(6).c_str();
		char posModeChar = *tokens.at(7).c_str();

		// Check if the data is valid ?
		// Get time
		if (!timeStr.empty()) {
			_timeUTC.hour = atoi(timeStr.substr(0, 2).c_str());
			_timeUTC.minute = atoi(timeStr.substr(2, 2).c_str());
			_timeUTC.second = atof(timeStr.substr(4, 2).c_str());
			Log.notice("GPGLL --> %d:%d:%F: ", _timeUTC.hour, _timeUTC.minute, _timeUTC.second);
		}
		if (statusStr == GPS_t::NMEAINFO::Status_DataValid) {		
			if (posModeChar != GPS_t::NMEAINFO::PosMode_No_Fix) {
				_wgs84Sample.TimeStamp = (unsigned long)millis();
				_wgs84Sample.Value.lat = (float)atoi(latStr.substr(0, 2).c_str()) + atoi(latStr.substr(2).c_str()) / 60;
				_wgs84Sample.Value.lon = (float)atoi(lonStr.substr(0, 3).c_str()) + atoi(lonStr.substr(2).c_str()) / 60;
				Log.notice(F("Lat: %F Lon: %F, "), _wgs84Sample.Value.lat, _wgs84Sample.Value.lon);
			}

			switch (posModeChar) {
			case GPS_t::NMEAINFO::PosMode_No_Fix:
			{
				Log.notice(F("Nofix \n"));
				break;
			}
			case GPS_t::NMEAINFO::PosMode_Estimated_Dead_Reckoning_Fix:
			{
				Log.notice(F("EDRFix \n"));
				break;
			}
			case GPS_t::NMEAINFO::PosMode_Autonomous_GNSS_Fix:
			{
				Log.notice(F("Auto GNSS Fix \n"));
				break;
			}
			case GPS_t::NMEAINFO::PosMode_Differential_GNSS_Fix:
			{
				Log.notice(F("DiffFix \n"));
				break;
			}
			default: {
				Log.notice(F("Parsed not define or wrong \n"));
				break;
			}
			}
		}
		else {
			Log.notice(F("Data Unreliable: %s \n"), msg.c_str());
		}
	}
	// GPRMC[0]		 time[1]		status[2]	lat[3]	NS[4]	lon[5]	EW[6]	speedOverGround[7]	courseOverGround[8]		date[9]		mv[10]		mvEW[11]	posMode[12] */
	else if (msgCmd == ("GPRMC"))
	{
		std::string timeStr = tokens.at(1);
		char status = *tokens.at(2).c_str();
		std::string latStr = tokens.at(3);
		std::string NS = tokens.at(4);
		std::string lonStr = tokens.at(5);
		std::string EW = tokens.at(6);
		std::string date = tokens.at(9);
		//std::string posMode = tokens.at(12).c_str();
		char posModeChar = *tokens.at(12).c_str();

		if (!timeStr.empty()) {
			_timeUTC.hour = atoi(timeStr.substr(0, 2).c_str());
			_timeUTC.minute = atoi(timeStr.substr(2, 2).c_str());
			_timeUTC.second = atof(timeStr.substr(4, 2).c_str());
			Log.notice("GPRMC --> %d:%d:%F: ", _timeUTC.hour, _timeUTC.minute, _timeUTC.second);
		}
		
		if (status == GPS_t::NMEAINFO::Status_DataValid) {	
			// Parsed the message if we have Fix
			if (posModeChar != GPS_t::NMEAINFO::PosMode_No_Fix) {
				_wgs84Sample.TimeStamp = (unsigned long)millis();
				_wgs84Sample.Value.lat = (float)atoi(latStr.substr(0, 2).c_str()) + atoi(latStr.substr(2).c_str()) / 60;
				_wgs84Sample.Value.lon = (float)atoi(lonStr.substr(0, 3).c_str()) + atoi(lonStr.substr(2).c_str()) / 60;
				Log.notice(F("Lat: %F Lon: %F "), _wgs84Sample.Value.lat, _wgs84Sample.Value.lon);
			}

			// Print output
			switch (posModeChar) {
				case GPS_t::NMEAINFO::PosMode_No_Fix:
				{
					Log.notice(F("Nofix \n"));
					break;
				}
				case GPS_t::NMEAINFO::PosMode_Estimated_Dead_Reckoning_Fix:
				{
					Log.notice(F("EDRFix \n"));
					break;
				}
				case GPS_t::NMEAINFO::PosMode_Autonomous_GNSS_Fix:
				{
					Log.notice(F("Auto GNSS Fix \n"));
					break;
				}
				case GPS_t::NMEAINFO::PosMode_Differential_GNSS_Fix:
				{
					Log.notice(F("DiffFix \n"));
					break;
				}
				default: {
					Log.notice(F("Parsed not define or wrong \n"));
					break;
				}
			}
		}
		else {
			Log.notice(F("Data Unreliable: %s \n"), msg.c_str());
		}
	}
	else if(msgCmd == ("GPGSA"))
	{
		Log.notice(F("%s \n"), msg.c_str());

	}
	// GPGSV(GNSS Sat in view)[0]		numMsg[1]		msgID[2]		numSatinView[3]		\loop [SatID	Elv		Az		SS] *	\in 4
	else if (msgCmd == ("GPGSV"))
	{
	std::string numMsg = tokens.at(1);
	uint8_t msgID = atoi(tokens.at(2).c_str());
	uint8_t numSatinView = atoi(tokens.at(3).c_str());

	if(msgID == 1)
		Log.notice(" GPGSV --> nSat in view: %d \n", numSatinView);
	}
	// GPVTG
	else if (msgCmd == ("GPVTG"))
	{
		Log.notice(F("%s \n"), msg.c_str());
	}
	else if (msgCmd == ("GPGGA"))
	{
		Log.notice(F("%s \n"), msg.c_str());
	}
	else
	{
		Log.warning("GPS Message not yet defined ! \n");
		Log.notice(F("%s \n"), msg.c_str());
	}
}

