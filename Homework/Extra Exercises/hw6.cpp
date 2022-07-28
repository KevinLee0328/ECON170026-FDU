#include <bits/stdc++.h>
const int i_MAX = 23;
const int t_MAX = 267;
const int list_to_print[] = { 1,2,4,8,16,32,64,128,256 };

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::vector;

void initialize(vector<double>& p_i, vector< vector<bool> >& x_i_t, vector<bool>& y_t) {
	ifstream infile_x("hw6_spectX.txt");
	ifstream infile_y("hw6_spectY.txt");
	bool temp;

	for (int i = 0; i < i_MAX; ++i) {
		p_i.push_back(0.05);
	}
	
	vector<bool> x_i;
	for (int t = 0; t < t_MAX; ++t) {
		for (int i = 0; i < i_MAX; ++i) {
			infile_x >> temp;
			x_i.push_back(temp);
		}
		x_i_t.push_back(x_i);
		x_i.clear();
	}
	
	for (int t = 0; t < t_MAX; ++t) {
		infile_y >> temp;
		y_t.push_back(temp);
	}
}

void sanity_check(const vector<double>& p_i, const vector< vector<bool> >& x_i_t, const vector<bool>& y_t) {
	for (int t = 0; t < t_MAX; ++t) {
		for (int i = 0; i < i_MAX; ++i) {
			cout << x_i_t[t][i] << " ";
		}
		cout << endl;
	}
	cout << endl;
	
	for (int t = 0; t < t_MAX; ++t) {
		cout << y_t[t];
	}
	cout << endl;
	
	for (int i = 0; i < i_MAX; ++i) {
		cout << p_i[i] << " ";
	}
	cout << endl;
}

double probability_Y_X(const vector<bool>& x, const vector<double>& p_i) {
	double product = 1;
	for (int i = 0; i < i_MAX; ++i) {
		if (x[i])
			product *= 1 - p_i[i];
	}
	return 1 - product;
}

double log_likelihood(const vector<double>& p_i, const vector< vector<bool> >& x_i_t, const vector<bool>& y_t) {
	double log_likelihood = 0;
	for (int t = 0; t < t_MAX; ++t) {
		if (y_t[t])
			log_likelihood += log(probability_Y_X(x_i_t[t], p_i));
		else
			log_likelihood += log(1 - probability_Y_X(x_i_t[t], p_i));
	}
	log_likelihood /= t_MAX;
	return log_likelihood;
}

double probability_Z_X_Y(int i, const vector<bool>& x, const bool& y, const vector<double>& p_i) {
	double prob = 0;
	double product = 1;
	for (int j = 0; j < i_MAX; ++j) {
		if (x[j])
			product *= 1 - p_i[j];
	}
	prob = y * x[i] * p_i[i] / (1 - product);
	return prob;
}

void update_p(vector<double>& p_i, const vector< vector<bool> >& x_i_t, const vector<bool>& y_t) {
	vector<double> new_p_i;
	for (int i = 0; i < i_MAX; ++i) {
		double numerator = 0;
		int denominator = 0;
		for (int t = 0; t < t_MAX; ++t) {
			numerator += probability_Z_X_Y(i, x_i_t[t], y_t[t], p_i);
			denominator += x_i_t[t][i];
		}
		new_p_i.push_back(numerator / denominator);
	}
	p_i = new_p_i;
}

int mistake(const vector<double>& p_i, const vector< vector<bool> >& x_i_t, const vector<bool>& y_t) {
	int mistake = 0;
	for (int t = 0; t < t_MAX; ++t) {
		if (y_t[t]) {
			if (probability_Y_X(x_i_t[t], p_i) <= 0.5)
				++mistake;
		}
		else {
			if (probability_Y_X(x_i_t[t], p_i) >= 0.5)
				++mistake;
		}
	}
	return mistake;
}

bool print_check(int k) {
	for (int i = 0; i < sizeof(list_to_print) / sizeof(int); ++i) {
		if (list_to_print[i] == k)
			return true;
	}
	return false;
}

int main() {
	vector<double> p_i;
	vector< vector<bool> > x_i_t;
	vector<bool> y_t;

	initialize(p_i, x_i_t, y_t);
	
	//sanity_check(p_i, x_i_t, y_t);

	cout << 0 << " " << mistake(p_i, x_i_t, y_t) << " " << log_likelihood(p_i, x_i_t, y_t) << endl;
	for (int k = 0; k < 256; ++k) {
		update_p(p_i, x_i_t, y_t);
		if (print_check(k + 1)) {
			cout << k + 1 << " " << mistake(p_i, x_i_t, y_t) << " " << log_likelihood(p_i, x_i_t, y_t) << endl;
		}
	}

	return 0;
}
