#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>

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
long long read_n_bit_int(iter_t& it, int n)
{
	long long result = 0;
	for(int i=0;i<n;++i,++it)
		result = result*2 + *it;
	return result;
}

constexpr int literal = 4;
constexpr int op_sum = 0;
constexpr int op_prod = 1;
constexpr int op_min = 2;
constexpr int op_max = 3;
constexpr int op_greater = 5;
constexpr int op_smaller = 6;
constexpr int op_equal = 7;

struct handle_ret_val_type
{
	int read_bits;
	long long value;
};

template <typename iter_t>
handle_ret_val_type handle_packet(iter_t& it);

template <typename iter_t, typename op_t>
handle_ret_val_type compute_subpacket_val(iter_t& it, op_t op, long long start)
{
	handle_ret_val_type ret_val{0,start};
	
	int length_type = *it;
	++it;
	if(length_type==0)
	{
		int subpackages_length = read_n_bit_int(it,15);
		int read = 0;
		while(read<subpackages_length)
		{
			const auto result = handle_packet(it);
			ret_val.value = op(ret_val.value,result.value);
			read+=result.read_bits;
		}
		
		ret_val.read_bits+=read+16;
	}
	else
	{
		int subpackages_number = read_n_bit_int(it,11);
		ret_val.read_bits+=12;
		for(int i=0;i<subpackages_number;++i)
		{
			const auto result = handle_packet(it);
			ret_val.value = op(ret_val.value,result.value);
			ret_val.read_bits+=result.read_bits;
		}
		
	}
	
	return ret_val;
}

template <typename iter_t>
handle_ret_val_type handle_packet(iter_t& it)
{
	[[maybe_unused]] int version = read_n_bit_int(it,3);
	int type = read_n_bit_int(it,3);
		
	handle_ret_val_type ret_val{6,0};
	switch(type)
	{
		case literal:
		{
			while(*it==1)
			{
				++it;
				ret_val.value=ret_val.value*16+read_n_bit_int(it,4);
				ret_val.read_bits+=5;
			}
			++it;
			ret_val.value=ret_val.value*16+read_n_bit_int(it,4);
			ret_val.read_bits+=5;
			break;
		};
		case op_sum:
		{
			auto result = compute_subpacket_val(it,std::plus<>{},0);
			ret_val.read_bits+=result.read_bits;
			ret_val.value = result.value;
			break;
		}
		case op_prod:
		{
			auto result = compute_subpacket_val(it,std::multiplies<>{},1);
			ret_val.read_bits+=result.read_bits;
			ret_val.value = result.value;
			break;
		}
		case op_min:
		{
			auto result = compute_subpacket_val(it,[](auto lhs, auto rhs){ return std::min(lhs,rhs); },std::numeric_limits<long long>::max());
			ret_val.read_bits+=result.read_bits;
			ret_val.value = result.value;
			break;
		}
		case op_max:
		{
			auto result = compute_subpacket_val(it,[](auto lhs, auto rhs){ return std::max(lhs,rhs); },std::numeric_limits<long long>::min());
			ret_val.read_bits+=result.read_bits;
			ret_val.value = result.value;
			break;
		}
		case op_smaller:
		{
			auto result = compute_subpacket_val(it,[i=0](auto lhs, auto rhs) mutable
			{
				if(!i++) return rhs;
				return lhs<rhs?1ll:0ll;
			},0);
			ret_val.read_bits+=result.read_bits;
			ret_val.value = result.value;
			break;
		}
		case op_greater:
		{
			auto result = compute_subpacket_val(it,[i=0](auto lhs, auto rhs) mutable
			{
				if(!i++) return rhs;
				return lhs>rhs?1ll:0ll;
			},0);
			ret_val.read_bits+=result.read_bits;
			ret_val.value = result.value;
			break;
		}
		case op_equal:
		{
			auto result = compute_subpacket_val(it,[i=0](auto lhs, auto rhs) mutable
			{
				if(!i++) return rhs;
				return lhs==rhs?1ll:0ll;
			},0);
			ret_val.read_bits+=result.read_bits;
			ret_val.value = result.value;
			break;
		}
	}
	return ret_val;
}

int main(int argc, char* argv[])
{
	auto it=bitwise_iter<std::istream_iterator<char>>{std::istream_iterator<char>{std::cin}};
	
	while(it!=bitwise_iter<std::istream_iterator<char>>::sentinel{})
	{
		std::cout<<handle_packet(it).value;
		break;
	}
}
