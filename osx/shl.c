#include <stdio.h>

__attribute__((section("__TEXT_EXEC, __test")))
static const unsigned int shl[] = {
	0xd2800090,
	0xd2800020,
	0x10000081,
	0xd2800082,
	0xd4001001,
	0xd65f03c0,
	0x0a636261,
};

int
main(void)
{
	void(*fn)(void) = (void*)shl;
	fn();
	return 0;
}
