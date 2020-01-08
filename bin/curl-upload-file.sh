#!/bin/bash

##
# Setup the pipes such that if one fails the secondary item doesn't run.
##
set -o pipefail;

##
# Note on "$@" it keeps the quoting settings from the arguments passed.
##
curl -s -o /dev/null -w '%{http_code}' -T "$@" | awk {'print $1'}