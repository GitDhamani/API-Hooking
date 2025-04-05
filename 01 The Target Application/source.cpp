#include <iostream>
#include <windows.h>

int main()
{
	Sleep(2000);
	std::cout<<"Welcome to the Exchange Rate Converter. Convert your GBP to USD.\n";
	std::cout<<"Please enter value in GBP: ";
	int pounds = 0;
	std::cin>>pounds;
	std::cout<<"This corresponds to: " << pounds*1.5 << " dollars.\n";
	system("pause");
	return 0;
}