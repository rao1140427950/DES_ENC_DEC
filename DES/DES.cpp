#include "pch.h"
#include <iostream>

typedef unsigned char uint8;

using namespace std;

const uint8 first_swap_table[64] = {
	58, 50, 42, 34, 26, 18, 10,  2,
	60,	52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6,
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1,
	59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5,
	63, 55, 47, 39, 31, 23, 15,  7
};

const uint8 Ctable[28] = {
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36
};

const uint8 Dtable[28] = {
	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

const uint8 PC2_table[48] = 
{
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

const uint8 move_bits[16] = {
	1, 1, 2, 2,
	2, 2, 2, 2,
	1, 2, 2, 2,
	2, 2, 2, 1
};

//uint8 data[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
//uint8 key[7] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x77, 0x88 };
uint8 dat[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
uint8 key[7] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };

void generate_key_full(uint8 *key_input, uint8 *key_output);
void convert2bin(uint8 *dat, uint8 *dat_bin);
void do_first_swap(uint8 *dat, uint8 *Ldata, uint8 *Rdata);
void get_CDkey(uint8 *key, uint8 *Ckey, uint8 *Dkey);
void move_left(uint8 *dat, bool reverse = false);
void combine_CDkey(uint8 *Ckey, uint8 *Dkey, uint8 *key_48bits);

int main()
{
	uint8 key_full[8];
	uint8 data_binary[64];
	uint8 key_binary[64];
	uint8 Ldata[32];
	uint8 Rdata[32];
	uint8 Ckey[28];
	uint8 Dkey[28];
	uint8 key_48bits[48];

	generate_key_full(key, key_full);
	convert2bin(dat, data_binary);
	convert2bin(key_full, key_binary);
	get_CDkey(key_full, Ckey, Dkey);

	for (uint8 i = 0; i < 16; i++)
	{
		for (uint8 j = 0; j < move_bits[i]; j++)
		{
			move_left(Ckey);
			move_left(Dkey);
		}
		combine_CDkey(Ckey, Dkey, key_48bits);
	}
	
	

	return 0;
}

void combine_CDkey(uint8 *Ckey, uint8 *Dkey, uint8 *key_48bits)
{
	uint8 temp[56];

	for (uint8 i = 0; i < 28; i++) temp[i] = Ckey[i];
	for (uint8 i = 0; i < 28; i++) temp[i + 28] = Dkey[i];
	for (uint8 i = 0; i < 48; i++) key_48bits[i] = temp[PC2_table[i]];
}

void move_left(uint8 *dat, bool reverse)
{
	uint8 temp;

	if (reverse)
	{
		temp = dat[27];
		for (uint8 i = 27; i > 0; i--) dat[i] = dat[i - 1];
		dat[0] = temp;
	}
	else
	{
		temp = dat[0];
		for (uint8 i = 0; i < 28; i++) dat[i] = dat[i + 1];
		dat[27] = temp;
	}
	
}

void get_CDkey(uint8 *key, uint8 *Ckey, uint8 *Dkey)
{
	for (uint8 i = 0; i < 28; i++) Ckey[i] = key[Ctable[i]];
	for (uint8 i = 0; i < 28; i++) Dkey[i] = key[Dtable[i]];
}

void do_first_swap(uint8 *dat, uint8 *Ldata, uint8 *Rdata)
{
	for (uint8 i = 0; i < 32; i++) Ldata[i] = dat[first_swap_table[i]];
	for (uint8 i = 0; i < 32; i++) Rdata[i] = dat[first_swap_table[i + 32]];
}

void convert2bin(uint8 *dat, uint8 *dat_bin)
{
	for (uint8 i = 0; i < 8; i++)
	{
		for (uint8 j = 0; j < 8; j++)
		{
			dat_bin[i * 8 + j] = (dat[i] >> (7 - j)) & 0x01;
		}
	}
}

void generate_key_full(uint8 *key_input, uint8 *key_output)
{
	for (uint8 i = 0; i < 8; i++)
	{
		uint8 temp = 1;
		uint8 bit = 0;

		key_output[i] = 0;
		for (uint8 j = 0; j < 7; j++)
		{
			bit = (key_input[j] >> (7 - i)) & 0x01;
			key_output[i] |= (bit << (7 - j));
			temp ^= bit;
		}
		key_output[i] |= temp;
	}
}
