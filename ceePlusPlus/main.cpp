#include "noisy_type.hpp"

#include <iostream>

Noisy func(bool b)
{
	Noisy n;
	Noisy m;
	if (b)
		return Noisy();
	else
		return n;
	//	return std::move(n);
}

Noisy func( Noisy param )
{

	return param;
}


int main(int argc, char* argv[])
{
	std::cout << argc << '\n';

	return 0;
}
