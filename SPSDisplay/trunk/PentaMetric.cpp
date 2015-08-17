//
//  PentaMetric.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 8/14/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "PentaMetric.h"

#include <errno.h>
#include <math.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "SPSUtil.h"



PentaMetric::PentaMetric(const std::string& inPath)
	:
	mPath(inPath),
	mFD(-1)
{
	resetValues();
}

void
PentaMetric::resetValues()
{
	mVoltage = 0.0;
	mCurrent = 0.0;
	mPower = 0.0;
	mAmpHours = 0.0;
	mSOC = 0.0;
}

void
PentaMetric::open()
{
	int fd = ::open(mPath.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0)
	{
		LZLogDebug("Error opening PentaMetric port (%s): %s\n", mPath.c_str(), strerror(errno));
		return;
	}
	
	//	Disallow multiple opens…
	
	if (::ioctl(fd, TIOCEXCL) == -1)
	{
		LZLogDebug("Error setting TIOCEXCL: %s (%d)\n", ::strerror(errno), errno);
		::close(fd);
		return;
	}
	
	//	Set blocking on the port…
	
#if 1
	if (::fcntl(fd, F_SETFL, 0) == -1)
	{
		LZLogDebug("Error clearing O_NONBLOCK %s(%d).\n", ::strerror(errno), errno);
		::close(fd);
		return;
	}
#endif

	//	Get the current options…
	
	struct termios		options;
	if (::tcgetattr(fd, &options) == -1)
	{
		LZLogDebug("Error getting termios %s(%d).\n", ::strerror(errno), errno);
		if (fd != -1)		::close(fd);
		return;
	}
	
	//	Set raw (non-canonical) mode…
	
	::cfmakeraw(&options);
	options.c_cc[VMIN] = 1;
	options.c_cc[VTIME] = 0;

	//LZLogDebug("Current options: 0x%08lX\n", options.c_cflag);
	::cfsetspeed(&options, B2400);
	
	options.c_cflag |= CS8;
	//options.c_cflag |= PARENB;
	//options.c_cflag |= PARODD;
	//options.c_cflag |= (CCTS_OFLOW | CRTS_IFLOW);
	//LZLogDebug("Current options: 0x%08lX\n", options.c_cflag);
	
	int err = ::tcsetattr(fd, TCSANOW, &options);
	if (err != 0)
	{
		LZLogDebug("Error calling tcsetattr: %d, %d, %s\n", err, errno, strerror(errno));
		return;
	}
	
	//	Clear the buffers…
	
	if (::tcflush(fd, TCIOFLUSH))
	{
		LZLogDebug("Error calling tcflush: %d, %d, %s\n", err, errno, strerror(errno));
	}
	
	mFD = fd;
}

void
PentaMetric::readVoltage1()
{
	if (mFD <= 0) return;
	
	uint8_t		buf[] = { 0x81, 0x01, 0x02, 0x7B };
	ssize_t count = ::write(mFD, buf, 4);
	if (count < 1)
	{
		LZLogDebug("Error writing PentaMetric: %d, %d, %s\n", (int32_t) count, errno, strerror(errno));
		return;
	}
	
	mVoltage = readFormat1();
	LZLogDebug("Voltage: %4.1f V\n", mVoltage);
}

void
PentaMetric::readCurrent1()
{
	if (mFD <= 0) return;
	
	uint8_t		buf[] = { 0x81, 0x05, 0x03, 0x76 };
	ssize_t count = ::write(mFD, buf, 4);
	if (count < 1)
	{
		LZLogDebug("Error writing PentaMetric: %ld, %d, %s\n", count, errno, strerror(errno));
		return;
	}
	
	mCurrent = readFormat2();
	LZLogDebug("Current: %4.1f A\n", mCurrent);
}

void
PentaMetric::readPower1()
{
	if (mFD <= 0) return;
	
	uint8_t		buf[] = { 0x81, 0x17, 0x03, 0x64 };
	ssize_t count = ::write(mFD, buf, 4);
	if (count < 1)
	{
		LZLogDebug("Error writing PentaMetric: %ld, %d, %s\n", count, errno, strerror(errno));
		return;
	}
	
	mPower = readFormat2();
	LZLogDebug("Power: %4.1f W\n", mPower);
}

void
PentaMetric::readAmpHours1()
{
	if (mFD <= 0) return;
	
	uint8_t		buf[] = { 0x81, 0x0C, 0x03, 0x6f };
	ssize_t count = ::write(mFD, buf, 4);
	if (count < 1)
	{
		LZLogDebug("Error writing PentaMetric: %ld, %d, %s\n", count, errno, strerror(errno));
		return;
	}
	
	mAmpHours = readFormat3();
	LZLogDebug("Amp•Hours: %4.1f Ah\n", mAmpHours);
}

