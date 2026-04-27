#ifndef SHA256_H
#define SHA256_H

#include <string>

using namespace std;

string sha256(const string &str);
string generate_hash_with_zeros(int difficulty);

#endif
