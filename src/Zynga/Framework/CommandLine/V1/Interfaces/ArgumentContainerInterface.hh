<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Interfaces;

use Zynga\Framework\CommandLine\V1\Interfaces\ArgumentInterface;

interface ArgumentContainerInterface {
  public function getArguments(): Vector<ArgumentInterface>;
}
