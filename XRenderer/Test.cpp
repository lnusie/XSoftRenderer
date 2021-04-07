#include "Test.h"
#include <iostream>
#include <string>
//#include "geometry.h"

void Test::DOTest(void)
{/*
	Cls cls(1,2);
	std::cout << cls.Get() << "\n";
	cls.Set(10, 20);
	std::cout << cls.Get() << "\n";*/
	/*std::string str1 = "v 0.000283538 -1 0.286843";
	std::cout << str1.compare(0, 5, "v ") << "\n";
	if (str1.compare(0,1,"v") == 0)
	{
		std::cout << "true";
	}
	else
	{
		std::cout << "false";
	}*/
	

}

void Cls::Set(int v1, int v2)
{
	a = v1;
	b = v2;
}

int Cls::Get()
{
	return a+b;
}

Cls::Cls(int v1, int v2)
{
	a = v1;
	b = v2;
}
