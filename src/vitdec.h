#pragma once

#include <vector>
#include <algorithm>
#include <deque>

typedef std::vector<bool> bool_vect;

class Encoder
{
public:

	bool_vect encode(const bool_vect &input);

private:
	const uint8_t R = 2;
	const uint8_t P = 2;

};



class Decoder
{
public:

	bool_vect decode(const bool_vect &input);
	
private:

	const uint8_t R = 2;
	const uint8_t P = 2;
	const uint8_t NP = 3;

	const uint8_t I = 4;


	std::deque<uint8_t> currnet_way;
	bool_vect current_decoded_sequence;

	std::deque <std::deque<std::deque<uint8_t>>> ways;
	std::deque <std::deque<bool_vect>> decoded_sequences;

	std::deque<std::deque<uint32_t>> costs_ways;
	std::deque<uint32_t> count_of_ways;

	bool_vect bits;
	bool_vect inpt;


	const int core[4][4] = {
		{ 0b00, -1, 0b11, -1 },
		{ 0b11, -1, 0b00, -1 },
		{ -1, 0b01, -1, 0b10 },
		{ -1, 0b10, -1, 0b01 } };



	void build_ways(uint8_t point, int current_deep, int max_deep);
	void delete_ways();
	void way_to_bits(bool_vect &bits, const std::deque<uint8_t> &way);
	uint32_t hamming_distance(const bool_vect x, const bool_vect y, uint32_t len);


};