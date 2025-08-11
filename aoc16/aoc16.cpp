#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

enum class op { SPIN, EXCHANGE, PARTNER};

struct op_t
{
	op op_;
	int a_;
	int b_;
};

auto abx(std::string_view v)
{
	auto p = v.find('/');
	auto a = sv_to_t<int>(std::string_view(v.data(), p));
	auto b = sv_to_t<int>(std::string_view(v.begin() + p + 1, v.end()));
	return std::make_pair(a, b);
}

auto abp(std::string_view v)
{
	auto a = v[0];
	auto b = v[2];
	return std::make_pair(a, b);
}

auto get_input()
{
	std::vector<op_t> vo;
	std::string ln;
	std::getline(std::cin, ln);
	for(auto m : ctre::split<",">(ln))
	{
		auto v = m.to_view();
		auto o = v[0];
		v.remove_prefix(1);
		switch(o)
		{
			case 's':
				vo.emplace_back(op::SPIN, sv_to_t<int>(v), 0);
				break;
			case 'x':
				{
					auto[a, b] = abx(v);
					vo.emplace_back(op::EXCHANGE, a, b);
				}
				break;
			case 'p':
				{
					auto[a, b] = abp(v);
					vo.emplace_back(op::PARTNER, a, b);
				}
				break;
			default:
				fmt::println("tilt {}{}", o, v);
				break;
		}
	}
	return vo;
}

std::string pt1(auto const& in)
{
	timer t("p1");

	std::array<char, 16> a;
	std::iota(a.begin(), a.end(), 'a');
	for(auto& o: in)
	{
		switch(o.op_)
		{
			case op::SPIN:
				std::ranges::rotate(a, a.begin() + a.size() - o.a_);
				break;
			case op::EXCHANGE:
				std::swap(a[o.a_], a[o.b_]);
				break;
			case op::PARTNER:
				{
					auto A = std::ranges::find(a, o.a_);
					auto B = std::ranges::find(a, o.b_);
					std::iter_swap(A, B);
				}
				break;
		}
	}
	return std::string(a.data(), a.size());
}

auto pt2(auto const& in)
{
	timer t("p2");

	std::array<char, 16> a;
	std::iota(a.begin(), a.end(), 'a');
	std::map<std::string, int> mp;
	mp.emplace(std::string(a.data(), a.size()), mp.size());
	while(1)
	{
		for(auto& o: in)
		{
			switch(o.op_)
			{
				case op::SPIN:
					std::ranges::rotate(a, a.begin() + a.size() - o.a_);
					break;
				case op::EXCHANGE:
					std::swap(a[o.a_], a[o.b_]);
					break;
				case op::PARTNER:
					{
						auto A = std::ranges::find(a, o.a_);
						auto B = std::ranges::find(a, o.b_);
						std::iter_swap(A, B);
					}
					break;
			}
		}
		if( mp.contains(std::string(a.data(), a.size())))
			break;
		mp.emplace(std::string(a.data(), a.size()), mp.size());
	}
	auto k = 1000000000 % mp.size();
	for(auto kv : mp)
		if( kv.second == k)
			return kv.first;
	return std::string("not found");
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
