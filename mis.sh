#!/bin/bash

SERVER="127.0.0.1"
PORT="8080"
NICK="TestBot"

exec 3<>/dev/tcp/$SERVER/$PORT
echo -e "NICK $NICK\r\nUSER $NICK $NICK $NICK :Test Bot\r\n" >&3
echo -e "PRIVMSG #test :Hello, World!\r\n" >&3
sleep 1
cat <&3

