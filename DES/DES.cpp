#include "pch.h"
#include <iostream>
#include <iomanip>

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

const uint8 inv_swap_table[64] = {
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27, 
	34,  2, 42, 10, 50, 18, 58, 26, 
	33,  1, 41,  9, 49, 17, 57, 25
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

const uint8 move_bits_inv[16] = {
	0, 1, 2, 2,
	2, 2, 2, 2,
	1, 2, 2, 2, 
	2, 2, 2, 1
};

const uint8 expandR_table[48] = {
	32,  1,  2,  3,  4,  5,
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

const uint8 S_box[8][4][16] = {
	{{ 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7 },
	 {  0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8 },
	 {  4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0 },
	 { 15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13 }},

	{{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
	 { 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
	 {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
	 {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9} },

	 {{ 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
	  {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
	  {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
      {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }},

	  {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
	   {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
	   {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
	   { 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},

	  {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
	   {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
	   { 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
	   {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},

	  {{ 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
	   {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
	   { 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
	   { 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }},

	  {{ 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
	   {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
	   { 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
	   { 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }},

	  {{ 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
	   { 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
	   { 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
	   { 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }}

};

const uint8 P_box[32] = {
	16,7,20,21,
	29,12,28,17,
	1,15,23,26,
	5,18,31,10,
	2,8,24,14,
	32,27,3,9,
	19,13,30,6,
	22,11,4,25
};

//uint8 data[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
//uint8 key[7] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x77, 0x88 };
uint8 dat[8] = { 0x16, 0x71, 0x10, 0x83, 0x00, 0x00, 0x00, 0x00 };
uint8 key[7] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void encrypt(uint8 *dat_8bytes, uint8 *key_7bytes, uint8 *dat_enc_8bytes);
void decrypt(uint8 *dat_8bytes, uint8 *key_7bytes, uint8 *dat_dec_8bytes);

void generate_key_full(uint8 *key_input, uint8 *key_output);
void convert2bin(uint8 *dat, uint8 *dat_bin);
void do_first_swap(uint8 *dat, uint8 *Ldata, uint8 *Rdata);
void get_CDkey(uint8 *key, uint8 *Ckey, uint8 *Dkey);
void move_left(uint8 *dat, bool reverse = false);
void combine_CDkey(uint8 *Ckey, uint8 *Dkey, uint8 *key_48bits);
void expand_Rdata(uint8 *Rdata_32bits, uint8 *Rdata_48bits);
void dat_xor_key(uint8 *key_48bits, uint8 *dat_48bits, uint8 *res);
void calc_SPbox(uint8 *dat_48bits, uint8 *dat_32bits);
void calc_LRdata(uint8 *Rdata, uint8 *Ldata, uint8 *SPbox_res);
void do_inv_swap(uint8 *Ldata, uint8 *Rdata, uint8 *dat_enc_bin);
void convert2hex(uint8 *dat_bin, uint8 *dat_hex);

int main()
{
	uint8 encrypt_text[8];
	uint8 decrypt_text[8];

	encrypt(dat, key, encrypt_text);
	decrypt(encrypt_text, key, decrypt_text);
	cout << "The orignal data is: ";
	for (uint8 i = 0; i < 8; i++) cout << setw(2) << setfill('0') << hex << int(dat[i]);
	cout << endl;
	cout << "The key is: ";
	for (uint8 i = 0; i < 7; i++) cout << setw(2) << setfill('0') << hex << int(key[i]);
	cout << endl;
	cout << "The encrypt data is: ";
	for (uint8 i = 0; i < 8; i++) cout << setw(2) << setfill('0') << hex << int(encrypt_text[i]);
	cout << endl;
	cout << "The decrypt data is: ";
	for (uint8 i = 0; i < 8; i++) cout << setw(2) << setfill('0') << hex << int(decrypt_text[i]);
	cout << endl;

	return 0;
}

void decrypt(uint8 *dat_8bytes, uint8 *key_7bytes, uint8 *dat_dec_8bytes)
{
	uint8 key_full[8];
	uint8 data_binary[64];
	uint8 key_binary[64];
	uint8 Ldata[32];
	uint8 Rdata[32];
	uint8 Ckey[28];
	uint8 Dkey[28];
	uint8 key_48bits[48];
	uint8 Rdata_48bits[48];
	uint8 xor_res[48];
	uint8 SPbox_res[32];

	generate_key_full(key_7bytes, key_full);
	convert2bin(dat_8bytes, data_binary);
	convert2bin(key_full, key_binary);
	get_CDkey(key_binary, Ckey, Dkey);
	do_first_swap(data_binary, Ldata, Rdata);

	for (uint8 i = 0; i < 16; i++)
	{
		for (uint8 j = 0; j < move_bits_inv[i]; j++)
		{
			move_left(Ckey, true);
			move_left(Dkey, true);
		}
		combine_CDkey(Ckey, Dkey, key_48bits);
		expand_Rdata(Rdata, Rdata_48bits);
		dat_xor_key(key_48bits, Rdata_48bits, xor_res);
		calc_SPbox(xor_res, SPbox_res);
		calc_LRdata(Rdata, Ldata, SPbox_res);
	}

	do_inv_swap(Ldata, Rdata, data_binary);
	convert2hex(data_binary, dat_dec_8bytes);
}

void encrypt(uint8 *dat_8bytes, uint8 *key_7bytes, uint8 *dat_enc_8bytes)
{
	uint8 key_full[8];
	uint8 data_binary[64];
	uint8 key_binary[64];
	uint8 Ldata[32];
	uint8 Rdata[32];
	uint8 Ckey[28];
	uint8 Dkey[28];
	uint8 key_48bits[48];
	uint8 Rdata_48bits[48];
	uint8 xor_res[48];
	uint8 SPbox_res[32];

	generate_key_full(key_7bytes, key_full);
	convert2bin(dat_8bytes, data_binary);
	convert2bin(key_full, key_binary);
	get_CDkey(key_binary, Ckey, Dkey);
	do_first_swap(data_binary, Ldata, Rdata);

	for (uint8 i = 0; i < 16; i++)
	{
		for (uint8 j = 0; j < move_bits[i]; j++)
		{
			move_left(Ckey);
			move_left(Dkey);
		}
		combine_CDkey(Ckey, Dkey, key_48bits);
		expand_Rdata(Rdata, Rdata_48bits);
		dat_xor_key(key_48bits, Rdata_48bits, xor_res);
		calc_SPbox(xor_res, SPbox_res);
		calc_LRdata(Rdata, Ldata, SPbox_res);
	}

	do_inv_swap(Ldata, Rdata, data_binary);
	convert2hex(data_binary, dat_enc_8bytes);
}

void convert2hex(uint8 *dat_bin, uint8 *dat_hex)
{
	for (uint8 i = 0; i < 8; i++)
	{
		dat_hex[i] = 0;
		dat_hex[i] |= (dat_bin[i * 8] << 7);
		dat_hex[i] |= (dat_bin[i * 8 + 1] << 6);
		dat_hex[i] |= (dat_bin[i * 8 + 2] << 5);
		dat_hex[i] |= (dat_bin[i * 8 + 3] << 4);
		dat_hex[i] |= (dat_bin[i * 8 + 4] << 3);
		dat_hex[i] |= (dat_bin[i * 8 + 5] << 2);
		dat_hex[i] |= (dat_bin[i * 8 + 6] << 1);
		dat_hex[i] |= dat_bin[i * 8 + 7];
	}
}

void do_inv_swap(uint8 *Ldata, uint8 *Rdata, uint8 *dat_enc_bin)
{
	for (uint8 i = 0; i < 64; i++)
	{
		if (inv_swap_table[i] - 1 < 32) dat_enc_bin[i] = Rdata[inv_swap_table[i] - 1];
		else dat_enc_bin[i] = Ldata[inv_swap_table[i] - 1 - 32];
	}
}

void calc_LRdata(uint8 *Rdata, uint8 *Ldata, uint8 *SPbox_res)
{
	for (uint8 i = 0; i < 32; i++) SPbox_res[i] ^= Ldata[i];
	for (uint8 i = 0; i < 32; i++) Ldata[i] = Rdata[i];
	for (uint8 i = 0; i < 32; i++) Rdata[i] = SPbox_res[i];
}

void calc_SPbox(uint8 *dat_48bits, uint8 *dat_32bits)
{
	uint8 temp[32];

	for (uint8 i = 0; i < 8; i++)
	{
		uint8 x = 0;
		uint8 y = 0;
		uint8 s = 0;

		x |= (dat_48bits[i * 6] << 1);
		x |= dat_48bits[i * 6 + 5];
		y |= (dat_48bits[i * 6 + 1] << 3);
		y |= (dat_48bits[i * 6 + 2] << 2);
		y |= (dat_48bits[i * 6 + 3] << 1);
		y |= dat_48bits[i * 6 + 4];
		s = S_box[i][x][y];
		temp[i * 4] = (s & 0x08) >> 3;
		temp[i * 4 + 1] = (s & 0x04) >> 2;
		temp[i * 4 + 2] = (s & 0x02) >> 1;
		temp[i * 4 + 3] = s & 0x01;
	}
	for (uint8 i = 0; i < 32; i++) dat_32bits[i] = temp[P_box[i] - 1];
}

void dat_xor_key(uint8 *key_48bits, uint8 *dat_48bits, uint8 *res)
{
	for (uint8 i = 0; i < 48; i++) res[i] = key_48bits[i] ^ dat_48bits[i];
}

void expand_Rdata(uint8 *Rdata_32bits, uint8 *Rdata_48bits)
{
	for (uint8 i = 0; i < 48; i++) Rdata_48bits[i] = Rdata_32bits[expandR_table[i] - 1];
}

void combine_CDkey(uint8 *Ckey, uint8 *Dkey, uint8 *key_48bits)
{
	uint8 temp[56];

	for (uint8 i = 0; i < 28; i++) temp[i] = Ckey[i];
	for (uint8 i = 0; i < 28; i++) temp[i + 28] = Dkey[i];
	for (uint8 i = 0; i < 48; i++) key_48bits[i] = temp[PC2_table[i] - 1];
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
	for (uint8 i = 0; i < 28; i++) Ckey[i] = key[Ctable[i] - 1];
	for (uint8 i = 0; i < 28; i++) Dkey[i] = key[Dtable[i] - 1];
}

void do_first_swap(uint8 *dat, uint8 *Ldata, uint8 *Rdata)
{
	for (uint8 i = 0; i < 32; i++) Ldata[i] = dat[first_swap_table[i] - 1];
	for (uint8 i = 0; i < 32; i++) Rdata[i] = dat[first_swap_table[i + 32] - 1];
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
