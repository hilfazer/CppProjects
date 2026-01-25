#include <iostream>
#include <algorithm>
#include <vector>
#include <string>


int main()
{
	std::vector<std::string> zxc = {"foo", "baz"};
	std::vector<std::string> asd = {"foo", "bar", "baz"};

	auto mismatch = std::mismatch(zxc.begin(), zxc.end(), asd.begin(), asd.end());

	if( mismatch.first != zxc.end() )
		std::cout << "first: " << *mismatch.first << std::endl;

	if( mismatch.second != asd.end() )
		std::cout << "second: " << *mismatch.second << std::endl;
	return 0;
}
