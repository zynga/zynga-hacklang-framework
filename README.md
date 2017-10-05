# zynga-hacklang-framework

This is a collection of libraries that the Zynga uses to enable hacklang strict usage.

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
Zynga\Framework\Type | Typeboxes for hacklang supporting data validation | [Types](docs/Zynga_Framework_Type.md)

# Setting up a development environment
./bin/setup.sh

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
