#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

enum class op_t { set, seti, sub, subi, mul, muli, jnz, jnzi, jnzii, jnzir};

struct inst_t
{
	op_t op_;
	int64_t a_;
	int64_t b_;
};

using regs = std::array<int64_t, 8>;

int to_reg(std::string_view v)
{
	return v[0] - 'a';
}

auto get_input()
{
	std::vector<inst_t> vi;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, a, b] = ctre::match<"set ([a-z]) ([a-z])">(ln); m)
			vi.emplace_back(op_t::set, to_reg(a), to_reg(b));
		else
		if(auto[m, a, b] = ctre::match<"set ([a-z]) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::seti, to_reg(a), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"sub ([a-z]) ([a-z])">(ln); m)
			vi.emplace_back(op_t::sub, to_reg(a), to_reg(b));
		else
		if(auto[m, a, b] = ctre::match<"sub ([a-z]) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::subi, to_reg(a), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"mul ([a-z]) ([a-z])">(ln); m)
			vi.emplace_back(op_t::mul, to_reg(a), to_reg(b));
		else
		if(auto[m, a, b] = ctre::match<"mul ([a-z]) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::muli, to_reg(a), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"jnz ([a-z]) ([a-z])">(ln); m)
			vi.emplace_back(op_t::jnz, to_reg(a), to_reg(b));
		else
		if(auto[m, a, b] = ctre::match<"jnz ([a-z]) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::jnzi, to_reg(a), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"jnz (-?\\d+) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::jnzii, a.to_number<int>(), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"jnz (-?\\d+) ([a-z])">(ln); m)
			vi.emplace_back(op_t::jnzir, a.to_number<int>(), to_reg(b));
		else
			fmt::println("parse fail {}", ln);
	}
	return vi;
}

struct processor
{
	std::vector<inst_t> const& i_;
	regs r_;
	int ip_ = 0;
	int cnt_ = 0;
	void run()
	{
		while(ip_ >= 0 && ip_ < i_.size())
		{
			auto& ii = i_[ip_];
			switch(ii.op_)
			{
				case op_t::set:
					r_[ii.a_] = r_[ii.b_];
					break;
				case op_t::seti:
					r_[ii.a_] = ii.b_;
					break;
				case op_t::sub:
					r_[ii.a_] -= r_[ii.b_];
					break;
				case op_t::subi:
					r_[ii.a_] -= ii.b_;
					break;
				case op_t::mul:
					r_[ii.a_] *= r_[ii.b_];
					++cnt_;
					break;
				case op_t::muli:
					r_[ii.a_] *= ii.b_;
					++cnt_;
					break;
				case op_t::jnz:
					if(r_[ii.a_] != 0)
						ip_ += r_[ii.b_] - 1;
					break;
				case op_t::jnzi:
					if(r_[ii.a_] != 0)
						ip_ += ii.b_ - 1;
					break;
				case op_t::jnzii:
					if(ii.a_ != 0)
						ip_ += ii.b_ - 1;
					break;
				case op_t::jnzir:
					if(ii.a_ != 0)
						ip_ += r_[ii.b_] - 1;
					break;
			}
			++ip_;
		}
	}
};

int64_t pt1(auto const& in)
{
	timer t("p1");
	processor proc(in);
	proc.run();
	return proc.cnt_;
}

bool is_prime(int64_t b)
{
	if( b % 2 == 0 || b % 3 == 0 || b % 5 == 0)
		return false;
	int64_t f = 7;
	while( f * f < b)
	{
		if ( b % f == 0)
			return false;
		f += 2;
	}
	return true;
}

// every 17 from b tp c, count not prime...
//
int64_t hard_code(std::vector<inst_t> const& i)
{
	int64_t b = i[0].b_;
	int64_t c = b;
	b *= 100;
	b += 100000;
	c = b + 17000;
	int64_t h = 0;
	for(; b <= c; b += 17)
		h += !is_prime(b);

	return h;
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	return hard_code(in);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
