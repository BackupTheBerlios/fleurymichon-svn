#!/bin/sh

gcc -lpthread -Wall -W -DFLEURY_DEBUG fleury.c fleury_daemon.c fleury_socket.c fleury_thread.c fleury_irc.c -o fleury
