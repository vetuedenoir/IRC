#!/bin/bash

# Message à envoyer
MESSAGE="salut bien ou bien dans la zone 2222222222222222222222222 !"

# Boucle infinie pour envoyer le message en continu
while true; do
    # Envoi du message au serveur en utilisant echo
    echo "$MESSAGE"
    
    # Attente d'une seconde avant d'envoyer le prochain message
    sleep 1
done
