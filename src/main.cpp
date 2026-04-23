#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>

#include "entities.h"

#include <openssl/sha.h>

using namespace std;

static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";
int size = sizeof(alphanum) - 1;

string generate_random_number(int length){
    srand(time(0));
    string result = "";
	for (int i = 0; i < length; i++)
	{
		result += alphanum[rand() % ::size];
	}
    return result;
}

string sha256(const string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

User signIn() {
    std::string prv_key = generate_random_number(255);
    std::string pblc_key = sha256(prv_key);
    std::string address = "btc1" + generate_random_number(50);
    
    User new_user = User(prv_key, pblc_key, address, 0.0);

    return new_user;
}

int main() {
    User user = signIn();
    std::cout << "user_address: " << user.address << "\n";
    std::cout << "user_privateKey: " << user.privateKey << "\n";
    std::cout << "user_publicKey: " << user.publicKey << "\n";
    std::cout << "user_balance: " << user.balance << "\n";
}