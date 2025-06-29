#!/bin/bash

# Test pour vérifier la gestion des broken pipes
echo "=== Test Gestion Broken Pipe ==="
echo ""

# Démarrer le serveur
echo "Démarrage serveur sur port 4244..."
./zappy_server -p 4244 -x 10 -y 10 -n team1 -c 6 -f 100 &
SERVER_PID=$!
sleep 1

echo "Test broken pipe - client se déconnecte brutalement..."

# Créer un client qui se connecte puis se déconnecte brutalement
{
    echo "team1"
    sleep 0.1
    # Simuler une déconnexion brutale (le pipe sera cassé)
} | timeout 2 nc localhost 4244 &
CLIENT_PID=$!

sleep 0.5

# Vérifier que le serveur est toujours en vie
if kill -0 $SERVER_PID 2>/dev/null; then
    echo "✓ Serveur toujours actif après déconnexion brutale"
else
    echo "✗ Serveur s'est arrêté (problème)"
fi

# Test avec plusieurs clients qui se déconnectent
echo ""
echo "Test avec déconnexions multiples..."
for i in {1..3}; do
    {
        echo "team1"
        sleep 0.1
    } | timeout 1 nc localhost 4244 &
done

sleep 2

# Vérifier l'état final du serveur
if kill -0 $SERVER_PID 2>/dev/null; then
    echo "✓ Serveur résistant aux déconnexions multiples"
else
    echo "✗ Serveur s'est arrêté avec déconnexions multiples"
fi

echo ""
echo "Arrêt du serveur..."
kill $SERVER_PID 2>/dev/null
sleep 1
pkill -f "zappy_server.*4244" 2>/dev/null

echo "Test terminé."
