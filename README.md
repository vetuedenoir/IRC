# ft_irc :globe_with_meridians:

## Présentation du projet
ft_irc est un serveur IRC minimaliste développé en C++.
Il permet aux utilisateurs de créer des canaux de communication et de gérer divers aspects des interactions,
tels que les invitations, les connexions, et les messages privés.
Ce projet a été conçu pour approfondir mes connaissances en C++ et pour explorer
le fonctionnement des sockets et des communications réseau.
En bonus, un bot interactif a été ajouté, permettant aux utilisateurs de jouer
à un jeu de devinette en lui envoyant des messages privés.

## Objectifs pédagogiques
Le projet ft_irc a pour objectifs de :

Maîtriser la programmation en C++ dans le contexte de la création d'un serveur.
Comprendre et manipuler les sockets pour établir des communications réseau.
Approfondir la connaissance des protocoles de communication, notamment le protocole IRC.
Développer des compétences en gestion de l'état des utilisateurs et des canaux.

## Fonctionnement du programme
### Serveur
Le programme ircserv offre les fonctionnalités suivantes :

Création de canaux : Les utilisateurs peuvent créer et rejoindre des canaux pour discuter.
### Commandes gérées :
- invite : Inviter un utilisateur à un canal.
- join : Rejoindre un canal.
- kick : Expulser un utilisateur d'un canal.
- mode : Changer les modes des canaux ou des utilisateurs.
- nick : Changer le pseudo de l'utilisateur.
- part : Quitter un canal.
- pass : Gérer les mots de passe pour accéder à des canaux.
- privmsg : Envoyer des messages privés à d'autres utilisateurs.
- quit : Quitter le serveur.
- topic : Changer le sujet d'un canal.
- user : Gérer les informations des utilisateurs.

En plus de ces commandes, le serveur gere le casemapping et l envoie de fichier

### Client
Le client par défaut utilisé pour interagir avec le serveur est irssi, un client IRC populaire.
Les utilisateurs peuvent se connecter au serveur via ce client et utiliser les commandes disponibles.

### Bot interactif
Le bot est un programme externe qui se connecte au serveur a son lancement,
les utilisateurs peuvent interagir avec lui en envoyant des messages privés.
Le bot propose de jouer à un jeu de devinette (plus ou moins).
