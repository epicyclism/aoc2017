#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <numeric>
#include <charconv>
#include <fmt/format.h>

#include "timer.h"

auto knot_hash(std::vector<uint8_t> key)
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
	return dh;
}

auto knot_hash(std::string const& s)
{
	std::vector<uint8_t> v(s.size());
	std::ranges::copy(s, v.begin());
	return knot_hash(v);
}

std::string make_key(char const* rt, int n)
{
	std::string s(rt);
	s += '-';
	char tmp [4];
	auto r = std::to_chars(tmp, tmp + 4, n);
	*r.ptr = 0;
	s += tmp;
	return s;
}

int count_bits(auto const& v)
{
	return std::reduce(v.begin(), v.end(), 0, [](auto l, auto r){ return l + std::popcount(r);});
}

bool is_set(auto const& v, int n)
{
//	return v[n / 8] & (1 << (n % 8 ));
	return v[n / 8] & (0x80 >> (n % 8 ));
}

void set_region(std::vector<uint8_t> const& v, std::bitset<128 * 128>& visited, int node)
{
	visited[node] = true;
	if(node > 127 && !visited[node - 128] && is_set(v, node - 128))
		set_region(v, visited, node - 128);
	if(node % 128 > 0 && !visited[node - 1] && is_set(v, node - 1))
		set_region(v, visited, node - 1);
	if(node % 128 < 127 && !visited[node + 1] && is_set(v, node + 1))
		set_region(v, visited, node + 1);
	if(node < ( 128 * 127) && !visited[node + 128] && is_set(v, node + 128))
		set_region(v, visited, node + 128);
}

int count_regions(std::vector<uint8_t> const& v)
{
	std::bitset<128 * 128> visited;
	int n = 0;
	int regions = 0;
	while( n < visited.size())
	{
		if(is_set(v, n) && !visited[n])
		{
			++regions;
			set_region(v, visited, n);
		}
		++n;
	}
	return regions;
}

auto pt12(auto const& in)
{
	timer t("p12");
	std::vector<uint8_t> v;
	v.reserve(16 * 16);
	for(int n = 0; n < 128; ++n)
	{
		auto kh = knot_hash(make_key(in, n));
		v.append_range(kh);
	}
	auto p1 = count_bits(v);
	auto p2 = count_regions(v);
	return std::make_pair(p1, p2);
}

int main(int ac, char ** av)
{
	auto in = "amgozmfv";
	if(ac > 1)
		in = av[1];
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
