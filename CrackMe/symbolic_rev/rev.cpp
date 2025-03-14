#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <string.h>

namespace AprilCrackme
{
	char buf[9];

	void Process()
	{
		printf("Enter key...\n");

		std::cin >> buf;
		int check_first = 3 * (buf[0] + buf[4]) + 2 * buf[2] - buf[1] - buf[3];	
		int check_second = 3 * (buf[3] + 2 * buf[0]) - 2 * (buf[1] + 2 * buf[4]) - buf[2];	
		int check_third = buf[4] + buf[1] - buf[2] + buf[0] + 2 * (buf[1] - buf[2]) + 7 * buf[3];
		int check_fourth = buf[1] + 2 * buf[0] + 3 * (buf[3] + buf[2]) - 7 * buf[4];
		int check_fifth = buf[4] + buf[0] + buf[2] + buf[3] + buf[1];

		printf("%x %x %x %x %x\n", check_first, check_second, check_third, check_fourth, check_fifth);
		int check_sixth = buf[6] ^ 0x6F;
		printf("%x\n", check_sixth);
		bool check_seventh = buf[6] == buf[7];
		printf("%x %s %x\n", buf[6], (check_seventh ? "==" : "!="), buf[7]);
		int check_eighth = buf[8] - buf[5];	
		int check_ninth = buf[5] + buf[8];	

		printf("%i %x\n", check_eighth, check_ninth);

		if (check_first == 0x15b &&
			check_second == 0x68 &&
			check_third == 0x1c2 &&
			check_fourth == 0x57 &&
			check_fifth == 0x10e &&
			check_sixth == 0x5f &&
			check_seventh &&
			check_eighth == 0x3 &&
			check_ninth == 0xeb)
			printf("Cracked!\n");
		else
			printf("Wrong key!\n");

		int retchar = getchar();

		/*
		 *	Crackme 'what_is_my_password' by:
		 *		br0ken - https://crackmes.one/crackme/5ab77f5333c5d40ad448c103
		*/
	}
}

int main()
{
	AprilCrackme::Process();
}