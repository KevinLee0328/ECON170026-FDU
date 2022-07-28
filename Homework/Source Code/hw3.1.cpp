#include<bits/stdc++.h>
int digit(int num, int dig)
{
	int dividend = pow(2, dig);
	return (num / dividend) % 2;
}
//digit 0:T 1:F 2:A 3:S 4:L 5:R

int main()
{
	double prob[64];
	for(int i = 0; i < 64; ++i)
	{
		double temp = 1;
		
		if(digit(i, 0) == 1) temp *= 0.02;
		else temp *= 0.98;
		
		if(digit(i, 1) == 1) temp *= 0.01;
		else temp *= 0.99;
		
		if(digit(i, 2) == 1){
			if(digit(i, 1) == 1){
				if(digit(i, 0) == 1){
					temp *= 0.5;
				}
				else{
					temp *= 0.99;
				}
			}
			else{
				if(digit(i, 0) == 1){
					temp *= 0.85;
				}
				else{
					temp *= 0.0001;
				}
			}
		}
		else{
			if(digit(i, 1) == 1){
				if(digit(i, 0) == 1){
					temp *= 0.5;
				}
				else{
					temp *= 0.01;
				}
			}
			else{
				if(digit(i, 0) == 1){
					temp *= 0.15;
				}
				else{
					temp *= 0.9999;
				}
			}
		}
		
		if(digit(i, 3) == 1){
			if(digit(i, 1) == 1){
				temp *= 0.9;
			}
			else{
				temp *= 0.01;
			}
		}
		else{
			if(digit(i, 1) == 1){
				temp *= 0.1;
			}
			else{
				temp *= 0.99;
			}
		}
		
		if(digit(i, 4) == 1){
			if(digit(i, 2) == 1){
				temp *= 0.88;
			}
			else{
				temp *= 0.001;
			}
		}
		else{
			if(digit(i, 2) == 1){
				temp *= 0.12;
			}
			else{
				temp *= 0.999;
			}
		}
		
		if(digit(i, 5) == 1){
			if(digit(i, 4) == 1){
				temp *= 0.75;
			}
			else{
				temp *= 0.01;
			}
		}
		else{
			if(digit(i, 4) == 1){
				temp *= 0.25;
			}
			else{
				temp *= 0.99;
			}
		}
		
		prob[i] = temp;
	}
	
	double numerator = 0, denominator = 0, sum = 0;
	for(int i = 0; i < 64; ++i){
		sum += prob[i];
		if(digit(i, 0) == 1 && digit(i, 3) == 1 && digit(i, 5) == 1) numerator += prob[i]; //numerator
		if(digit(i, 3) == 1 && digit(i, 5) == 1) denominator += prob[i]; //denominator
	}
	//digit 0:T 1:F 2:A 3:S 4:L 5:R
	
	double result = numerator / denominator;
	std::cout << sum << std::endl << result;
	
	return 0;
}
