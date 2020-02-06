#!/bin/bash

/etc/init.d/postgresql start

RETRIES=15

until psql --user=zframework --host=localhost -d phpunit -c 'select 1' > /dev/null 2>&1 || [ $RETRIES -eq 0 ]; do
  echo "Waiting for postgres server to start, $((RETRIES)) remaining attempts..."
  RETRIES=$((RETRIES-=1))
  sleep 1
done
