# 🧠 Zappy - Epitech Year 2 Project

Zappy is a real-time multiplayer network game where multiple AI teams compete to achieve a single goal: **have 6 players of the same team reach level 8**. The project is composed of **three independent modules**:

* A **server** written in C
* A **graphical client** written in C++ (using Irrlicht)
* An **AI** written in Python

---

## 📁 Project Structure

```
.
├── server/              # Server source code (C)
├── gui/                 # Graphical client (C++ with Irrlicht)
├── ai/                  # AI in Python
├── scripts/             # Launch and test scripts
├── docs/                # Technical documentation
└── README.md            # This file
```

---

## ⚙️ Dependencies

### Server (C)

* `gcc` or `clang`
* `make`
* `valgrind` (optional for debugging)

### Graphical Client (C++)

* `g++`
* [**Irrlicht**](https://irrlicht.sourceforge.net/)

  * Recommended install: via package manager (e.g. `libirrlicht-dev` on Debian/Ubuntu)

```bash
sudo apt install libirrlicht-dev
```

### AI (Python)

* Python ≥ 3.8
* Python dependencies (recommended: use a virtual environment)

```bash
sudo apt install python3
```

---

## 🔨 Compilation

### Server

```bash
cd server/
make
```

This will produce the `zappy_server` binary.

### Graphical Client

```bash
cd gui/
make
```

This will produce the `zappy_gui` binary.

---


### AI Client

```bash
cd ai/
make
```

This will produce the `zappy_ai` binary.

---

### For all

```bash
make
```

This will produce the 3 binaries

---

## 🚀 Launching the Game

### 1. Launch the Server

```bash
./zappy_server -p <PORT> -x <WIDTH> -y <HEIGHT> -n <TEAM1> <TEAM2> ... -c <CLIENTS> -f <FREQ>
```

Example:

```bash
./zappy_server -p 4242 -x 20 -y 20 -n Team1 Team2 -c 6 -f 100
```

### 2. Launch the Graphical Client

```bash
./zappy_gui -p 4242 -h 127.0.0.1
```

### 3. Launch the Python AI

```bash
cd ai/
python3 zappy_ai.py -p 4242 -h 127.0.0.1 -n Team1
```

ℹ️ You need to launch one zappy_ai for one team

---

## 🎯 Game Objective

The game is won by the first team to have **6 of its players reach level 8**. To achieve this, AIs must:

* Stay alive by collecting food
* Gather the required resources to level up
* Use `broadcast` to coordinate with teammates
* Use `Fork` to create new team players

---

## 🧩 Server Commands Supported by the AI

* `Forward`
* `Right` / `Left`
* `Look`
* `Inventory`
* `Take <item>`
* `Set <item>`
* `Broadcast <msg>`
* `Connect_nbr`
* `Fork`
* `Incantation`

⏱️ Each command has an execution time depending on the server frequency (`-f`).

---

## 🧪 Testing & Debugging

* `valgrind ./zappy_server ...`: check for memory leaks on the server side
* Python AI: enable debug logs with `DEBUG=1 python3 zappy_ai.py ...`
* Use tools from the `scripts/` folder to automate tests or level-up scenarios

---

## 📚 Documentation

Available in the `docs/` folder !

---

## 👨‍💻 Authors

* Robin Schuffenecker (Python AI)
* Evann Bloutin (C++ GUI)
* Titouan Bouillot (C++ GUI)
* Paul-Antoine Salmon (C server)
* Santiago Pidcova (C server)

---
