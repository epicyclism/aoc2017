#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

enum class op_t { snd, sndi, set, seti, add, addi, mul, muli, mod, modi, rcv, rcvi, jgz, jgzi, jgzii, jgzir};

struct inst_t
{
	op_t op_;
	int64_t a_;
	int64_t b_;
};

using regs = std::array<int64_t, 26>;
using hw   = int64_t;

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
		if(auto[m, a] = ctre::match<"snd ([a-z])">(ln); m)
			vi.emplace_back(op_t::snd, to_reg(a), 0);
		else
		if(auto[m, a] = ctre::match<"snd (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::sndi, a.to_number<int>(), 0);
		else
		if(auto[m, a, b] = ctre::match<"set ([a-z]) ([a-z])">(ln); m)
			vi.emplace_back(op_t::set, to_reg(a), to_reg(b));
		else
		if(auto[m, a, b] = ctre::match<"set ([a-z]) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::seti, to_reg(a), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"add ([a-z]) ([a-z])">(ln); m)
			vi.emplace_back(op_t::add, to_reg(a), to_reg(b));
		else
		if(auto[m, a, b] = ctre::match<"add ([a-z]) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::addi, to_reg(a), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"mul ([a-z]) ([a-z])">(ln); m)
			vi.emplace_back(op_t::mul, to_reg(a), to_reg(b));
		else
		if(auto[m, a, b] = ctre::match<"mul ([a-z]) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::muli, to_reg(a), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"mod ([a-z]) ([a-z])">(ln); m)
			vi.emplace_back(op_t::mod, to_reg(a), to_reg(b));
		else
		if(auto[m, a, b] = ctre::match<"mod ([a-z]) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::modi, to_reg(a), b.to_number<int>());
		else
		if(auto[m, a] = ctre::match<"rcv ([a-z])">(ln); m)
			vi.emplace_back(op_t::rcv, to_reg(a), 0);
		else
		if(auto[m, a] = ctre::match<"rcv (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::rcvi, a.to_number<int>(), 0);
		else
		if(auto[m, a, b] = ctre::match<"jgz ([a-z]) ([a-z])">(ln); m)
			vi.emplace_back(op_t::jgz, to_reg(a), to_reg(b));
		else
		if(auto[m, a, b] = ctre::match<"jgz ([a-z]) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::jgzi, to_reg(a), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"jgz (-?\\d+) (-?\\d+)">(ln); m)
			vi.emplace_back(op_t::jgzii, a.to_number<int>(), b.to_number<int>());
		else
		if(auto[m, a, b] = ctre::match<"jgz (-?\\d+) ([a-z])">(ln); m)
			vi.emplace_back(op_t::jgzir, a.to_number<int>(), to_reg(b));
		else
			fmt::println("parse fail {}", ln);
	}
	return vi;
}

template<typename O> struct processor : public O
{
	std::vector<inst_t> const& i_;
	regs r_;
	int ip_ = 0;
	void run()
	{
		while(ip_ >= 0 && ip_ < i_.size())
		{
			auto& ii = i_[ip_];
			switch(ii.op_)
			{
				case op_t::snd:
					if(!O::send(r_[ii.a_]))
						return;
					break;
				case op_t::sndi:
					if(!O::send(ii.a_))
						return;
					break;
				case op_t::set:
					r_[ii.a_] = r_[ii.b_];
					break;
				case op_t::seti:
					r_[ii.a_] = ii.b_;
					break;
				case op_t::add:
					r_[ii.a_] += r_[ii.b_];
					break;
				case op_t::addi:
					r_[ii.a_] += ii.b_;
					break;
				case op_t::mul:
					r_[ii.a_] *= r_[ii.b_];
					break;
				case op_t::muli:
					r_[ii.a_] *= ii.b_;
					break;
				case op_t::mod:
					r_[ii.a_] %= r_[ii.b_];
					break;
				case op_t::modi:
					r_[ii.a_] %= ii.b_;
					break;
				case op_t::rcv:
					if(!O::receive(r_[ii.a_]))
						return;
					break;
				case op_t::rcvi:
//					if(!O::receive(ii.a_))
//						return;
					break;
				case op_t::jgz:
					if(r_[ii.a_] > 0)
						ip_ += r_[ii.b_] - 1;
					break;
				case op_t::jgzi:
					if(r_[ii.a_] > 0)
						ip_ += ii.b_ - 1;
					break;
				case op_t::jgzii:
					if(ii.a_ > 0)
						ip_ += ii.b_ - 1;
					break;
				case op_t::jgzir:
					if(ii.a_ > 0)
						ip_ += r_[ii.b_] - 1;
					break;
			}
			++ip_;
		}
	}
	template<typename... OArgs> processor(std::vector<inst_t> const& i, OArgs... aa) : i_(i), O(aa...)
	{
		r_.fill(0);
	}
};

struct part1
{
	int64_t v_;
	bool send(int64_t v)
	{
		v_ = v;
		return true;
	}
	bool receive(int64_t v)
	{
		return v == 0;
	}
};

int64_t pt1(auto const& in)
{
	timer t("p1");
	processor<part1> p(in);
	p.run();
	return p.v_;
}

struct part2
{
	std::queue<int64_t>& out_;
	std::queue<int64_t>& in_;
	int cnt_ = 0;
	bool send(int64_t v)
	{
		out_.push(v);
		++cnt_;
		return true;
	}
	bool receive(int64_t& v)
	{
		if(in_.empty())
			return false;
		v = in_.front();
		in_.pop();
		return true;
	}
};

int64_t pt2(auto const& in)
{
	timer t("p2");
	std::queue<int64_t> A_B;
	std::queue<int64_t> B_A;
	processor<part2> pA(in, std::ref(A_B), std::ref(B_A));
	processor<part2> pB(in, std::ref(B_A), std::ref(A_B));
	pA.r_['p' - 'a'] = 0;
	pB.r_['p' - 'a'] = 1;
	do
	{
		pA.run();
		pB.run();
	} while (!(A_B.empty() && B_A.empty()));
	
	return pB.cnt_;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
