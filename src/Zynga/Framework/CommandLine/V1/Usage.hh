<?hh // strict

namespace Zynga\Framework\CommandLine\V1;

use Zynga\Framework\CommandLine\V1\Interfaces\ArgumentContainerInterface;

use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;

class Usage {

  public function getSectionSeperator(
    ArgumentContainerInterface $container,
    (function(string): void) $displayMethod,
  ): void {
    $sep = str_repeat('-', 80);
    $displayMethod($sep);
  }
  public function getHeader(
    ArgumentContainerInterface $container,
    (function(string): void) $displayMethod,
  ): void {
    $usageText = 'Usage: ';
    $displayMethod($usageText);
  }

  public function getCommandLine(
    ArgumentContainerInterface $container,
    (function(string): void) $displayMethod,
  ): void {

    $usageText = SuperGlobals::getScriptName();

    $arguments = $container->getArguments();

    // do a pass for required arguments
    foreach ($arguments as $argument) {
      $argumentName = $argument->getName();
      if ($argument->isRequired() === true) {
        $usageText .= ' '.$argument->getArgumentUsage();
      }
    }

    // do a pass for optional arguments
    foreach ($arguments as $argument) {
      $argumentName = $argument->getName();
      if ($argument->isRequired() === false) {
        $usageText .= ' ['.$argument->getArgumentUsage().']';
      }
    }

    $displayMethod($usageText);

  }

  public function getArgumentDescriptions(
    ArgumentContainerInterface $container,
    (function(string): void) $displayMethod,
  ): void {
    $usageTextHeader = "Arguments:";
    $displayMethod($usageTextHeader);

    $arguments = $container->getArguments();

    foreach ($arguments as $argument) {

      $usageText = '';

      $argumentName = $argument->getName();

      if ($argument->isRequired() === true) {
        $usageText .= sprintf(
          "%-30s: %s",
          $argument->getArgumentUsage(),
          $argument->getDescription(),
        );
      } else {
        $usageText .= sprintf(
          "%-30s: %s",
          '['.$argument->getArgumentUsage().']',
          $argument->getDescription(),
        );
      }

      if ($argument->hasValue() === true) {
        $usageText .= ' Value: '.strval($argument->get());
      } else if ($argument->isRequired() === true) {
        $usageText .= ' *MISSING*';
      }

      $displayMethod($usageText);

    }

  }

  public function createUsage(
    ArgumentContainerInterface $container,
    (function(string): void) $displayMethod,
  ): void {
    $this->getSectionSeperator($container, $displayMethod);
    $this->getHeader($container, $displayMethod);
    $this->getSectionSeperator($container, $displayMethod);
    $this->getCommandLine($container, $displayMethod);
    $this->getSectionSeperator($container, $displayMethod);
    $this->getArgumentDescriptions($container, $displayMethod);
    $this->getSectionSeperator($container, $displayMethod);
  }

}
