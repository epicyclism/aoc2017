#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>

#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "ctre_inc.h"
#include "timer.h"

// 01 -> 20
// 23    31
//
// 01 -> 10
// 23.   32
//
// 01 -> 23
// 23.   12
//
// 012    630
// 345 -> 741
// 678    852
//
// 012.   210
// 345 -> 543
// 678    876
//
// 012.   678
// 345 -> 345
// 678    012
//

constexpr std::array<size_t, 4> rot_2  {2, 0, 3, 1};
constexpr std::array<size_t, 4> flip_2 {1, 0, 3, 2};
constexpr std::array<size_t, 4> flipx_2 {2, 3, 0, 1};
constexpr std::array<size_t, 9> rot_3  {6, 3, 0, 7, 4, 1, 8, 5, 2};
constexpr std::array<size_t, 9> flip_3 {2, 1, 0, 5, 4, 3, 8, 7, 6};
constexpr std::array<size_t, 9> flipx_3 {6, 7, 8, 3, 4, 5, 0, 1, 2};

std::array<char, 4> rotate(std::array<char, 4> const& in)
{
	std::array<char, 4> r;
	for(auto i = 0; i < rot_2.size(); ++i)
		r[rot_2[i]] = in[i]; 
	return r;
}

std::array<char, 9> rotate(std::array<char, 9> const& in)
{
	std::array<char, 9> r;
	for(auto i = 0; i < rot_3.size(); ++i)
		r[rot_3[i]] = in[i]; 
	return r;
}

std::array<char, 4> flip(std::array<char, 4> const& in)
{
	std::array<char, 4> r;
	for(auto i = 0; i < flip_2.size(); ++i)
		r[flip_2[i]] = in[i]; 
	return r;
}

std::array<char, 4> flipx(std::array<char, 4> const& in)
{
	std::array<char, 4> r;
	for(auto i = 0; i < flipx_2.size(); ++i)
		r[flipx_2[i]] = in[i]; 
	return r;
}

std::array<char, 9> flip(std::array<char, 9> const& in)
{
	std::array<char, 9> r;
	for(auto i = 0; i < flip_3.size(); ++i)
		r[flip_3[i]] = in[i]; 
	return r;
}

std::array<char, 9> flipx(std::array<char, 9> const& in)
{
	std::array<char, 9> r;
	for(auto i = 0; i < flipx_3.size(); ++i)
		r[flipx_3[i]] = in[i]; 
	return r;
}

