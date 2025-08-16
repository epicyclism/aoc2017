#pragma once

#include <bitset>

template<size_t X, size_t Y> class bs_2d_t
{
private:
    std::bitset<X*Y> bs_;
public:
    void set(size_t x, size_t y)
    {
        bs_.set(y * X + x);
    }
    void reset(size_t x, size_t y)
    {
        bs_.reset(y * X + x);
    }
    void flip(size_t x, size_t y)
    {
        bs_.flip(y * X + x);
    }
    bool test(size_t x, size_t y)
    {
        return bs_.test(y * X + x);
    }
};

template<size_t X, size_t Y> class bs_2d_sym_t
{
private:
    std::bitset<X*Y> bs_;
    const int x_2 = X / 2;
    const int y_2 = Y / 2;
public:
    void set(int x, int y)
    {
        x += x_2;
        y += y_2;
        bs_.set(y * X + x);
    }
    void reset(int x, int y)
    {
        x += x_2;
        y += y_2;
        bs_.reset(y * X + x);
    }
    void flip(int x, int y)
    {
        x += x_2;
        y += y_2;
        bs_.flip(y * X + x);
    }
    bool test(int x, int y)
    {
        x += x_2;
        y += y_2;
        return bs_.test(y * X + x);
    }
};