#include "sha256.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

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

// Génère n'importe quel hash qui commence par "x*0"
std::string generate_hash_with_zeros(int difficulty) {
    std::string target(difficulty, '0');
    int nonce = 0;
    
    while(true) {
        std::string hash = sha256(std::to_string(nonce));
        if(hash.substr(0, difficulty) == target){
            return hash;
        }
        nonce++;
    }
}