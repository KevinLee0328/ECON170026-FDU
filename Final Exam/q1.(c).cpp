#include<bits/stdc++.h>
int digit(int num, int dig)
{
	int dividend = pow(2, dig);
	return (num / dividend) % 2;
}
//digit 0:M 1:C 2:L

int main()
{
	double prob[8];
	for(int i = 0; i < 8; ++i)
	{
		double temp = 1;
		
		if(digit(i, 0) == 1) temp *= 0.05;
		else temp *= 0.95;
		
		if(digit(i, 1) == 1) temp *= 0.01;
		else temp *= 0.99;
		
		if(digit(i, 2) == 1){
			if(digit(i, 1) == 1){
				if(digit(i, 0) == 1){
					temp *= 0.44;
				}
				else{
					temp *= 0.3;
				}
			}
			else{
				if(digit(i, 0) == 1){
					temp *= 0.2;
				}
				else{
					temp *= 0;
				}
			}
		}
		else{
			if(digit(i, 1) == 1){
				if(digit(i, 0) == 1){
					temp *= 0.56;
				}
				else{
					temp *= 0.7;
				}
			}
			else{
				if(digit(i, 0) == 1){
					temp *= 0.8;
				}
				else{
					temp *= 1;
				}
			}
		}
		
		prob[i] = temp;
	}
	
	double numerator = 0, denominator = 0;
	for(int i = 0; i < 8; ++i){
		if(digit(i, 0) == 1 && digit(i, 1) == 1 && digit(i, 2) == 1) numerator += prob[i]; //numerator
		if(digit(i, 1) == 1 && digit(i, 2) == 1) denominator += prob[i]; //denominator
	}
	//digit 0:M 1:C 2:L
	
	double result = numerator / denominator;
	std::cout << result;
	
	return 0;
}
