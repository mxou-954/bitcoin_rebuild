#include "security_check_for_pay.h"
#include "entities.h"
#include "sha256.h"
#include "utxos.h"

bool security_check_for_pay(User currentUser, std::optional<User> match_address, std::optional<User> match_public_key, std::vector<std::string> user_command_part) {
    if(!match_address.has_value()){
        std::cout << "L'adresse de destination n'appartient a aucun utilisateur !" << std::endl;
        return false;
    }
    if(!match_public_key.has_value()){
        std::cout << "La clé public ne correspond pas a un utilisateur valide !" << std::endl;
        return false;
    }
    if(sha256(currentUser.privateKey) != user_command_part[1]){
        std::cout << "La clé public n'est pas celle lié a votre compte." << std::endl;
        return false;
    }
    if(stod(user_command_part[2]) > currentUser.balance){
        std::cout << "Vous n'avez pas les fonds nécessaire pour le payement !" << std::endl;
        return false;
    }
    if(user_command_part[3] == currentUser.address){
        std::cout << "Vous ne pouvez pas vous envoyer de l'argent depuis votre propre compte !" << std::endl;
        return false;
    } if(utxos_sum_balance(currentUser) < stod(user_command_part[2])){
        std::cout << "Vous n'avez pas les fonds nécessaire pour le payement !" << std::endl;
        return false;
    }
    return true;
}