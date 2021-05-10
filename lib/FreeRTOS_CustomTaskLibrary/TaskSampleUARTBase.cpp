#include "TaskSampleUARTBase.h"
#include "ArduinoLog.h"

#define DBG_LVL verbose
#define DBG_BUFFER_LVL verbose

TaskSampleUARTBase::TaskSampleUARTBase(uint16_t packetSize) : TaskBase(), _packetSize(packetSize), parserFIFO(packetSize * 5)
{

}

void TaskSampleUARTBase::begin(HardwareSerial* serial, unsigned long baud)
{
	this->_port = serial;
	this->_port->setRxBufferSize(_packetSize);
	_port->begin(baud);

}
	
void TaskSampleUARTBase::loop(void* data)
{
	bool isValidPacket = false;
	auto nBytesRx = _port->available();
	auto nAvailbleToRead = (int) parserFIFO.available();
	Log.DBG_LVL("FIFO free: %d, nRx: %d \n", nAvailbleToRead, nBytesRx);

	// If we receive something at the serial port, save these bytes into the private FIFO
	if (nBytesRx > 0) {
		// If the parserFIFO is almost full, we just read a portion of the data at the UART peripheral
		if (nAvailbleToRead < nBytesRx) {
			Log.warning("parserFIFO almost full. Saved only %F %% \n", (double)nAvailbleToRead * 100 / nBytesRx);
		}
		// Transmit all the available bytes into the FIFO
		for (int i = 0; i < min(nAvailbleToRead, nBytesRx); i++) {
			parserFIFO.enqueue(_port->read());
		}
	}

	// Parse if the total bytes in FIFO is larger than the min packet size
	if (parserFIFO.size() > _packetSize) {
#define DBG_BUFFER
#ifdef DBG_BUFFER
		Log.DBG_BUFFER_LVL("Buffer contains %d Bytes: \n", parserFIFO.size());
		for (int i = 0; i < parserFIFO.size(); i++) {
			Log.DBG_BUFFER_LVL("%X ", parserFIFO[i]);
		}
		Log.DBG_BUFFER_LVL("\n Buffer End: \n");
#endif // DBG_BUFFER

		/// START FINDING THE VALID HEADER AND PASS THE PARSERFIFO TO THE CHILDCLASS TO GET THE MESSAGE PAYLOAD
		// Start 
		bool isValidHeader = false;
		//// Allocate header buffers for the child class
		static uint8_t nHeader = 0;
		static const uint8_t *trueHeaderPtr = getHeaderBytes(&nHeader);
		static CircularBuffer<uint8_t> discardedFIFO(_packetSize * 5);

		/*Log.DBG_BUFFER_LVL("%d Header Bytes are: \n", nHeader);
		for (int i = 0; i < nHeader; i++) {
			Log.DBG_BUFFER_LVL("%X ", trueHeaderPtr[i]);
		}*/

		while (!isValidHeader && (parserFIFO.size() > _packetSize)) {
			// Dequeue FIFO until we find the first header
			while (parserFIFO[0] != trueHeaderPtr[0] && (parserFIFO.size() > _packetSize)) {
				discardedFIFO.enqueue(parserFIFO.dequeue());
			}

			// Compare the consecutive header Bytes to find the true header
			// These lines are for generic header bytes checking since some sensors have multiple header bytes
			bool tmpIsHeaderValid = true;
			for (int i = 0; i < nHeader; i++) {
				tmpIsHeaderValid &= (parserFIFO[i] == trueHeaderPtr[i]);
			}
			isValidHeader = tmpIsHeaderValid;

			// Parse the packet if we could find the perfect header byte
			if (isValidHeader)
			{
				// Parse if the payload is complete
				if (parserFIFO.size() > _packetSize) {
					isValidPacket = parse();
					Log.DBG_BUFFER_LVL("Header Byte found ! Start parsing... \n");
				}
			}
			else { // Else discard the previous found header bytes and restart the finding header byte process
				discardedFIFO.enqueue(parserFIFO.dequeue());
			}
		}

		if (discardedFIFO.size() > 0) {
			Log.warning("%d Discarded Buffer: \n", discardedFIFO.size());
			for (int i = 0; i < discardedFIFO.size(); i++) {
				Log.warning("%X ", discardedFIFO.dequeue());
			}
			Log.warning("\n End Discarded Buffer:\n");
		}

		// Do something if the parsed packet was valid
		// if(isValidPacket) ...
		// else
		// ...
	}
}


