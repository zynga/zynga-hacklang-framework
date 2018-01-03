<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenExporter
;

use Zynga\Framework\Type\V1\StringBox;

class InvalidNested extends StorableObject {
  public StringBox $otherVar;
  public ValidButBrokenExporter $brokenExporterObj;

  public function __construct() {
    parent::__construct();

    $this->otherVar = new StringBox();

    $this->brokenExporterObj = new ValidButBrokenExporter();

  }

}
