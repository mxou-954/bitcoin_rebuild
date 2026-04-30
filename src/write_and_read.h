#ifndef WRITE_AND_READ_H
#define WRITE_AND_READ_H

#include <string>
#include "entities.h"
#include "json.hpp"
using json = nlohmann::json;

void is_file_exist(std::string path);
json export_user_in_registre(std::string path, User user)

#endif
