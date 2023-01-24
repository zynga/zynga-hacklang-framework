**Please note:** This project is deprecated at Zynga and is no longer maintained.

---

# zynga-hacklang-framework

This is a collection of libraries that Zynga uses to enable hacklang strict usage.

# What is provided:

Namespace | Description | Documentation
--------- | ----------- | -------------
Zynga\Framework\AutoLoader | Simple autoloader | [AutoLoader](docs/Zynga_Framework_AutoLoader.md)
Zynga\Framework\Cache | StorableObject cache | [Cache](docs/Zynga_Framework_Cache.md)
Zynga\Framework\CommandLine | Commandline script framework handling argument parsing | [CommandLine](docs/Zynga_Framework_CommandLine.md)
Zynga\Framework\Database | Database abstraction | [Database](docs/Zynga_Framework_Database.md)
Zynga\Framework\Datadog | Datadog native hack implementation | [Datadog](docs/Zynga_Framework_Datadog.md)
Zynga\Framework\Dynamic | Encapsulation for dealing with dynamic class / method usage within a strict environment | [Dynamic](docs/Zynga_Framework_Dynamic.md)
Zynga\Framework\ELO | Implmentation of the ELO algorithim for various game use. | [ELO](docs/Zynga_Framework_ELO.md)
Zynga\Framework\Environment\CodePath | Class for finding the root of the codebase | [CodePath](docs/Zynga_Framework_Environment_CodePath.md)
Zynga\Framework\Environment\Command\Finder | Finder for binaries | [Command Finder](docs/Zynga_Framework_Environment_Command_Finder.md)
Zynga\Framework\Environment\DevelopmentMode | Class for finding which environment your code is running within | [DevelopmentMode](docs/Zynga_Framework_Environment_DevelopmentMode.md)
Zynga\Framework\Environment\ErrorCapture | Framework for capturing legacy php_notice messages. | [ErrorCapture](docs/Zynga_Framework_Environment_ErrorCapture.md)
Zynga\Framework\Environment\SuperGlobals | Class for abstracting away accessing the legacy php super global variables in a safe manner | [SuperGlobals](docs/Zynga_Framework_Environment_SuperGlobals.md)
Zynga\Framework\Exception | Exception class used throughout the codebase | [Exception](docs/Zynga_Framework_Exception.md)
Zynga\Framework\Factory | Factory pattern infrastructure | [Factory](docs/Zynga_Framework_Factory.md)
Zynga\Framework\Performance | Time and memory checkpoint timers | [Performance](docs/Zynga_Framework_Performance.md)
Zynga\Framework\StorableObject | Structures that implement import and export functionality | [StorableObject](docs/Zynga_Framework_StorableObject.md)
Zynga\Framework\Testing | Testing implementation that abstracts phpUnit in a strict compatible way. | [Testing](docs/Zynga_Framework_Testing.md)
Zynga\Framework\Type | Typeboxes for hacklang supporting data validation | [Types](docs/Zynga_Framework_Types.md)

# Setting up a development environment

0) Start with your dependencies for running the project locally.
0.0) mysql
0.1) postgresql
0.2) hhvm with postgresql enabled
0.3) composer

```bash
# using brew
brew install mysql

# ------
# HHVM 3.25 and above now includes postgre for Mac. If you install an older version, you can follow the steps below.
# More info on this ticket: https://github.com/zynga/zynga-hacklang-framework/issues/16
# Main advantage is that you can use the binaries rather than needing to build.
brew install postgresql
brew edit hhvm

# Manual work around for versions older than 3.25. You will be adding the following postgresql definitions to the brew recipie
## look for the following line
# -DLIBSODIUM_LIBRARIES=#{Formula["libsodium"].opt_lib}/libsodium.dylib
## Add the following
#-DENABLE_EXTENSION_PGSQL=ON
#-DPGSQL_INCLUDE_DIR=#{Formula["postgresql"].opt_include}
#-DPGSQL_LIBRARY=#{Formula["postgresql"].opt_lib}/libpq.dylib
brew reinstall --build-from-source hhvm
```

1) Setup composer for use locally

Download composer:

https://getcomposer.org/download/

Follow the above steps to get a composer.phar in your ~/bin directory.

Create a composer phar wrapper to handle running it via hhvm.

Edit ~/bin/composer

