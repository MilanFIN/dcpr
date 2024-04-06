#!/bin/bash
docker build -t gbai . 
docker create --name gbac gbai
docker cp gbac:/dcpr/dcpr.gba dcpr.gba                                                               
docker rm -f gbac
