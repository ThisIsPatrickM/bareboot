#! /bin/bash

echo "Creating new Key my.key"
openssl rand -out my.key 32
hexdump my.key 
