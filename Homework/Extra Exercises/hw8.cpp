#include <bits/stdc++.h>
const int i_MAX = 4;
const int j_MAX = 76;
const int T = 258;
const int list_to_print[] = { 1,2,4,8,16,32,64,128,256 };

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::sort;

struct rating
{
	bool seen;
	bool recommend;
};

struct student
{
	string id;
	vector<rating> ratings;
};

struct movie
{
	string name;
	long double ratio;
};

void initialize(vector<long double>& probZ_i, vector< vector<long double> >& probR_j_i, vector<student>& students, vector<movie>& movies) {
	ifstream infile_movies("hw8_movies.txt");
	ifstream infile_ids("hw8_ids.txt");
	ifstream infile_ratings("hw8_ratings.txt");
	ifstream infile_probR("hw8_probR_init.txt");
	ifstream infile_probZ("hw8_probZ_init.txt");
	
	movie temp_movie;
	long double temp_ld;
	string temp_s;
	char temp_c;

	for (int j = 0; j < j_MAX; ++j) {
		infile_movies >> temp_movie.name;
		movies.push_back(temp_movie);
	}
	
	for (int i = 0; i < i_MAX; ++i) {
		infile_probZ >> temp_ld;
		probZ_i.push_back(temp_ld);
	}
	
	vector<long double> probR_i;
	for (int j = 0; j < j_MAX; ++j) {
		for (int i = 0; i < i_MAX; ++i) {
			infile_probR >> temp_ld;
			probR_i.push_back(temp_ld);
		}
		probR_j_i.push_back(probR_i);
		probR_i.clear();
	}

	student temp_student;
	rating temp_rating;
	for (int t = 0; t < T; ++t) {
		infile_ids >> temp_s;
		temp_student.id = temp_s;
		for (int j = 0; j < j_MAX; ++j) {
			infile_ratings >> temp_c;
			if (temp_c == '?') {
				temp_rating.seen = false;
			}
			else {
				temp_rating.seen = true;
				if (temp_c == '1') {
					temp_rating.recommend = true;
				}
				else {
					temp_rating.recommend = false;
				}
			}
			temp_student.ratings.push_back(temp_rating);
		}
		students.push_back(temp_student);
		temp_student.ratings.clear();
	}
}

bool compare_movie(const movie& lhs, const movie& rhs) {
	return lhs.ratio < rhs.ratio;
}

void sanity_check(const vector<long double>& probZ_i, const vector< vector<long double> >& probR_j_i, const vector<student>& students, vector<movie>& movies) {
	for (int j = 0; j < j_MAX; ++j) {
		int numerator = 0;
		int denominator = 0;
		for (int t = 0; t < T; ++t) {
			if (students[t].ratings[j].seen) {
				++denominator;
				if (students[t].ratings[j].recommend) {
					++numerator;
				}
			}
		}
		movies[j].ratio = (long double)numerator / denominator;
	}
	
	sort(movies.begin(), movies.end(), compare_movie);
	
	for (int j = 0; j < j_MAX; ++j) {
		cout << movies[j].name << endl;
	}
}

long double probability_R(int t, const vector<long double>& probZ_i, const vector< vector<long double> >& probR_j_i, const vector<student>& students) {
	long double prob = 0;
	for (int i = 0; i < i_MAX; ++i) {
		long double product = 1;
		product *= probZ_i[i];
		for (int j = 0; j < j_MAX; ++j) {
			if (students[t].ratings[j].seen) {
				if (students[t].ratings[j].recommend) {
					product *= probR_j_i[j][i];
				}
				else {
					product *= (1 - probR_j_i[j][i]);
				}
			}
		}
		prob += product;
	}
	return prob;
}

long double probability_Z_R(int i, int t, const vector<long double>& probZ_i, const vector< vector<long double> >& probR_j_i, const vector<student>& students) {
	long double prob = 1;
	prob *= probZ_i[i];
	for (int j = 0; j < j_MAX; ++j) {
		if (students[t].ratings[j].seen) {
			if (students[t].ratings[j].seen) {
				if (students[t].ratings[j].recommend) {
					prob *= probR_j_i[j][i];
				}
				else {
					prob *= (1 - probR_j_i[j][i]);
				}
			}
		}
	}
	prob /= probability_R(t, probZ_i, probR_j_i, students);
	return prob;
}

void update(vector<long double>& probZ_i, vector< vector<long double> >& probR_j_i, const vector<student>& students) {
	vector<long double> probZ_i_new;
	vector< vector<long double> > probR_j_i_new;
	for (int i = 0; i < i_MAX; ++i) {
		long double numerator = 0;
		for (int t = 0; t < T; ++t) {
			numerator += probability_Z_R(i, t, probZ_i, probR_j_i, students);
		}
		probZ_i_new.push_back(numerator / T);
	}
	for (int j = 0; j < j_MAX; ++j) {
		vector<long double> probR_i_new;
		for (int i = 0; i < i_MAX; ++i) {
			long double numerator = 0;
			long double denominator = 0;
			for (int t = 0; t < T; ++t) {
				if (students[t].ratings[j].seen) {
					if (students[t].ratings[j].recommend) {
						numerator += probability_Z_R(i, t, probZ_i, probR_j_i, students);
					}
				}
				else {
					numerator += probability_Z_R(i, t, probZ_i, probR_j_i, students) * probR_j_i[j][i];
				}
				denominator += probability_Z_R(i, t, probZ_i, probR_j_i, students);
			}
			probR_i_new.push_back(numerator / denominator);
		}
		probR_j_i_new.push_back(probR_i_new);
		probR_i_new.clear();
	}
	probZ_i = probZ_i_new;
	probR_j_i = probR_j_i_new;
}

long double log_likelihood(const vector<long double>& probZ_i, const vector< vector<long double> >& probR_j_i, const vector<student>& students) {
	long double log_likelihood = 0;
	for (int t = 0; t < T; ++t) {
		log_likelihood += log(probability_R(t, probZ_i, probR_j_i, students));
	}
	log_likelihood /= T;
	return log_likelihood;
}

bool print_check(int k) {
	for (int i = 0; i < sizeof(list_to_print) / sizeof(int); ++i) {
		if (list_to_print[i] == k)
			return true;
	}
	return false;
}

int main() {
	vector<long double> probZ_i;
	vector< vector<long double> > probR_j_i;
	vector<student> students;
	vector<movie> movies;

	initialize(probZ_i, probR_j_i, students, movies);
	
	//sanity_check(probZ_i, probR_j_i, students, movies);

	cout << 0 << " " << log_likelihood(probZ_i, probR_j_i, students) << endl;
	for (int k = 0; k < 256; ++k) {
		update(probZ_i, probR_j_i, students);
		if (print_check(k + 1)) {
			cout << k + 1 << " " << log_likelihood(probZ_i, probR_j_i, students) << endl;
		}
	}
	
	return 0;
}
