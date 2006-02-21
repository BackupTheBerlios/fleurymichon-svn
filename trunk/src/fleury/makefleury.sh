#!/bin/sh

gcc -lpthread -Wall -W -DFLEURY_DEBUG -I../shared fleury.c fleury_daemon.c fleury_socket.c fleury_thread.c fleury_irc.c fleury_load.c ../shared/gen_list.c -o fleury
