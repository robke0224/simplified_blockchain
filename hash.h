#ifndef HASH_UTIL_H
#define HASH_UTIL_H

#include <string>
#include <fstream>
#include <random>

using namespace std;

string keiciaIvesti(const string& input, char key, int shift, bool invert, bool useNand);
string ivestis_i_bitus(string input, int minLength, char padChar);
char hexas(char hexChar, int step);
string sesiolika_bitu(string& bits, const string& originalInput);
int priebalses(const string& input);
string daugyba (string bits, int wordSum, bool invert);
void apdoroja(const string& input, ofstream& outputFile);
void failiukas(const string& filename, char c, size_t size);
void kratinukas(const string& filename, size_t size);
void nevienodi(const string& filename1, const std::string& filename2, size_t size);
void tuscias(const string& filename);
void testukas1(const string& filename, int lineCount);
string poros_random(int length);
void kolizijos(const vector<pair<string, string> >& stringPairs);
vector<pair<string, string> > loadStringPairsFromFile(const string& filename);
string generate_random_string(size_t length, const string& charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
double compute_bit_difference(const string &binary1, const string &binary2);
double compute_hex_difference(const string &hex1, const string &hex2);
void fillBits(std::bitset<256>& bits, unsigned long long sum);
string getHashString(const string& input);

#endif
