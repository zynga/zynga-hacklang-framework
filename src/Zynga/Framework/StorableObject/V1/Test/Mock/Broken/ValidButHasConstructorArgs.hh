<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\Broken;

use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;

class ValidButHasConstructorArgs extends Valid {
  public function __construct(string $myPrettyArg) {

    parent::__construct();

    // --
    // kaboooomski! as the storable objects as a whole don't typically have args,
    // therefor it will explode when fed to some of the systems that expect to
    // allocate storable objects with no args.
    // --

  }
}
