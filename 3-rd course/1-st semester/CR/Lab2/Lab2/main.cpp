#include <iostream>

template<typename T>
void Swap(T& arg1, T& arg2) {
	T tmp = arg1;
	arg1 = arg2;
	arg2 = tmp;
}

const size_t block_size = 16; 
const size_t key_size = 16; 

static const unsigned char s0[16] = {
	 0x0C,
	 0x0F,
	 0x07,
	 0x0A,
	 0x0E,
	 0x0D,
	 0x0B,
	 0x00,
	 0x02,
	 0x06,
	 0x03,
	 0x01,
	 0x09,
	 0x04,
	 0x05,
	 0x08
};

static const unsigned char s1[16] = {
	 0x07,
	 0x02,
	 0x0E,
	 0x09,
	 0x03,
	 0x0B,
	 0x00,
	 0x04,
	 0x0C,
	 0x0D,
	 0x01,
	 0x0A,
	 0x06,
	 0x0F,
	 0x08,
	 0x05
};

static const unsigned char m1[8] = {
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
};

static const unsigned char m2[8] = {
	0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE
};

#define SHIFTLEFT(x, n) (x) << (n)
#define SHIFTRIGHT(x, n) static_cast<unsigned char>(x) >> (n)

#define HIGHSUBBYTE(x) SHIFTRIGHT((x), 4)
#define LOWSUBBYTE(x) (x) & 0x0F

void Lucifer(char block[block_size], char key[key_size], bool decrypt) {
	char* lower_half = block;
	char* upper_half = block + block_size / 2;

	int key_byte_idx = decrypt ? 8 : 0;

	const int round_count = 16;
	for (int round = 0; round < round_count; ++round) {
		if (decrypt)
			key_byte_idx = (key_byte_idx + 1) % round_count;

		int transform_countrol_byte_idx = key_byte_idx;

		const int step_count = 8;
		for (int step = 0; step < step_count; ++step) {
			char message_byte = upper_half[step];

			if (key[transform_countrol_byte_idx] & m1[step_count - step - 1])
				message_byte = SHIFTLEFT(s1[HIGHSUBBYTE(message_byte)], 4) | s0[LOWSUBBYTE(message_byte)];
			else
				message_byte = SHIFTLEFT(s0[HIGHSUBBYTE(message_byte)], 4) | s1[LOWSUBBYTE(message_byte)];

			message_byte ^= key[key_byte_idx];

			message_byte = SHIFTRIGHT(message_byte & m1[0], 3) |
				SHIFTRIGHT(message_byte & m1[1], 4) |
				SHIFTLEFT(message_byte & m1[2], 2) |
				SHIFTRIGHT(message_byte & m1[3], 1) |
				SHIFTLEFT(message_byte & m1[4], 2) |
				SHIFTLEFT(message_byte & m1[5], 4) |
				SHIFTRIGHT(message_byte & m1[6], 1) |
				SHIFTLEFT(message_byte & m1[7], 1);

			lower_half[(7 + step) % step_count] = ((message_byte ^ lower_half[(7 + step) % step_count]) & m1[0]) | (lower_half[(7 + step) % step_count] & m2[0]);
			lower_half[(6 + step) % step_count] = ((message_byte ^ lower_half[(6 + step) % step_count]) & m1[1]) | (lower_half[(6 + step) % step_count] & m2[1]);
			lower_half[(2 + step) % step_count] = ((message_byte ^ lower_half[(2 + step) % step_count]) & m1[2]) | (lower_half[(2 + step) % step_count] & m2[2]);
			lower_half[(1 + step) % step_count] = ((message_byte ^ lower_half[(1 + step) % step_count]) & m1[3]) | (lower_half[(1 + step) % step_count] & m2[3]);
			lower_half[(5 + step) % step_count] = ((message_byte ^ lower_half[(5 + step) % step_count]) & m1[4]) | (lower_half[(5 + step) % step_count] & m2[4]);
			lower_half[(0 + step) % step_count] = ((message_byte ^ lower_half[(0 + step) % step_count]) & m1[5]) | (lower_half[(0 + step) % step_count] & m2[5]);
			lower_half[(3 + step) % step_count] = ((message_byte ^ lower_half[(3 + step) % step_count]) & m1[6]) | (lower_half[(3 + step) % step_count] & m2[6]);
			lower_half[(4 + step) % step_count] = ((message_byte ^ lower_half[(4 + step) % step_count]) & m1[7]) | (lower_half[(4 + step) % step_count] & m2[7]);

			if (step < step_count - 1 || decrypt)
				key_byte_idx = (key_byte_idx + 1) % round_count;
		}

		Swap(lower_half, upper_half);
	}

	for (int i = 0; i < block_size / 2; ++i)
		Swap(block[i], block[i + block_size / 2]);
}

int main() {
	char crypt[block_size];
	std::cin >> crypt;
	char key[block_size] = { '1','2','3','6','5','6','5','7','4','8','4','8','4','8','5','7' };

	Lucifer(crypt, key, false);
	for (int i = 0; i < block_size; i++)
		printf("%c", crypt[i]);
	printf("\n");

	Lucifer(crypt, key, true);
	for (int i = 0; i < block_size; i++)
		printf("%c", crypt[i]);
	printf("\n");

	system("pause");
	return 0;
}
