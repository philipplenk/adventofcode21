#include <iostream>
#include <iterator>

template <typename underlying_t>
struct bitwise_iter
{
	bitwise_iter(underlying_t it):
		iter_{it},
		bit_idx_{0}
	{
		char v = *iter_++;
		value_ = v<='9'?v-'0':(v-'A'+10);
	}
	
	bitwise_iter operator++()
	{
		if(++bit_idx_>=4 && iter_!=underlying_t{})
		{
			char v = *iter_++;
			value_ = v<='9'?v-'0':(v-'A'+10);
			bit_idx_ = 0;
		}
		return *this;
	}
	
	bool operator*() const { return (value_>>(3-bit_idx_))&1; }
	
	
	struct sentinel{};
	friend bool operator==(const bitwise_iter& lhs, const sentinel& rhs)
	{
		return lhs.iter_==underlying_t{} && lhs.bit_idx_>=4;
	}
	
	friend bool operator!=(const bitwise_iter& lhs, const sentinel& rhs)
	{
		return !(lhs==rhs);
	}
	
	underlying_t iter_;
	unsigned char value_;
	int bit_idx_;
	
};

template <typename iter_t>
int read_n_bit_int(iter_t& it, int n)
{
	int result = 0;
	for(int i=0;i<n;++i,++it)
		result = result*2 + *it;
	return result;
}

constexpr int literal = 4;

template <typename iter_t,typename fun_t>
int handle_packet(iter_t& it, fun_t fun)
{
	int version = read_n_bit_int(it,3);
	int type = read_n_bit_int(it,3);
	
	fun(version,type);
	
	int total = 6;
	switch(type)
	{
		case literal:
		{
			unsigned long long n = 0;
			while(*it==1)
			{
				++it;
				n=n*16+read_n_bit_int(it,4);
				total+=5;
			}
			++it;
			read_n_bit_int(it,4);
			total+=5;
			break;
		};
		default:
		{
			int length_type = *it;
			++it;
			if(length_type==0)
			{
				int subpackages_length = read_n_bit_int(it,15);
				int read = 0;
				while(read<subpackages_length)
					read+=handle_packet(it,fun);
				
				total+=read+16;
			}
			else
			{
				int subpackages_number = read_n_bit_int(it,11);
				total+=12;
				for(int i=0;i<subpackages_number;++i)
					total+=handle_packet(it,fun);
				
			}
			break;
		}
	}
	return total;
}

int main(int argc, char* argv[])
{
	auto it=bitwise_iter<std::istream_iterator<char>>{std::istream_iterator<char>{std::cin}};
	
	int sum = 0;
	while(it!=bitwise_iter<std::istream_iterator<char>>::sentinel{})
	{
		handle_packet(it,[&](auto v, auto t){ sum+=v; });
	}
	std::cout<<sum;
}