void test()
{
	fmt::println("2x2");
	fmt::println("rotate");
	std::array<char, 4> two = { '#', ' ', ' ', ' '};
	fmt::println("[{}]", fmt::join(two, ","));
	for(int n = 0; n < 3; ++n)
	{
		two = rotate(two);
		fmt::println("[{}]", fmt::join(two, ","));
	}
	two = rotate(two);
	fmt::println("flip");
	two = flip(two);
	fmt::println("[{}]", fmt::join(two, ","));
	fmt::println("3x3");
	fmt::println("rotate");
	std::array<char, 9> three = { '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
	fmt::println("[{}]", fmt::join(three, ","));
	for(int n = 0; n < 3; ++n)
	{
		three= rotate(three);
		fmt::println("[{}]", fmt::join(three, ","));
	}
	three = rotate(three);
	fmt::println("flip");
	three = flip(three);
	fmt::println("[{}]", fmt::join(three, ","));
}

void test2()
{
	fmt::println("3x3");
	fmt::println("rotate");
	std::array<char, 9> three = {'.', '#', '.', '.', '.', '#', '#', '#', '#'};;
	for(int n = 0; n < 4; ++n)
	{
		fmt::println("r [{}]", fmt::join(three, ","));
		auto fl = flip(three);
		fmt::println("f [{}]", fmt::join(fl, ","));
		three = rotate(three);
	}
	fmt::println("[{}]", fmt::join(three, ","));
}

// n is the offset into v of the top left character of the 2x2 array
//
std::array<char, 4> from2(std::vector<char> const& v, size_t stride, size_t n)
{
	std::array<char, 4> r;
	r[0] = v[n];
	r[1] = v[n + 1];
	r[2] = v[n + stride];
	r[3] = v[n + stride + 1];

	return r;
}

std::array<char, 9> from3(std::vector<char> const& v, size_t stride, size_t n)
{
	std::array<char, 9> r;
	r[0] = v[n];
	r[1] = v[n + 1];
	r[2] = v[n + 2];
	r[3] = v[n + stride];
	r[4] = v[n + stride + 1];
	r[5] = v[n + stride + 2];
	r[6] = v[n + stride + stride];
	r[7] = v[n + stride + stride + 1];
	r[8] = v[n + stride + stride + 2];
	return r;
}

void to3(std::vector<char>& v, size_t stride, size_t n, std::array<char, 9> const& f)
{
	v[n] = f[0];
	v[n + 1] = f[1] ;
	v[n + 2] = f[2];
	v[n + stride] = f[3] ;
	v[n + stride + 1] = f[4];
	v[n + stride + 2] = f[5];
	v[n + stride + stride] = f[6];
	v[n + stride + stride + 1] = f[7];
	v[n + stride + stride + 2] = f[8];
}

void to4(std::vector<char>& v, size_t stride, size_t n, std::array<char, 16> const& f)
{
	size_t x = 0;
	for(size_t o = 0; o < 4; ++o)
	{
		v[n + o * stride] = f[x++] ;
		v[n + o * stride + 1] = f[x++];
		v[n + o * stride + 2] = f[x++];
		v[n + o * stride + 3] = f[x++];
	}
}

template<size_t S> auto from_sv(std::string_view v) -> std::array<char, S> 
{
	std::array<char, S> r;
	std::ranges::copy_if(v, r.begin(), [](auto c){ return c != '/';});
	return r;
}

auto get_input()
{
	std::map<std::array<char, 4>, std::array<char, 9>> m23;
	std::map<std::array<char, 9>, std::array<char, 16>> m34;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		// #../.../... => .###/.##./.##./....
		if(auto[m, f, t] = ctre::match<"([./#]+) => ([./#]+)">(ln); m)
		{
			if(f.size() == 5)
			{
				auto bs = from_sv<4>(f);
				auto to = from_sv<9>(t);
				m23.emplace(bs, to);
				m23.emplace(flip(bs), to);
				bs = rotate(bs);
				m23.emplace(bs, to);
				m23.emplace(flip(bs), to);
				bs = rotate(bs);
				m23.emplace(bs, to);
				m23.emplace(flip(bs), to);
				bs = rotate(bs);
				m23.emplace(bs, to);
				m23.emplace(flip(bs), to);
			}
			else
			{
				auto bs = from_sv<9>(f);
				auto to = from_sv<16>(t);
				m34.emplace(bs, to);
				m34.emplace(flip(bs), to);
				bs = rotate(bs);
				m34.emplace(bs, to);
				m34.emplace(flip(bs), to);
				bs = rotate(bs);
				m34.emplace(bs, to);
				m34.emplace(flip(bs), to);
				bs = rotate(bs);
				m34.emplace(bs, to);
				m34.emplace(flip(bs), to);
			}
		}
		else
			fmt::println("parse fail {}", ln);
	}
	return std::make_pair(m23, m34);
}

auto pt12(auto const& in)
{
	timer t("p12");
	auto const& m23 = in.first;
	auto const& m34 = in.second;
	int iterations = in.first.size() == 4 ? 2 : 18;
	std::vector<char> a = {'.', '#', '.', '.', '.', '#', '#', '#', '#'};
	std::vector<char> b;
	size_t str_a = 3;
	size_t str_b = 4;
	int p1 = 0;
	for(int c = 0; c < iterations; ++c)
	{
		if(iterations == 5)
			p1 = std::ranges::count(a, '#');
		if(a.size() % 4 == 0)
		{
			b.resize(a.size() / 4 * 9);
			str_b = str_a * 3 / 2;
			size_t o = 0;
			size_t d = 0;
			for(int n = 0; n < a.size() / 4; ++n)
			{
				auto pf = from2(a, str_a, o);
				if( !m23.contains(pf))
					fmt::println("tilt, pattern [{}] not in map23", fmt::join(pf, ",") );
				to3(b, str_b, d, m23.at(pf));
				o += 2;
				if( o % str_a == 0)
					o += str_a;
				d += 3;
				if(d % str_b == 0)
					d += 2 * str_b;
			}
		}
		else
		{
			b.resize(a.size() / 9 * 16);
			str_b = str_a * 4 / 3;
			size_t o = 0;
			size_t d = 0;
			for(int n = 0; n < a.size() / 9; ++n)
			{
				auto pf = from3(a, str_a, o);
				if( !m34.contains(pf))
					fmt::println("tilt, pattern [{}] not in map34", fmt::join(pf, ",") );
				to4(b, str_b, d, m34.at(pf));
				o += 3;
				if( o % str_a == 0)
					o += 2 * str_a;
				d += 4;
				if(d % str_b == 0)
					d += 3 * str_b;
			}
		}
		a.swap(b);
		str_a = str_b;
	}

	return std::make_pair(p1, std::ranges::count(a, '#'));
}


int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
