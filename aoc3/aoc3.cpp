#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

int sq_below(int v)
{
	int N = 1;
	while(1)
	{
		auto odd = 2 * N + 1;
		if(odd * odd > v)
			break;
		++N;
	}
	return N - 1;
}

auto xy(int n)
{
	auto N = sq_below(n);
	auto SL = 2 * N + 1;
	auto D = n - (2 * N + 1) * (2 * N + 1);

	auto x = N;
	auto y = -N;
	if(D == 0)
		return std::make_pair(x, y);
	++x;
	--D;
	if(D < SL)
		return std::make_pair(x, y + D);
	y += SL;
	D -= SL;
	if(D <= SL)
		return std::make_pair(x - D, y);
	x -= (SL + 1);
	D -= (SL + 1);
	if(D <= SL)
		return std::make_pair(x, y - D);
	y -= (SL + 1);
	D -= (SL + 1);
	return std::make_pair(x + D, y);
}

int pt1a(auto in)
{
	timer t("p1");
	auto N = sq_below(in);
	auto SL = 2 * N + 1;
	auto D = in - (2 * N + 1) * (2 * N + 1);

	auto x = N;
	auto y = -N;
	if(D == 0)
		return std::abs(x) + std::abs(y);
	++x;
	--D;
	if(D < SL)
		return std::abs(x) + std::abs(y + D);
	y += SL;
	D -= SL;
	if(D <= SL)
		return std::abs(x - D) + std::abs(y);
	x -= (SL + 1);
	D -= (SL + 1);
	if(D <= SL)
		return std::abs(x) + std::abs(y - D);
	y -= (SL + 1);
	D -= (SL + 1);
	return std::abs(x + D) + std::abs(y);
}

int pt1(auto in)
{
	timer t("p1");
	auto[x, y] = xy(in);

	return std::abs(x) + std::abs(y);
}

struct map2d
{
	const int SIDE = 1024;
	const int OFFS = SIDE / 2;
	std::vector<int> v_;
	map2d() : v_(SIDE * SIDE, 0)
	{}
	void put(int x, int y, int n)
	{
		v_[(y + OFFS) * SIDE + x + OFFS] = n;
	}
	int get(int x, int y)
	{
		return v_[(y + OFFS) * SIDE + x + OFFS];
	}
};

int pt2(auto in)
{
	timer t("p2");
	map2d mp;
	mp.put(0, 0, 1);
	int n = 2;
	while(1)
	{
		auto[x, y] = xy(n);
		int v = 0;
		v += mp.get(x + 1, y);
		v += mp.get(x + 1, y + 1);
		v += mp.get(x, y + 1);
		v += mp.get(x - 1, y + 1);
		v += mp.get(x - 1, y);
		v += mp.get(x - 1, y - 1);
		v += mp.get(x, y - 1);
		v += mp.get(x + 1, y - 1);
		mp.put(x, y, v);
		if( v > in)
			return v;
		++n;
	}

	return 0;
}

int main(int ac, char**av)
{
	auto in = 368078;
	if(ac > 1)
		in = sv_to_t<int>(av[1]);

	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
