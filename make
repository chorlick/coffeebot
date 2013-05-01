#!/bin/bash

g++ ./main.cpp Bot.cpp `pkg-config --libs gloox` `pkg-config --cflags gloox` -L/usr/local/lib -o coffeebot



