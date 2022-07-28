#include <bits/stdc++.h>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::sort;

typedef std::size_t size_type;

char correct_letter[9][2] = { {},{},{'Q'},{'Q'},{'Z','E'},{},{'D','I'},{'D','I'},{'U'} };
int correct_digit[9][2] = { {},{},{0},{0},{2,3},{},{0,3},{0,3},{1} };
char incorrect_letter[9][5] = { {},{'E','O'},{},{'U'},{'A','D','I','R'},{'E','O'},{},{'A'},{'A','E','I','O','S'} };

struct Node
{
	string word;
	size_type frequency;
	double probability;
};

void initialize(vector<Node>& dictionary)
{
	ifstream infile("hw2_word_counts_05.txt");
	Node temp_node;
	
	while (infile >> temp_node.word >> temp_node.frequency) {
		dictionary.push_back(temp_node);
	}
	
	size_type sum = 0;
	for (vector<Node>::iterator iter = dictionary.begin(); iter != dictionary.end(); ++iter) {
		sum += iter->frequency;
	}
	for (vector<Node>::iterator iter = dictionary.begin(); iter != dictionary.end(); ++iter) {
		iter->probability = (double)iter->frequency / sum;
	}
}

bool compare(const Node& lhs, const Node& rhs)
{
	return lhs.probability > rhs.probability;
}

void sanity_check(vector<Node>& dictionary)
{
	cout << "print out the ten most and least frequent 5-letter words as a sanity check:" << endl;
	
	sort(dictionary.begin(), dictionary.end(), compare);
	
	for (vector<Node>::iterator iter = dictionary.begin(); iter != dictionary.begin() + 10; ++iter) {
		cout << iter->word << " " << iter->frequency << " " << iter->probability << endl;
	}
	for (vector<Node>::iterator iter = dictionary.end() - 1; iter != dictionary.end() - 11; --iter) {
		cout << iter->word << " " << iter->frequency << " " << iter->probability << endl;
	}
	cout << endl;
}

bool correctly_guessed(Node& node, int digit, char guess) {
	if (node.word[digit] == guess) {
		for (int i = 0; i < 5; ++i) {
			if (i != digit && node.word[i] == guess) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool incorrectly_guessed(Node& node, char guess) {
	for (int i = 0; i < 5; ++i) {
		if (node.word[i] == guess) {
			return false;
		}
	}
	return true;
}

bool evidence_check(int line, Node& node) {
	for (int i = 0; i < 2 && correct_letter[line][i] != 0; ++i) {
		if (!correctly_guessed(node, correct_digit[line][i], correct_letter[line][i])) {
			return false;
		}
	}
	for (int i = 0; i < 5 && incorrect_letter[line][i] != 0; ++i) {
		if (!incorrectly_guessed(node, incorrect_letter[line][i])) {
			return false;
		}
	}
	return true;
}

double predictive_probability(int line, char letter, vector<Node>& dictionary) {
	double sum = 0, prob = 0;
	for (vector<Node>::iterator iter = dictionary.begin(); iter != dictionary.end(); ++iter) {
		if (evidence_check(line, *iter)) {
			sum += iter->probability;
		}
	}
	for (vector<Node>::iterator iter = dictionary.begin(); iter != dictionary.end(); ++iter) {
		if (!incorrectly_guessed(*iter, letter)) {
			if (evidence_check(line, *iter)) {
				prob += iter->probability / sum;
			}
		}
	}
	return prob;
}

bool letter_guessed(int line, char letter) {
	for (int i = 0; i < 2 && correct_letter[line][i] != 0; ++i) {
		if (correct_letter[line][i] == letter) {
			return true;
		}
	}
	for (int i = 0; i < 5 && incorrect_letter[line][i] != 0; ++i) {
		if (incorrect_letter[line][i] == letter) {
			return true;
		}
	}
	return false;
}

int main()
{
	vector<Node> dictionary;
	
	initialize(dictionary);
	
	sanity_check(dictionary);

	for (int j = 0; j < 9; ++j) {
		double max_probability = 0;
		char max_letter;
		for (int i = 0; i < 26; ++i) {
			char letter = 'A' + i;
			if (!letter_guessed(j, letter) && predictive_probability(j, letter, dictionary) > max_probability) {
				max_probability = predictive_probability(j, letter, dictionary);
				max_letter = letter;
			}
		}
		cout << "The next best guess is " << max_letter << " with probability " << max_probability << endl;
	}
	
	return 0;
} 
