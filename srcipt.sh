#!/bin/bash

# Adresse IP et port du serveur
SERVER_IP="127.0.0.1"
SERVER_PORT="8080"

# Message à envoyer
MESSAGE="Bonjour, serveur de numero 1!"

# Boucle infinie pour envoyer des messages en continu
while true; do
    # Envoi du message au serveur en utilisant nc
    echo "$MESSAGE" | nc -q 0 "$SERVER_IP" "$SERVER_PORT"
    
    # Attente d'une seconde avant d'envoyer le prochain message
done
