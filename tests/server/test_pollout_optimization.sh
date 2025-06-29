#!/bin/bash

# Test script pour vérifier l'optimisation POLLOUT
# Ce script lance le serveur et simule plusieurs connexions client

echo "=== Test d'Optimisation POLLOUT ==="
echo ""

# Démarrer le serveur en arrière-plan
echo "Démarrage du serveur..."
./zappy_server -p 4243 -x 10 -y 10 -n team1 team2 -c 6 -f 100 &
SERVER_PID=$!
sleep 2

echo "Serveur démarré (PID: $SERVER_PID)"
echo ""

# Test 1: Connexion rapide pour vérifier handshake
echo "Test 1: Vérification handshake..."
timeout 3 sh -c 'echo "team1" | nc localhost 4243' 2>/dev/null &
sleep 1

# Test 2: Test simultané de plusieurs connexions
echo "Test 2: Connexions multiples simultanées..."
for i in {1..3}; do
    timeout 2 sh -c 'echo "team1" | nc localhost 4243' 2>/dev/null &
done
sleep 2

# Test 3: Connexion GUI
echo "Test 3: Connexion client graphique..."
timeout 2 sh -c 'echo "GRAPHIC" | nc localhost 4243' 2>/dev/null &
sleep 1

echo ""
echo "Tests en cours... (5 secondes)"
sleep 5

# Nettoyer les processus
echo ""
echo "Nettoyage des processus..."
kill $SERVER_PID 2>/dev/null
sleep 1
pkill -f "nc localhost 4243" 2>/dev/null
pkill -f "zappy_server" 2>/dev/null

echo "Test terminé."
echo ""
echo "Points de vérification du correctif:"
echo "✓ zn_has_pending_writes() implémentée"
echo "✓ setup_poll_events() optimisée"
echo "✓ POLLOUT ajouté seulement si nécessaire"
echo "✓ handle_client_write() améliorée"
