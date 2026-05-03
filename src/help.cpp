#include <string>
#include <iostream>

void help(){
    std::cout << "\n==========================================\n";
    std::cout << "         COMMANDES DISPONIBLES\n";
    std::cout << "==========================================\n";
    std::cout << "\n--- Compte ---\n";
    std::cout << "/balance\n";
    std::cout << "   -> Affiche votre solde et vos UTXOs\n";
    std::cout << "/d\n";
    std::cout << "   -> Deconnexion\n";
    std::cout << "/swipe [publicKey_actuel] [publicKey_suivant]\n";
    std::cout << "   -> Changer de compte\n";
    std::cout << "\n--- Transactions ---\n";
    std::cout << "/pay [publicKey] [amount_BTC] [address_destination]\n";
    std::cout << "   -> Envoyer des BTC a une adresse\n";
    std::cout << "/transfer [euros] [publicKey]\n";
    std::cout << "   -> Convertir des euros en BTC\n";
    std::cout << "\n--- Blockchain ---\n";
    std::cout << "/mine [nonce]\n";
    std::cout << "   -> Soumettre un nonce pour valider le bloc en cours\n";
    std::cout << "/blockchain\n";
    std::cout << "   -> Afficher tous les blocs\n";
    std::cout << "/find_block_by_index [index]\n";
    std::cout << "   -> Trouver un bloc par son index\n";
    std::cout << "/find_block_by_hash [hash]\n";
    std::cout << "   -> Trouver un bloc par son hash\n";
    std::cout << "\n--- Connexion ---\n";
    std::cout << "/c [publicKey]\n";
    std::cout << "   -> Se connecter a un compte\n";
    std::cout << "\n--- Informations ---\n";
    std::cout << "/users\n";
    std::cout << "   -> Lister tous les utilisateurs\n";
    std::cout << "/mempool\n";
    std::cout << "   -> Voir les transactions en attente\n";
    std::cout << "\n--- General ---\n";
    std::cout << "/help\n";
    std::cout << "   -> Afficher cette aide\n";
    std::cout << "/q\n";
    std::cout << "   -> Quitter\n";
    std::cout << "==========================================\n";
}