#pragma once

#include <stdio.h>
#include "ATC3DG.h"

class CSystem
{
public:
	SYSTEM_CONFIGURATION	m_config;
}; 

class CSensor
{
public:
	SENSOR_CONFIGURATION	m_config;
};

class CXmtr
{
public:
	TRANSMITTER_CONFIGURATION	m_config;
};


class TrackStarMatrix {
public:
	TrackStarMatrix();
	TrackStarMatrix(double scale, double xx, double xy, double xz, double yx, double yy, double yz, double zx, double zy, double zz, double tx, double ty, double tz);

	bool convert(double &x, double &y, double &z);
	bool reverse(double &x, double &y, double &z);

	bool calcMatrix(double tsX1, double tsY1, double tsZ1, double ogreX1, double ogreY1, double ogreZ1,
	                double tsX2, double tsY2, double tsZ2, double ogreX2, double ogreY2, double ogreZ2);
	
	bool calcTranslation(double tsX, double tsY, double tsZ, double ogreX, double ogreY, double ogreZ);


	double scale;
	double xx, xy, xz;
	double yx, yy, yz;
	double zx, zy, zz;
	double tx, ty, tz;
};


class TrackStarInterface {
public:
	TrackStarInterface() {}
	virtual ~TrackStarInterface() {}

	virtual bool getNextEntry(int &sensorID, double &x, double &y, double &z, double &a, double &e, double &r, int timeout) = 0;
};


class TrackStarFileReader : public TrackStarInterface {
public:
	TrackStarFileReader(const char* filename, bool repeat);
	virtual ~TrackStarFileReader();

	virtual bool getNextEntry(int &sensorID, double &x, double &y, double &z, double &a, double &e, double &r, int timeout);

protected:
	char* getNextLine();

	char* trackData;
	int trackDataLen;
	int pos;
};


class TrackStarNetworkReader : public TrackStarInterface {
public:
	TrackStarNetworkReader(const char* address, int port);
	virtual ~TrackStarNetworkReader();

	virtual bool getNextEntry(int &sensorID, double &x, double &y, double &z, double &a, double &e, double &r, int timeout);

protected:
	char* getNextLine();

	char* address;
	int port;
	// socket;
};


class TrackStarDirectReader : public TrackStarInterface {
public:
	TrackStarDirectReader();
	virtual ~TrackStarDirectReader();

	virtual bool getNextEntry(int &sensorID, double &x, double &y, double &z, double &a, double &e, double &r, int timeout);

protected:
	CSystem			ATC3DG;
	CSensor			*pSensor;
	CXmtr			*pXmtr;

	int				errorCode;
	int				i;
	int				sensorID;
	short			id;
};
