#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<int> vv;
	std::string ln;
	std::getline(std::cin, ln);
	for(auto v: ctre::search_all<"(\\d+)">(ln))
		vv.emplace_back(v.to_number<int>());
	return std::make_pair(vv, ln);
}

int pt1(auto const& in)
{
	timer t("pt1");
	int skip = 0;
	int cp = 0;
	std::array<uint8_t, 256> a;
	std::iota(a.begin(), a.end(), 0);
	for(auto op: in.first)
	{
		std::ranges::reverse(a.begin(), a.begin() + op);
		std::ranges::rotate(a, a.begin() + ((op + skip) % 256));
		cp += 256 - ((op + skip) % 256);
		cp %= 256;
		++skip;
	}
	return a[cp] * a[ cp + 1];
}

char to_hex(uint8_t u)
{
	if( u < 10)
		return u + '0';
	return u + 'a' - 10;
}

std::string knot_hash(std::vector<uint8_t> key)
{
	constexpr std::array ap = {17, 31, 73, 47, 23};
	key.append_range(ap);
	int skip = 0;
	int cp = 0;
	std::array<uint8_t, 256> a;
	std::iota(a.begin(), a.end(), 0);
	for(int n = 0; n < 64; ++n)
	{
		for(auto op: key)
		{
			std::ranges::reverse(a.begin(), a.begin() + op);
			std::ranges::rotate(a, a.begin() + ((op + skip) % 256));
			cp += 256 - ((op + skip) % 256);
			cp %= 256;
			++skip;
		}
	}
	std::ranges::rotate(a, a.begin() + cp);
	std::array<uint8_t, 16> dh;
	for(int out = 0; out < 16; ++out)
	{
		uint8_t x = 0;
		for(int in = 0; in < 16; ++in)
		{
			x ^= a[out * 16 + in];
		}
		dh[out] = x;
	}
	std::string s;
	for(auto b : dh)
	{
		s.push_back(to_hex((b >> 4)& 0x0f));
		s.push_back(to_hex(b & 0x0f));
	}
	return s;
}

std::string knot_hash(std::string const& s)
{
	std::vector<uint8_t> v(s.size());
	std::ranges::copy(s, v.begin());
	return knot_hash(v);
}

auto pt2(auto const& in)
{
	timer t("p2");
	return knot_hash(in.second);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
