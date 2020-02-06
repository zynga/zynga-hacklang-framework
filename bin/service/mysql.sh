#!/bin/bash

#Experiment to see if this resolves the docker mysql start issue
find /var/lib/mysql/mysql -exec touch -c -a {} \;

# dump out the schema as it stands
/etc/init.d/mysql start

RETRIES=15
until mysqladmin ping --silent > /dev/null 2>&1 || [ $RETRIES -eq 0 ]; do
  echo "Waiting for mysql server to start, $((RETRIES)) remaining attempts..."
  RETRIES=$((RETRIES-=1))
  sleep 1
done
