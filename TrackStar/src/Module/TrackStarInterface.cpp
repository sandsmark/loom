#include "TrackStarInterface.h"

using namespace Loom::TrackStar;

TrackStarMatrix::TrackStarMatrix() {
	scale = 1.0;
	xx = xy = xz = 0;
	yx = yy = yz = 0;
	zx = zy = zz = 0;
	tx = ty = tz = 0;
}

TrackStarMatrix::TrackStarMatrix(double scale, double xx, double xy, double xz, double yx, double yy, double yz, double zx, double zy, double zz, double tx, double ty, double tz) {
	this->scale = scale;
	this->xx = xx;
	this->xy = xy;
	this->xz = xz;
	this->yx = yx;
	this->yy = yy;
	this->yz = yz;
	this->zx = zx;
	this->zy = zy;
	this->zz = zz;
	this->tx = tx;
	this->ty = ty;
	this->tz = tz;
}

bool TrackStarMatrix::convert(double &x, double &y, double &z) {
	double cx = x, cy = y, cz = z;
	x = (scale * (xx*cx + xy*cy + xz*cz)) + tx;
	y = (scale * (yx*cx + yy*cy + yz*cz)) + ty;
	z = (scale * (zx*cx + zy*cy + zz*cz)) + tz;
	return true;
}

bool TrackStarMatrix::reverse(double &x, double &y, double &z) {
	double cx = x - tx, cy = y - ty, cz = z - tz;
	x = (xx*cx + yx*cy + zx*cz) / scale;
	y = (xy*cx + yy*cy + zy*cz) / scale;
	z = (xz*cx + yz*cy + zz*cz) / scale;
	return true;
}

bool TrackStarMatrix::calcTranslation(double tsX, double tsY, double tsZ, double ogreX, double ogreY, double ogreZ) {
	double cx = tsX, cy = tsY, cz = tsZ;
	convert(cx, cy, cz);
	tx = ogreX - cx;
	ty = ogreY - cy;
	tz = ogreZ - cz;
	return true;
}

bool TrackStarMatrix::calcMatrix(double tsX1, double tsY1, double tsZ1, double ogreX1, double ogreY1, double ogreZ1,
								 double tsX2, double tsY2, double tsZ2, double ogreX2, double ogreY2, double ogreZ2) {

	// First calc inner matrix
	double dtsX = tsX2 - tsX1;
	double dtsY = tsY2 - tsY1;
	double dtsZ = tsZ2 - tsZ1;

	return false;
}





TrackStarFileReader::TrackStarFileReader(const char* filename, bool repeat) {
	trackData = NULL;
	trackDataLen = 0;
	pos = 0;

	FILE* file = fopen(filename, "r");
	if (file == NULL)
		return;

	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (len <= 0) {
		fclose(file);
		return;
	}

	char* data = new char[len+1];

	int res = (int)fread(data, 1, len, file);

	if (res <= 0) {
		delete [] data;
		fclose(file);
		return;
	}
	len = res;

	fclose(file);

	trackDataLen = res;
	data[len] = 0;
	trackData = data;
}

TrackStarFileReader::~TrackStarFileReader() {
	if (trackData) {
		delete [] trackData;
		trackData = NULL;
		trackDataLen = 0;
	}
}


bool TrackStarFileReader::getNextEntry(int &sensorID, double &x, double &y, double &z, double &a, double &e, double &r, int timeout) {
	if (trackData == NULL)
		return false;

	char* data = getNextLine();
	if (data == NULL) {
		pos = 0;
		data = getNextLine();
		if (data == NULL)
			return false;
	}
	
	char *src = data;
	sensorID = (int)strtod(src, &src);
	x = strtod(src, &src);
	y = strtod(src, &src);
	z = strtod(src, &src);
	a = strtod(src, &src);
	e = strtod(src, &src);
	r = strtod(src, &src);

	delete [] data;

	return true;
}

char* TrackStarFileReader::getNextLine() {
	// Find the next line end
	char* crPos = strchr(trackData+pos, 13);
	char* lfPos = strchr(trackData+pos, 10);
	if (crPos == NULL)
		crPos = lfPos;
	if (lfPos == NULL)
		lfPos = crPos;
	char* newPos = (crPos <= lfPos) ? crPos : lfPos;
	if (newPos == NULL)
		return NULL;

	int len = newPos - trackData - pos;
	char* data = new char[len+1];
	memcpy(data, trackData + pos, len);
	data[len] = 0;

	while (*newPos <= 32)
		newPos++;
	pos += newPos - trackData - pos;
	return data;
}




















