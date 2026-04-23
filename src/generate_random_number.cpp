#include "generate_random_number.h"
#include <cstdlib>

using namespace std;

static const char alphanum[] = "0123456789!@#$%^&*ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static int alphanum_size = sizeof(alphanum) - 1;

string generate_random_number(int length){
    string result = "";
    for (int i = 0; i < length; i++){
        result += alphanum[rand() % alphanum_size];
    }
    return result;
}