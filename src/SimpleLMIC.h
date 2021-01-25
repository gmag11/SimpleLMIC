﻿// ---------------------------------------------------- //
// SimpleLMIC.h
// ---------------------------------------------------- //

#ifndef SIMPLE_LMIC_H
#define SIMPLE_LMIC_H

#include <Arduino.h>
#include "lmic.h"
#include "hal/hal.h"
#include "arduino_lmic_hal_boards.h"

// ---------------------------------------------------- //
// define
// ---------------------------------------------------- //

#define LORA_HTOI(c) ((c <= '9') ? (c - '0') : ((c <= 'F') ? (c - 'A' + 10) : ((c <= 'f') ? (c - 'a' + 10) : (0))))
#define LORA_TWO_HTOI(h, l) ((LORA_HTOI(h) << 4) + LORA_HTOI(l))
#define LORA_HEX_TO_BYTE(a, h, n)                           \
	{                                                       \
		for (int i = 0; i < n; i++)                         \
			(a)[i] = LORA_TWO_HTOI(h[2 * i], h[2 * i + 1]); \
	}
#define LORA_DEVADDR(a) (uint32_t)((uint32_t)(a)[3] | (uint32_t)(a)[2] << 8 | (uint32_t)(a)[1] << 16 | (uint32_t)(a)[0] << 24)

#define SimpleLMIC SimpleLMICClass

// ---------------------------------------------------- //
// class
// ---------------------------------------------------- //

class SimpleLMICClass : public Stream
{
private:
	// ----------------------------------------------- //
	// 	buffer
	uint8_t bufferPosition = 0;
	// ----------------------------------------------- //
public:
	// ----------------------------------------------- //
	//	degub event
	bool _debug = false;
	// ----------------------------------------------- //
	// 	LoraWan OTAA
	uint8_t DevEui[8];
	uint8_t AppEui[8];
	uint8_t AppKey[16];
	// ----------------------------------------------- //

	SimpleLMICClass();

	bool begin();
	bool begin(const void *pPinMap);

	void join();
	void join(const char *_aeui, const char *_akey);
	void join(const char *_deui, const char *_aeui, const char *_akey);

	void setLink(bool link = true);
	void setAdr(bool adr = true);
	void setSubBand(uint8_t band = 1);
	void setError(uint8_t error = 5);

	bool isBusy();
	bool isLink();

	void debug(bool _bug = true);

	void loop();
	void sleep();
	void end();

	virtual size_t write(uint8_t byte);
	virtual size_t write(const uint8_t *buffer, size_t size);
	virtual int available();
	virtual int read();
	virtual int peek();
	virtual void flush();

	void send(uint8_t port = 1, bool confirm = false);

	void clear();
	uint8_t *buffer();

	int16_t read16();
	int32_t read24();
	int32_t read32();

	unsigned int readInt();
	unsigned long readLong();

	void onMessage(void (*cb)(uint8_t *payload, size_t size, uint8_t port));

	void setAppKey(uint8_t *_akey);
	void setAppKey(const char *_akey);
	void setAppEui(uint8_t *_aeui);
	void setAppEui(const char *_aeui);
	void setDevEui(uint8_t *_deui);
	void setDevEui(const char *_deui);

	void personalize(const char * devAddr, const char * nwkSKey, const char * appSKey);

	void message(uint8_t *payload, size_t size, uint8_t port);
    
    void onJoin (void(*cb)(uint32_t netid, uint32_t devaddr, uint8_t* nwkKey, uint8_t* artKey));
    void joined (uint32_t netid, uint32_t devaddr, uint8_t* nwkKey, uint8_t* artKey);

private:
	void (*messageCallback)(uint8_t *payload, size_t size, uint8_t port);
    void (*nodeJoined)(uint32_t netid, uint32_t devaddr, uint8_t* nwkKey, uint8_t* artKey);
	void (*sleepCallback)();
};

#endif
