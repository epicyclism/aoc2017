#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

template<uint64_t V> struct generator 
{
	uint64_t prev_;
	unsigned operator()()
	{
		prev_ = (prev_ * V) % 0x7fffffff;;
		return unsigned(prev_);
	}
};

template<uint64_t V, uint64_t D> struct generator2 
{
	uint64_t prev_;
	unsigned operator()()
	{
		do
		{
			prev_ = (prev_ * V) % 0x7fffffff;
		} while (prev_ % D);
		return unsigned(prev_);
	}
};

auto get_input()
{
	unsigned A = 0;
	unsigned B = 0;
	std::string ln;
	std::getline(std::cin, ln);
	if(auto[m, a] = ctre::match<"Generator A starts with (\\d+)">(ln); m)
		A = a.to_number<uint32_t>();
	std::getline(std::cin, ln);
	if(auto[m, b] = ctre::match<"Generator B starts with (\\d+)">(ln); m)
		B = b.to_number<uint32_t>();

	return std::make_pair(A, B);
}

int pt1(uint32_t A, uint32_t B)
{
	timer t("p1");
	generator<16807> gA(A);
	generator<48271> gB(B);
	int cnt = 0;
	for(int n = 0; n < 40000000; ++n)
		cnt += (gA() & 0xffff) == (gB() & 0xffff);
	return cnt;
}

int pt2(uint32_t A, uint32_t B)
{
	timer t("p2");
	generator2<16807, 4> gA(A);
	generator2<48271, 8> gB(B);
	int cnt = 0;
	for(int n = 0; n < 5000000; ++n)
		cnt += (gA() & 0xffff) == (gB() & 0xffff);
	return cnt;
}

int main()
{
	auto[A, B] = get_input();
	auto p1 = pt1(A, B);
	auto p2 = pt2(A, B);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
