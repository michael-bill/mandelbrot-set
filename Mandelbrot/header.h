#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

struct Complex
{
	double real;
	double imaginary;

	Complex(const double& re, const double& im) : real(re), imaginary(im) {}

	Complex operator += (const Complex& other)
	{
		real += other.real;
		imaginary += other.imaginary;
		return *this;
	}

	void sqr()
	{
		double temp = (real * real) - (imaginary * imaginary);
		imaginary = (2 * real * imaginary);
		real = temp;
	}

	bool pointNotFits() const
	{
		return (real * real) + (imaginary * imaginary) > 4;
	}
};
