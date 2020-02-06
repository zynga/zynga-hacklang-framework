#!/bin/bash

# Recreate the testing schema.
cat /var/source/tests/sql/postgresql/create_test_database.sql | psql --user=zframework --host=localhost phpunit

echo '\d' | psql --user=zframework --host=localhost phpunit
