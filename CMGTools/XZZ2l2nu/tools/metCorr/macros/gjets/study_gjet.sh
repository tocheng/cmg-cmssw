#!/bin/sh


g++ study_gjet.cc -o study_gjet.exe `root-config --cflags` `root-config --libs`


echo -- Command: ./study_gjet.exe 
./study_gjet.exe

