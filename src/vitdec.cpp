#include "vitdec.h"


bool_vect Encoder::encode(const bool_vect &input)
{
	//G1 = 1 + x^2
	//G2 = 1 + x + x^2

	if (input.size() == 0)
		return {};
	if (input.size() < P)
		throw std::length_error("Sequence too short");

	bool_vect output(input.size() * R);
	bool_vect buf = input;
	buf.insert(buf.begin(), P, false);

	auto t = 0;
	for (auto i = 0; i < input.size(); ++i)
	{
		output[t++] = buf[P + i] ^ buf[P + i - 2];
		output[t++] = buf[P + i] ^ buf[P + i - 1] ^ buf[P + i - 2];
	}

	return output;
}


bool_vect Decoder::decode(const bool_vect &input)
{
	if (input.size() == 0)
		return {};
	if (input.size() < P*R)
		throw std::length_error("Sequence too short");

	inpt = input;

	size_t output_size = input.size() / R;

	currnet_way.resize(output_size);
	current_decoded_sequence.resize(output_size);

	ways.resize(I);
	decoded_sequences.resize(I);
	costs_ways.resize(I);

	build_ways(0, 0, NP);
	delete_ways();

	for (auto k = NP; k < output_size; ++k)
	{

		for (auto i = 0; i < I; ++i)
			count_of_ways.push_back(ways.at(i).size());

		for (auto i = 0; i < I; ++i)
		{
			__int32 current_size_count_of_way = count_of_ways.at(i) - 1;
 			for (__int32 j = current_size_count_of_way; j >= 0; --j)
			{
				currnet_way = ways.at(i).at(j);
				current_decoded_sequence = decoded_sequences.at(i).at(j);
				ways.at(i).erase(ways.at(i).begin() + j);
				decoded_sequences.at(i).erase(decoded_sequences.at(i).begin() + j);
				costs_ways.at(i).erase(costs_ways.at(i).begin() + j);
				build_ways(i, k, k + 1);
			}
		}

		count_of_ways.clear();
		delete_ways();
	}

	size_t min = std::numeric_limits<size_t>::max();
	auto ind_i = 0;
	auto ind_j = 0;

	for (auto i = 0; i < I; ++i)
	{
		__int32 current_size_cost_way = costs_ways.at(i).size() - 1;
		for (__int32 j = current_size_cost_way; j >= 0; --j)
			if (min > costs_ways.at(i).at(j))
			{
				min = costs_ways.at(i).at(j);
				ind_i = i;
				ind_j = j;
			}
	}

	return decoded_sequences.at(ind_i).at(ind_j);
}


void Decoder::build_ways(uint8_t point, int current_deep, int max_deep)
{
	if (current_deep < max_deep)
	{
		for (auto i = 0; i < I; ++i)
			if (core[point][i] >= 0)
			{
				currnet_way.at(current_deep) = core[point][i];
				i >= 2 ? current_decoded_sequence.at(current_deep) = true :
					     current_decoded_sequence.at(current_deep) = false;
				build_ways(i, current_deep + 1, max_deep);
			}
	}
	else
	{
		ways.at(point).push_back(currnet_way);
		way_to_bits(bits, currnet_way);
		costs_ways.at(point).push_back(hamming_distance(bits, inpt, 2 * current_deep));
		decoded_sequences.at(point).push_back(current_decoded_sequence);
	}
}


void Decoder::delete_ways()
{
	for (auto i = 0; i < I; ++i)
	{
		auto min_it = std::min_element(costs_ways.at(i).begin(), costs_ways.at(i).end());
		uint32_t min = *min_it;

		for (auto j = 0; j < ways.at(i).size(); ++j)
			if (costs_ways.at(i).at(j) > min)
			{
				costs_ways.at(i).erase(costs_ways.at(i).begin() + j);
				ways.at(i).erase(ways.at(i).begin() + j);
				decoded_sequences.at(i).erase(decoded_sequences.at(i).begin() + j--);
			}

	}
}


void Decoder::way_to_bits(bool_vect &bits, const std::deque<uint8_t> &way)
{
	bits.resize(2 * way.size());
	auto j = 0;
	for (auto i = 0; i < way.size(); ++i)
	{
		bits[j++] = 0b00000010 & way[i];
		bits[j++] = 0b00000001 & way[i];
	}
}


uint32_t Decoder::hamming_distance(const bool_vect x, const bool_vect y, uint32_t len)
{
	int distance = 0;
	for (auto i = 0; i < len; ++i)
		distance += x[i] != y[i];

	return distance;
}