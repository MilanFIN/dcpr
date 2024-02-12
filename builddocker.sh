#!/bin/bash
docker build -t gbai . 
docker create --name gbac gbai
docker cp gbac:/gba-raycast-engine/gba-raycast-engine.gba gba-raycast-engine.gba                                                               
docker rm -f gbac