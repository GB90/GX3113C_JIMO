/**
 *
 * @file        app_tr_cas_api_crc32.c
 * @brief
 * @version     1.1.0
 * @date        12/29/2014 
 * @author      wangjian (wj), wangjian@nationalchip.com
 *
 */

#include "app_tr_cas_api_crc32.h"

static int32_t width;
static uint32_t poly;
static uint32_t initVal;
static uint32_t refin;
static uint32_t refout;
static uint32_t xorot;
static uint32_t reg;
static uint32_t widthMask;
static uint32_t crcTable[256];

static uint32_t s_bTableInited = 0;

static uint32_t CRCCreateTableEntry(uint32_t i);
static uint32_t CRCReflect(uint32_t byte, int32_t len);


/*----------------------------------------------------------------*/
static uint32_t CRCReflect(uint32_t byte, int32_t len)
{
	int32_t i;
	uint32_t temp = byte;

	for (i = 1; i <= len; i++)
	{
		if (temp & 0x1)
		{
			byte |= (1 << (len-i));
		}
		else
		{
			byte &= ~(1 << (len-i));
		}
		
		temp >>= 1;
	}
	
	return byte;
}

static uint32_t CRCCreateTableEntry(uint32_t byte)
{
	uint32_t r;
	uint32_t topbit;
	uint32_t i;

	if (refin)
	{
		byte = CRCReflect(byte,8);
	}

	r = byte << (width-8);
	topbit = 1 << (width-1);

	for (i = 0; i < 8; i++)
	{
		if (r & topbit)
		{
			r = (r << 1) ^ poly;
		}
		else
		{
			r <<= 1;
		}
	}

	if (refin)
	{
		r = CRCReflect(r, width);
	}

	return (r & widthMask);
}

void app_tr_cas_crc32_init(int32_t w, uint32_t p, uint32_t in, 
      						uint32_t ri, uint32_t ro, uint32_t xo)
{
	uint32_t i;

	if (1 == s_bTableInited)
	{
		if ((width == w) 
			&& (poly == p) 
			&& (initVal == in) 
			&& (refin == ri) 
			&& (refout == ro) 
			&& (xorot == xo))
		{
			app_tr_cas_crc32_reset();
			return;
		} 
	}

	width = w;
	poly = p;
	initVal = in;
	refin = ri;
	refout = ro;
	xorot = xo;

	if (width < 32)
	{
		widthMask = ((1 << width) - 1);
	}
	else
	{
		widthMask = 0xFFFFFFFF;
	}

	for (i = 0; i < 256; i++)
	{
		crcTable[i] = CRCCreateTableEntry(i);
	}

	app_tr_cas_crc32_reset();
	s_bTableInited = 1;

	return;
}

void app_tr_cas_crc32_reset(void)
{
	reg = initVal;
}

void app_tr_cas_crc32_nextbyte(uint8_t ch)
{
	if (refout)
	{
		reg = crcTable[(reg ^ ch) & 0xFF] ^ (reg >> 8);
	}
	else
	{
		reg = crcTable[((reg >> 24) ^ ch) & 0xFF] ^ (reg << 8);
	}

	return;
}

uint32_t app_tr_cas_crc32_get_result(void)
{
	uint32_t crc = (xorot ^ reg);

	app_tr_cas_crc32_reset();

	return crc;
}

