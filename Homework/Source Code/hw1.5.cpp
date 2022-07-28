#include<bits/stdc++.h>
int digit(int num, int dig)
{
	int dividend = pow(2, dig);
	return (num / dividend) % 2;
}
//digit 0:E 1:B 2:A 3:J 4:M

int main()
{
	double prob[32];
	for(int i = 0; i < 32; ++i)
	{
		double temp = 1;
		
		if(digit(i, 0) == 1) temp *= 0.005;
		else temp *= 0.995;
		
		if(digit(i, 1) == 1) temp *= 0.001;
		else temp *= 0.999;
		
		if(digit(i, 2) == 1){
			if(digit(i, 1) == 1){
				if(digit(i, 0) == 1){
					temp *= 0.98;
				}
				else{
					temp *= 0.96;
				}
			}
			else{
				if(digit(i, 0) == 1){
					temp *= 0.35;
				}
				else{
					temp *= 0.002;
				}
			}
		}
		else{
			if(digit(i, 1) == 1){
				if(digit(i, 0) == 1){
					temp *= 0.02;
				}
				else{
					temp *= 0.04;
				}
			}
			else{
				if(digit(i, 0) == 1){
					temp *= 0.65;
				}
				else{
					temp *= 0.998;
				}
			}
		}
		
		if(digit(i, 3) == 1){
			if(digit(i, 2) == 1){
				temp *= 0.93;
			}
			else{
				temp *= 0.1;
			}
		}
		else{
			if(digit(i, 2) == 1){
				temp *= 0.07;
			}
			else{
				temp *= 0.9;
			}
		}
		
		if(digit(i, 4) == 1){
			if(digit(i, 2) == 1){
				temp *= 0.65;
			}
			else{
				temp *= 0.01;
			}
		}
		else{
			if(digit(i, 2) == 1){
				temp *= 0.35;
			}
			else{
				temp *= 0.99;
			}
		}
		
		prob[i] = temp;
	}
	
	double numerator = 0, denominator = 0;
	for(int i = 0; i < 32; ++i){
		if(digit(i, 2) == 1 && digit(i, 4) == 0 && digit(i, 0) == 1) numerator += prob[i]; //numerator
		if(digit(i, 4) == 0 && digit(i, 0) == 1) denominator += prob[i]; //denominator
	}
	//digit 0:E 1:B 2:A 3:J 4:M
	
	double result = numerator / denominator;
	std::cout << result;
	
	return 0;
}
