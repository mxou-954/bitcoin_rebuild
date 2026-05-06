# Bitcoin Rebuild

Simulation d'un nœud Bitcoin en ligne de commande, écrite en C++. Le projet implémente les mécanismes fondamentaux de Bitcoin : gestion de comptes, UTXOs, mempool, minage par preuve de travail, et persistance JSON.

> Projet personnel — fait pour apprendre et s'amuser.

---

## Fonctionnalités

- Création de comptes avec clé privée, clé publique et adresse générées aléatoirement
- Authentification par clé publique
- Paiements BTC via le modèle UTXO
- Transferts internes entre utilisateurs
- Achat de BTC en euros (taux fixe)
- Mempool : file d'attente des transactions en attente
- Minage : validation des transactions et ajout au bloc
- Blockchain persistée en JSON
- Affichage de la blockchain, des blocs, du solde

---

## Prérequis

- C++17 ou supérieur
- CMake ≥ 3.15
- OpenSSL (pour sha256)

```bash
# Ubuntu / Debian
sudo apt install cmake libssl-dev

# macOS
brew install cmake openssl
```

---

## Installation

```bash
git clone https://github.com/ton-user/bitcoin-rebuild.git
cd bitcoin-rebuild
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Lancement

```bash
./bitcoin
```

Au démarrage, 3 utilisateurs sont générés automatiquement avec leurs clés affichées dans le terminal. Tu peux t'en servir pour te connecter.

---

## Commandes

### Sans connexion

| Commande | Description |
|---|---|
| `/users` | Liste tous les utilisateurs (adresse, balance, clé publique) |
| `/c [publicKey]` | Se connecter à un compte |
| `/mempool` | Afficher les transactions en attente |
| `/help` | Afficher l'aide |
| `/q` | Quitter |

### Connecté

| Commande | Description |
|---|---|
| `/balance` | Afficher son solde en satoshis |
| `/pay [publicKey] [amount] [address]` | Envoyer des BTC à un utilisateur |
| `/transfer [euros] [publicKey]` | Acheter des BTC en euros et les transférer |
| `/mine [nonce]` | Miner le prochain bloc (valide le mempool) |
| `/blockchain` | Afficher toute la blockchain |
| `/find_block_by_index [index]` | Chercher un bloc par index |
| `/find_block_by_hash [hash]` | Chercher un bloc par hash |
| `/swipe [currentPublicKey] [nextPublicKey]` | Changer de compte |
| `/d` | Se déconnecter |
| `/help` | Afficher l'aide |
| `/q` | Quitter |

---

## Architecture

```
BITCOIN_REBUILD/
├── registre/
│   ├── users.json          ← comptes persistés
│   ├── blockchain.json     ← blocs validés
│   └── mempool.json        ← transactions en attente
└── src/
    ├── entities.h          ← structs : User, Block, UTXO, Transaction, Mempool
    ├── main.cpp            ← boucle principale + routing des commandes
    │
    ├── sha256              ← hachage (OpenSSL)
    ├── generate_random_number
    ├── security_check_for_pay
    │
    ├── new_user            ← création de compte
    ├── login               ← connexion
    ├── swipe_user          ← changement de compte
    ├── sync_user           ← synchronisation liste users
    ├── find_user           ← recherche par clé publique
    ├── is_currentUser      ← vérification connexion
    │
    ├── pay                 ← paiement BTC (UTXO)
    ├── transfer            ← transfert interne
    ├── transfer_eur_to_btc ← achat BTC
    ├── utxos               ← gestion UTXOs
    │
    ├── block               ← minage + gestion blocs
    │
    ├── btc_to_satochi      ← conversion BTC ↔ satoshis
    ├── split               ← parsing des commandes
    ├── write_and_read      ← persistance JSON
    │
    ├── mempool_output      ← affichage mempool
    ├── help                ← affichage aide
    └── user_input          ← lecture input utilisateur
```

---

## Modèle UTXO

Chaque paiement crée un UTXO (Unspent Transaction Output) attaché au destinataire. Le solde d'un utilisateur est la somme de ses UTXOs non dépensés, exprimée en satoshis (1 BTC = 100 000 000 satoshis).

```
/pay [publicKey] [amount] [address]
  └── vérifie les UTXOs disponibles
  └── crée la transaction → mempool
  └── /mine valide la transaction → blockchain
```

---

## Roadmap

- [ ] Refacto : séparer `block.cpp` en `miner.cpp` + `block_display.cpp`
- [ ] Refacto : regrouper les fichiers par domaine (`user/`, `transaction/`, `crypto/`)
- [ ] Renommer `btc_to_satochi` → `converter` (faute d'ortho)
- [ ] Extraire `btc_price` dans un `constants.h`
- [ ] Remplacer le `if/else if` de `main.cpp` par une `std::map<string, function>`
- [ ] Ajouter des frais de transaction (fees) dynamiques
- [ ] Difficulté de minage ajustable
- [ ] Vérification de la chaîne (intégrité des hashs)
- [ ] Multi-nœuds (simulation réseau P2P)
- [ ] Interface TUI (ncurses)

---

## Licence

Projet personnel — libre de réutilisation.