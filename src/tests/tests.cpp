#include <gtest/gtest.h>
#include "vitdec.h"


#include <iostream>
#include <ctime>


//MATLAB:
//
//trellis = poly2trellis(3, { '1 + x^2','1 + x + x^2' });
//data = randi([0 1], 8, 1);
//codedData = convenc(data, trellis);
//tbdepth = 8;
//decodedData = vitdec(codedData, trellis, tbdepth, 'trunc', 'hard');
//biterr(data, decodedData)


TEST(encoder_test, stat_test_0)
{
	bool_vect input0 = {};

	bool_vect input1 = { 0,0,0,0};
	bool_vect input2 = { 1,1,1,1,1};

	bool_vect input3 = { 0,0 };
	bool_vect input4 = { 1,1 };


	bool_vect coderef0 = {};

	bool_vect coderef1 = { 0,0,0,0,0,0,0,0 };
	bool_vect coderef2 = { 1,1,1,0,0,1,0,1,0,1 };

	bool_vect coderef3 = { 0,0,0,0 };
	bool_vect coderef4 = { 1,1,1,0 };

	Encoder enc;

	EXPECT_TRUE(enc.encode(input0) == coderef0);
	EXPECT_TRUE(enc.encode(input1) == coderef1);
	EXPECT_TRUE(enc.encode(input2) == coderef2);
	EXPECT_TRUE(enc.encode(input3) == coderef3);
	EXPECT_TRUE(enc.encode(input4) == coderef4);

}

TEST(encoder_test, stat_test_1)
{
	bool_vect input1 = { 1,0,0,0,0,0,0,0 };
	bool_vect input2 = { 1,1,0,1,1,0,0,1 };

	bool_vect input3 = { 0,0,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,1,1,1 };

	bool_vect coderef1 = { 1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0 };
	bool_vect coderef2 = { 1,1,1,0,1,0,0,0,1,0,1,0,1,1,1,1 };

	bool_vect coderef3 = { 0,0,0,0,1,1,1,0,0,1,1,0,0,0,1,0,1,0,1,1,
	0,0,1,1,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,1 };

	Encoder enc;
	

	EXPECT_TRUE(enc.encode(input1) == coderef1);
	EXPECT_TRUE(enc.encode(input2) == coderef2);
	EXPECT_TRUE(enc.encode(input3) == coderef3);

}


TEST(decoder_test, stat_test_1)
{
	bool_vect input1 = { 1,0,0,0,0,0,0,0 };
	bool_vect input2 = { 1,1,0,1,1,0,0,1 };

	bool_vect input3 = { 0,0,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,1,1,1 };

	Encoder enc;

	auto code_data1 = enc.encode(input1);
	auto code_data2 = enc.encode(input2);
	auto code_data3 = enc.encode(input3);
  

	Decoder dcd;

	EXPECT_TRUE(dcd.decode(code_data1) == input1);
	EXPECT_TRUE(dcd.decode(code_data2) == input2);
	EXPECT_TRUE(dcd.decode(code_data3) == input3);

}

TEST(decoder_test, stat_test_2)
{
	bool_vect input0 = {};

	bool_vect input1 = { 0,0,0,0 };
	bool_vect input2 = { 1,1,1,1,1,1};
	bool_vect input3 = { 1, 1, 1};


	Encoder enc;

	auto code_data0 = enc.encode(input0);
	auto code_data1 = enc.encode(input1);
	auto code_data2 = enc.encode(input2);
	auto code_data3 = enc.encode(input3);
	

	Decoder dcd;

	EXPECT_TRUE(dcd.decode(code_data0) == input0);
	EXPECT_TRUE(dcd.decode(code_data1) == input1);
	EXPECT_TRUE(dcd.decode(code_data2) == input2);

	EXPECT_TRUE(dcd.decode(code_data3) == input3);


}


TEST(decoder_test, rand_test_1)
{
	srand(time(NULL));

	auto size = rand() % 30 + 3;
	bool_vect input(size);
	for (auto &v : input)
		v = rand() % 2;

	Encoder enc;

	auto code_data = enc.encode(input);

	Decoder dcd;

	EXPECT_TRUE(dcd.decode(code_data) == input);

}


TEST(decoder_test, rand_error_test_1)
{
	srand(time(NULL));
	auto size = rand() % 30 + 3;
	bool_vect input(size);
	for (auto &v : input)
		v = rand() % 2;

	Encoder enc;

	auto code_data = enc.encode(input);

	auto ind_error_bit = rand() % (code_data.size() - 2);
	code_data[ind_error_bit] = ~code_data[ind_error_bit];

	Decoder dcd;

	EXPECT_TRUE(dcd.decode(code_data) == input);

}

TEST(decoder_test, rand_error_test_2)
{
	srand(time(NULL));
	auto size = rand() % 30 + 3;
	bool_vect input(size);
	for (auto &v : input)
	{
		v = rand() % 2;
		std::cout << v << "; ";
	}
	std::cout << std::endl;

	Encoder enc;

	auto code_data = enc.encode(input);

	for (auto i = 0; i < code_data.size() - 3; i += 6)
	{
		auto ind_error_bit = rand() % 2 + i;
		code_data[ind_error_bit] = ~code_data[ind_error_bit];
	}

	for (auto &v : code_data)
	{
		std::cout << v << "; ";
	}
	std::cout << std::endl;

	Decoder dcd;

	EXPECT_TRUE(dcd.decode(code_data) == input);

}


int main(int argc, char* argv[])
{

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}