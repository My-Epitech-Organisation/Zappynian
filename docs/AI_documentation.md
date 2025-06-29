## 📘 Documentation de l’IA Zappy

### 1️⃣ Présentation

Ce projet implémente un client IA pour le jeu Zappy. L’objectif est de contrôler un drone Trantorien qui :

* survit en récoltant de la nourriture,
* collecte des ressources,
* coordonne des incantations pour s’élever dans la hiérarchie,
* se reproduit pour étendre la population de l’équipe.

L’IA communique avec le serveur via TCP selon le protocole défini dans le sujet Zappy.

---

### 2️⃣ Architecture générale

Le projet est organisé ainsi :

```
ai/
├── README.md
├── Makefile
└── src/
    ├── zappy_ai.py
    ├── command_queue.py
    ├── connection.py
    ├── inventory_parser.py
    ├── vision_parser.py
    ├── roles/
    │   ├── attack.py
    │   ├── breeder.py
    │   ├── game_logic.py
    │   ├── leader.py
    │   ├── miner.py
    │   └── survivor.py
    └── utils/
        ├── incantation_data.py
        └── route_factory.py
```

---

📜 Protocole de communication de l’IA

Après la connexion, le serveur envoie "WELCOME".

L’IA répond avec "TEAM <team_name>".

Le serveur renvoie :

Le nombre de slots disponibles pour l’équipe.

Les dimensions du monde.

Les commandes envoyées doivent se terminer par un retour à la ligne. Chaque commande a un délai spécifique en fonction de la fréquence du serveur (f).

L’IA peut envoyer jusqu’à 10 commandes sans attendre de réponse.

Les commandes principales incluent : Forward, Right, Left, Look, Inventory, Broadcast, Connect_nbr, Fork, Eject, Take, Set, Incantation.

Les réponses possibles sont : ok, ko, dead, Elevation underway, Current level, et des listes pour Inventory ou Look.

---

### 3️⃣ Boucle principale (`zappy_ai.py`)

* Connexion au serveur et handshake.
* Envoie systématiquement un batch de commandes à chaque cycle :

  * `Inventory`, `Look`, `Connect_nbr`.
* Attend toutes les réponses attendues, associées à une queue (`expected_responses`).
* Traite les broadcasts dès leur réception avec `parse_broadcast()`.
* Lance le rôle approprié en fin de cycle via `select_role()`.
* Gère le spawn de nouveaux joueurs si `Connect_nbr` le permet.

---

### 4️⃣ Gestion des commandes (`command_queue.py`)

* Maintient une queue (`queue`) des commandes à envoyer.
* Maintient `expected_responses` pour suivre l’ordre des réponses attendues.
* Méthodes principales :

  * `push(command)` : ajoute une commande à la queue.
  * `flush()` : envoie toutes les commandes de la queue.
  * `handle_response(world, line)` : décrémente `pending` et gère la mise à jour du niveau avec Current level.

---

### 5️⃣ Communication réseau (`connection.py`)

* Établit la connexion TCP avec le serveur.
* Utilise poll pour une lecture non bloquante des réponses.
* Méthodes :

  * `send_command(cmd)`
  * `read_line()`

---

### 6️⃣ Analyse des réponses

#### Inventaire (`inventory_parser.py`)

* Parse les réponses `[ food 5, linemate 1, ...]` pour mettre à jour `WorldState`.
* Détecte la nourriture restante et les ressources possédées.

#### Vision (`vision_parser.py`)

* Parse les réponses `[player, food linemate, ...]` pour mettre à jour la vision du joueur.
* Stocke la liste des objets sur chaque tuile visible.

---

### 7️⃣ Gestion des rôles (`roles/`)

#### `game_logic.py`

* Décide dynamiquement quel rôle doit s’exécuter à chaque cycle en fonction de l’état courant (faim, ressources, niveau, etc.).
* Rôles implémentés :

  * `survivor` : survie par la recherche de nourriture.
  * `miner` : collecte de ressources manquantes.
  * `leader` : planification et déclenchement des incantations.
  * `breeder` : reproduction via Fork.
  * `attack` : comportement offensif (si activé).

#### Exemple de rôles :

* Dans `survivor.py`, si la nourriture est en dessous du seuil critique, l’IA planifie un chemin vers la nourriture la plus proche.
* Dans `leader.py`, le joueur vérifie les prérequis pour lancer une incantation et déclenche l’incantation si possible.

---

### 8️⃣ Coordination via Broadcast

* Les IA échangent via la commande `Broadcast text`.
* Lorsqu’un IA se désigne comme leader, elle broadcast "i\_m\_leader".
* Les autres IA écoutent avec `parse_broadcast()` et planifient un chemin pour rejoindre le leader.

---

### 9️⃣ Gestion des montées de niveau

* L’IA gère l’incantation en deux temps :

  * attend "Elevation underway",
  * puis attend "Current level: X" ou "ko".
* Lorsqu’une incantation réussit, l’IA met à jour son niveau et broadcast un message pour informer les autres IA.

---

### 🔔 Points clés pour l’utilisation

* Assurez-vous que le serveur Zappy est lancé avec la bonne configuration (`-p port -x width -y height -n team -c clientsNb -f freq`).
* Lancez l’IA avec :

  ```bash
  ./zappy_ai -p <port> -n <team_name> -h <host>
  ```
* Pour créer plusieurs IA, ajustez Connect\_nbr ou lancez plusieurs processus.

---

### ✅ Points forts de l’IA

✔️ Architecture modulaire claire.
✔️ Gestion propre des cycles et des réponses.
✔️ Bonne robustesse avec la queue et expected\_responses.
✔️ Logs utiles pour déboguer.

---

### ⚠️ Limites connues

* Dépendance à la synchronisation parfaite entre commandes envoyées et réponses reçues : un Look automatique peut encore désynchroniser la queue si non géré explicitement.
* La coordination entre IA pour les incantations est basique (broadcast leader), et peut échouer si plusieurs leaders se désignent simultanément.

---