void
PentaMetric::readSOC1()
{
	if (mFD <= 0) return;
	
	uint8_t		buf[] = { 0x81, 0x1A, 0x01, 0x63 };
	ssize_t count = ::write(mFD, buf, 4);
	if (count < 1)
	{
		LZLogDebug("Error writing PentaMetric: %ld, %d, %s\n", count, errno, strerror(errno));
		return;
	}
	
	mSOC = readFormat6();
	LZLogDebug("SOC:    %3.0f   %%\n", mSOC);
}

double
PentaMetric::readFormat1()
{
	if (mFD <= 0) return 0;
	
	uint8_t buf[3];
	ssize_t count = ::read(mFD, buf, sizeof (buf));
	if (count < (ssize_t) sizeof (buf))
	{
		LZLogDebug("PentaMetric::readFormat1: Error reading: %ld, %d, %s\n", count, errno, strerror(errno));
		return ::nan("");
	}
	
	//LZLogDebug("Read bytes 0x%02x 0x%02x 0x%02x\n", buf[0], buf[1], buf[2]);
	
	if (verifyChecksum(buf, count))
	{
		uint16_t val = *reinterpret_cast<uint16_t*>(buf);
		return val / 20.0;
	}
	
	return ::nan("");
}

double
PentaMetric::readFormat2()
{
	if (mFD <= 0) return 0;
	
	uint8_t buf[4];
	ssize_t count = ::read(mFD, buf, sizeof (buf));
	if (count < (ssize_t) sizeof (buf))
	{
		LZLogDebug("PentaMetric::readFormat2: Error reading: %ld, %d, %s\n", count, errno, strerror(errno));
		return ::nan("");
	}
	
	//LZLogDebug("Read bytes 0x%02x 0x%02x 0x%02x\n", buf[0], buf[1], buf[2]);
	
	if (verifyChecksum(buf, count))
	{
		uint32_t val = 0;
		val = buf[0] | (buf[1] << 8) | buf[2] << 16;
		if (((val >> 23) & 0x1) == 1)
		{
			val = ~val;
			val &= 0x00FFFFFF;
			return val / -100.0;
		}
		
		return val / 100.0;
	}
	
	return ::nan("");
}

double
PentaMetric::readFormat2B()
{
	return ::nan("");
}

double
PentaMetric::readFormat3()
{
	if (mFD <= 0) return 0;
	
	uint8_t buf[4];
	ssize_t count = ::read(mFD, buf, sizeof (buf));
	if (count < (ssize_t) sizeof (buf))
	{
		LZLogDebug("PentaMetric::readFormat2: Error reading: %ld, %d, %s\n", count, errno, strerror(errno));
		return ::nan("");
	}
	
	//LZLogDebug("Read bytes 0x%02x 0x%02x 0x%02x\n", buf[0], buf[1], buf[2]);
	
	if (verifyChecksum(buf, count))
	{
		uint32_t val = 0;
		val = buf[0] | (buf[1] << 8) | buf[2] << 16;
		return val / 100.0;
	}
	
	return ::nan("");
}

double
PentaMetric::readFormat5()
{
	return ::nan("");
}

double
PentaMetric::readFormat6()
{
	uint8_t buf[2];
	ssize_t count = ::read(mFD, buf, sizeof (buf));
	if (count < (ssize_t) sizeof (buf))
	{
		LZLogDebug("PentaMetric::readFormat6: Error reading: %ld, %d, %s\n", count, errno, strerror(errno));
		return ::nan("");
	}
	
	//LZLogDebug("Read bytes 0x%02x 0x%02x 0x%02x\n", buf[0], buf[1], buf[2]);
	
	if (verifyChecksum(buf, count))
	{
		uint8_t val = *reinterpret_cast<uint8_t*>(buf);
		return val;
	}
	
	return ::nan("");
}

bool
PentaMetric::verifyChecksum(const void* inBuffer, size_t inLen)
{
	const uint8_t* buf = reinterpret_cast<const uint8_t*> (inBuffer);
	uint8_t sum = 0;
	for (size_t i = 0; i < inLen; ++i)
	{
		sum += buf[i];
	}
	if (sum != 0xFF)
	{
		LZLogDebug("Check sum fail: 0x%02x\n", sum);
		return false;
	}
	
	return true;
}


