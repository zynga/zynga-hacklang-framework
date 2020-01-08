#!/bin/bash

##
# JEO: Quick little script for standing up local webservers, I use this on
#   my laptop, feel free to improve upon it.
##
SOURCE="${BASH_SOURCE[0]}"

ROOT="$( cd "$(dirname $( dirname "${BASH_SOURCE[0]}" ))" >/dev/null && pwd )"

XHPROF_PORT=8181
XHPROF_DIR=$ROOT/third-party/xhprof/xhprof_html
XHPROF_LOG=$ROOT/tmp/hhvm_xhprof.log

PHPUNIT_PORT=8080
PHPUNIT_DIR=$ROOT/tmp/$USER-hh-phpunit-html
PHPUNIT_LOG=$ROOT/tmp/hhvm_phpunit.log

# stop any running servers
killall hhvm

# stand up a webserver for xhprof
if [ ! -d $XHPROF_DIR ]; then
  mkdir -p $XHPROF_DIR
fi

echo "starting xhprof webserver on port=$XHPROF_PORT dir=$XHPROF_DIR"

cd "${XHPROF_DIR}"

touch "${XHPROF_LOG}"

hhvm \
  -m daemon \
  -d hhvm.server.type=proxygen \
  -d hhvm.server.port=$XHPROF_PORT \
  -d hhvm.log.file=${XHPROF_LOG}

# stand up a webserver for our phpunit html output
if [ ! -d $PHPUNIT_DIR ]; then
  mkdir -p "${PHPUNIT_DIR}"
fi

touch "${PHPUNIT_LOG}"

echo "starting phpunit webserver on port=$PHPUNIT_PORT dir=${PHPUNIT_DIR}"
cd "${PHPUNIT_DIR}" 

hhvm \
  -m daemon \
  -d hhvm.server.type=proxygen \
  -d hhvm.server.port=${PHPUNIT_PORT} \
  -d hhvm.server.default_document=index.html \
  -d hhvm.log.file=${PHPUNIT_LOG}

ps -efa | grep hhvm