```php
#!/usr/local/bin/hhvm
<?hh

$command = array();
$command[] = '/usr/local/bin/hhvm';
$command[] = '-d hhvm.jit=false';
$command[] = dirname(__FILE__) . '/composer.phar';

$argvCount = count($argv);
for ( $i = 1; $i < $argvCount; $i++ ) {
  $command[] = $argv[$i];
}

// var_dump($command);

$commandString = join(' ', $command);

// var_dump($commandString);

$rv = 0;
passthru($commandString, $rv);

exit($rv);
```

Chmod the composer file to be executable:
```bash
chmod 755 ~/bin/composer
```

2) Run the setup process to get all the make files and symlinks available.
```bash
./bin/setup.sh
```
3) Setup the mysql environment needed for the test harness
```bash
brew services start mysql
mysql --user=root < tests/sql/mysql/create_test_database.sql
mysql --user=root -e 'SHOW DATABASES'
mysql --user=root -e 'SHOW TABLES' phpunit
```
4) Setup the postgresql environment needed for the test harness
```bash
brew services start postgresql
createdb `whoami`
psql < tests/sql/postgresql/create_test_database.sql
##
# caution the following commands will wipe out a existing .pgpass, so please
# be careful if you already have the need for this setting file.
##
echo "localhost:5432:phpunit:zframework:i-am-a-walrus" > ~/.pgpass
echo '\d' | psql --user=zframework --host=localhost phpunit
```
5) Setup memcache locally
```bash
brew install memcached # it should already be installed due to hhvm
brew services start memcached
```

6) Setup the environment you would like to emulate
```bash
# create the zynga directory on this host
sudo mkdir -p /opt/zynga
# create the environment indicator
sudo touch /opt/zynga/is_development
```

# Modifying code

## Adding new namespaces
In order for a new namespace to properly resolve to the correct file path, you have to provide the autoloader with a mapping from the namespace to the file path. To do this, you will need to:
1) Modify ./composer.json in the autoload/psr-4 section to contain your mapping from namespace to file path. These should be organized in alphabetical order.
2) Commit this change.
3) Run make composer-update.
4) Verify that your mapping appears in ./vendor/composer/autoload_psr4.php
5) Verify that your namespace is recognized and a class in said namespace can be loaded AT RUNTIME. Running hh_client isn't sufficient as the typechecker uses a different means of verifying classes/namespaces.

# Submitting code
1) Run make test
2) Ensure that code coverage is still at 100pct all submissions are required to keep us at that level.
3) make hack-format
4) submit pull request

# TODO
- [ ] Zynga\Exception - port short stack trace exporter
- [ ] Write doc/port internal docs: Code rules of the road
- [ ] Write doc/port internal docs: Design strategy
- [ ] Write doc/port internal docs: Zynga\Framework\AutoLoader
- [ ] Write doc/port internal docs: Zynga\Framework\Cache
- [ ] Write doc/port internal docs: Zynga\Framework\CommandLine
- [ ] Write doc/port internal docs: Zynga\Framework\Database
- [ ] Write doc/port internal docs: Zynga\Framework\Datadog
- [ ] Write doc/port internal docs: Zynga\Framework\Dynamic
- [ ] Write doc/port internal docs: Zynga\Framework\ELO
- [ ] Write doc/port internal docs: Zynga\Framework\Environment\CodePath
- [ ] Write doc/port internal docs: Zynga\Framework\Environment\Command\Finder
- [ ] Write doc/port internal docs: Zynga\Framework\Environment\DevelopmentMode
- [ ] Write doc/port internal docs: Zynga\Framework\Environment\ErrorCapture
- [ ] Write doc/port internal docs: Zynga\Framework\Environment\SuperGlobals
- [ ] Write doc/port internal docs: Zynga\Framework\Exception
- [ ] Write doc/port internal docs: ZYnga\Framework\Factory
- [ ] Write doc/port internal docs: Zynga\Framework\Performance
- [ ] Write doc/port internal docs: Zynga\Framework\StorableObject
- [ ] Write doc/port internal docs: Zynga\Framework\Testing
- [ ] Write doc/port internal docs: Zynga\Framework\Type
- [ ] Migrate the composer and project layout into something similar to what symfony is doing with multiple deps within a single master dep.
- [ ] Port StorableObject/V1/StorableMap to separate, new collection with added import support

# Naming Conventions

## Exceptions

Please postfix all exceptions with the 'Exception' name.

```
<?hh //strict

namespace My\Domain;

use Zynga\Framework\Exception\V1\Exception;

class MyDomainException extends Exception {}
```

# Build status

[![Build Status](https://travis-ci.org/zynga/zynga-hacklang-framework.svg?branch=master)](https://travis-ci.org/zynga/zynga-hacklang-framework)
