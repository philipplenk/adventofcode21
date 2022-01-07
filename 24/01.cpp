#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct operand
{
	std::string s;
	int i;
};

enum class instruction_type
{
	inp,
	mul,
	div,
	add,
	mod,
	eql
};

std::istream& operator>>(std::istream& in, operand& op)
{
	op.s.clear();
	in>>std::ws;
	if(std::isdigit(in.peek()) || in.peek()=='-')
		in>>op.i;
	else
		in>>op.s;
	return in;
}

std::ostream& operator<<(std::ostream& out, const operand& op)
{
	if(op.s.empty()) return out<<op.i;
	return out<<op.s;
}

struct instruction
{
	instruction_type type;
	operand a,b;
};

std::istream& operator>>(std::istream& in, instruction& i)
{
	std::string type;
	in>>type;
	if(type=="inp")
	{
		i.type=instruction_type::inp;
		return in>>i.a;
	}
	if(type=="mul")
	{
		i.type=instruction_type::mul;
		return in>>i.a>>i.b;
	}
	if(type=="div")
	{
		i.type=instruction_type::div;
		return in>>i.a>>i.b;
	}
	if(type=="add")
	{
		i.type=instruction_type::add;
		return in>>i.a>>i.b;
	}
	if(type=="mod")
	{
		i.type=instruction_type::mod;
		return in>>i.a>>i.b;
	}
	if(type=="eql")
	{
		i.type=instruction_type::eql;
		return in>>i.a>>i.b;
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const instruction& ins)
{
	switch(ins.type)
	{
		case instruction_type::inp: return out<<"inp "<<ins.a; break;
		case instruction_type::mul: return out<<"mul "<<ins.a<<' '<<ins.b; break;
		case instruction_type::div: return out<<"div "<<ins.a<<' '<<ins.b; break;
		case instruction_type::add: return out<<"add "<<ins.a<<' '<<ins.b; break;
		case instruction_type::mod: return out<<"mod "<<ins.a<<' '<<ins.b; break;
		case instruction_type::eql: return out<<"eql "<<ins.a<<' '<<ins.b; break;
	}
	return out;
}

int compute_step(int input, int acc, int check, int add)
{
	if(acc%26+check==input)
		return check<0?acc/26:acc;
		
	return 26*(acc/(check<0?26:1)) + input + add;
}

struct step_info
{
	int add, check;
};

std::vector<step_info> extract_constraints(std::istream& in)
{
	std::vector<step_info> ret_val;
	
	for(auto it = std::istream_iterator<instruction>{in};it!=std::istream_iterator<instruction>{};std::advance(it,3))
	{
		step_info next_step;
		std::advance(it,5);
		next_step.check = it->b.i;
		std::advance(it,10);
		next_step.add = it->b.i;
		
		ret_val.push_back(next_step);
	}
	
	return ret_val;
}

int main(int argc, char* argv[])
{
	const auto steps = extract_constraints(std::cin);
	
	const auto backtracking_search  = [&](std::string& str, std::size_t idx, int acc, auto rec) -> bool
	{
		if(idx==str.size())
			return acc==0;
		else
		{
			while(str[idx]>='1')
			{
				const auto new_acc = compute_step(str[idx]-'0',acc,steps[idx].check,steps[idx].add);
				
				if((steps[idx].check>0 || new_acc==acc/26) && rec(str,idx+1,new_acc,rec)) return true;
				--str[idx];
			}
			str[idx]='9';
			return false;
		}
	};
	std::string str(14,'9');
	backtracking_search(str,0,0,backtracking_search);
	std::cout<<str;
}
