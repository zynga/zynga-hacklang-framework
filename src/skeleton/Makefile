default:
	@echo "--------------------------------------------------------------------------------"
	@echo "Development commands"
	@echo "--------------------------------------------------------------------------------"
	@echo " composer-install                     - Stands up composer and installs deps"
	@echo " composer-update                      - Stands up composer and updates deps"
	@echo " check-exceptions                     - Verifies the use of sub classed exceptions"
	@echo " check-strict                         - Checks that all files are within guidelines"
	@echo " hack-format                          - Reformats the hack code into facebook standard"
	@echo " hack-restart                         - Restarts the hhvm instance"
	@echo " hack-lint                            - Verifies the codebase is hack compliant"
	@echo " test                                 - Runs phpunit"
	@echo " test-raw                             - Runs phpunit.phar instead of our phpunit"
	@echo "--------------------------------------------------------------------------------"

composer-install:
	time composer install

composer-update:
	time composer update

hack-restart:
	time ./bin/hhvm-restart-wrapper

hack-lint:
	time hh_client --lint

hack-format:
	find ./include -type f -exec hh_format -i {} \;

check-strict:
	./bin/check-hack-strict

check-exceptions:
	ack 'use \\Exception' include/ | grep -v 'Test.hh'
	ack 'new Exception' include/ | grep -v 'Test.hh'

test: composer-install hack-restart hack-lint check-strict
	time ./bin/phpunit --testsuite tests

test-xdebug: composer-install hack-restart hack-lint check-strict
	time ./bin/phpunit-xdebug --testsuite regression
