#pragma once

#include <array>
#include <cstdio>

#ifdef _MSC_VER
    #define LOG std::puts(__FUNCSIG__)
#endif
#ifndef _MSC_VER
    #define LOG std::puts(__PRETTY_FUNCTION__)
#endif

struct Noisy
{
	Noisy() { LOG; }
	~Noisy() { LOG; }
	Noisy(const Noisy&) { LOG; }
	Noisy& operator=(const Noisy&) { LOG; return *this; }
	Noisy(Noisy&&) { LOG; }
	Noisy& operator=(Noisy&&) { LOG; return *this; }

private:
	std::array<long, 1000> member;
};
