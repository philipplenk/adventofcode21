#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	std::string skip;
	char skip_c;
	
	int skip_i, min_y;
	std::cin>>skip>>skip>>std::ws>>skip_c>>skip_c>>skip_i>>skip_c>>skip_c>>skip_i>>skip_c>>skip_c>>skip_c>>min_y;
	
	std::cout<<(min_y*min_y+min_y)/2;
}