TrackStarNetworkReader::TrackStarNetworkReader(const char* address, int port) {
	this->address = new char[strlen(address)+1];
	strcpy(this->address, address);
	this->port = port;
}

TrackStarNetworkReader::~TrackStarNetworkReader() {
}


bool TrackStarNetworkReader::getNextEntry(int &sensorID, double &x, double &y, double &z, double &a, double &e, double &r, int timeout) {
	char* data = getNextLine();
	if (data == NULL) {
		return false;
	}
	
	char *src = data;
	sensorID = (int)strtod(src, &src);
	x = strtod(src, &src);
	y = strtod(src, &src);
	z = strtod(src, &src);
	a = strtod(src, &src);
	e = strtod(src, &src);
	r = strtod(src, &src);

	delete [] data;

	return true;
}

char* TrackStarNetworkReader::getNextLine() {
	return NULL;
}





























TrackStarDirectReader::TrackStarDirectReader() {

	printf("\nTrackStar initialising, please wait (around 10 seconds)...\n\n");
	fflush(stdout);

	errorCode = InitializeBIRDSystem();
	if(errorCode!=BIRD_ERROR_SUCCESS) {
		printf(" --- TrackStar init FAILED: InitializeBIRDSystem()\n");
		fflush(stdout);
		return;
	}

	errorCode = GetBIRDSystemConfiguration(&ATC3DG.m_config);
	if(errorCode!=BIRD_ERROR_SUCCESS) {
		printf(" --- TrackStar init FAILED: GetBIRDSystemConfiguration()\n");
		fflush(stdout);
		return;
	}

	pSensor = new CSensor[ATC3DG.m_config.numberSensors];
	for(i=0;i<ATC3DG.m_config.numberSensors;i++)
	{
		errorCode = GetSensorConfiguration(i, &(pSensor+i)->m_config);
		if(errorCode!=BIRD_ERROR_SUCCESS) {
			printf(" --- TrackStar init FAILED: GetSensorConfiguration()\n");
			fflush(stdout);
			return;
		}
	}

	pXmtr = new CXmtr[ATC3DG.m_config.numberTransmitters];
	for(i=0;i<ATC3DG.m_config.numberTransmitters;i++)
	{
		errorCode = GetTransmitterConfiguration(i, &(pXmtr+i)->m_config);
		if(errorCode!=BIRD_ERROR_SUCCESS) {
			printf(" --- TrackStar init FAILED: GetTransmitterConfiguration()\n");
			fflush(stdout);
			return;
		}
	}

	for(id=0;id<ATC3DG.m_config.numberTransmitters;id++)
	{
		if((pXmtr+id)->m_config.attached)
		{
			// Transmitter selection is a system function.
			// Using the SELECT_TRANSMITTER parameter we send the id of the
			// transmitter that we want to run with the SetSystemParameter() call
			errorCode = SetSystemParameter(SELECT_TRANSMITTER, &id, sizeof(id));
			if(errorCode!=BIRD_ERROR_SUCCESS) {
				printf(" --- TrackStar init FAILED: SELECT_TRANSMITTER\n");
				fflush(stdout);
				return;
			}
			break;
		}
	}

	printf("TrackStar initialisation SUCCESS!\n\n");
}

TrackStarDirectReader::~TrackStarDirectReader() {
	id = -1;
	errorCode = SetSystemParameter(SELECT_TRANSMITTER, &id, sizeof(id));
	delete[] pSensor;
	delete[] pXmtr;
}

bool TrackStarDirectReader::getNextEntry(int &sensorID, double &x, double &y, double &z, double &a, double &e, double &r, int timeout) {

	DOUBLE_POSITION_ANGLES_RECORD record, *pRecord = &record;

	// scan the sensor and request a record
	// sensor attached so get record
	errorCode = GetAsynchronousRecord(sensorID, pRecord, sizeof(record));
	if(errorCode==BIRD_ERROR_SUCCESS) {

		// get the status of the last data record
		// only report the data if everything is okay
		unsigned int status = GetSensorStatus(sensorID);

		if( status == VALID_STATUS)
		{
			x = record.x;
			y = record.y;
			z = record.z;
			a = record.a;
			e = record.e;
			r = record.r;
			return true;
		}
	}
	return false;
}
