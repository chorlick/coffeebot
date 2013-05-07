#!/bin/bash

g++ ./main.cpp Bot.cpp `pkg-config --libs gloox` `pkg-config --cflags gloox`  `pkg-config --libs libconfig++ `\
                `pkg-config --cflags libconfig++ ` -L/usr/local/lib -o coffeebot -g 

g++ ./writer.cpp `pkg-config --libs gloox` `pkg-config --cflags gloox`  `pkg-config --libs libconfig++ `\
                `pkg-config --cflags libconfig++ ` -L/usr/local/lib -o writer -g 



