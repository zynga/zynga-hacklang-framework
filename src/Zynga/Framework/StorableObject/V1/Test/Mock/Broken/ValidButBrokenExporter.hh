<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\Broken;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;
use Zynga\Framework\StorableObject\V1\Test\Mock\Broken\Exporter;

class ValidButBrokenExporter extends Valid {

  public function export(): ExportInterface {
    $exporter = new Exporter($this);
    return $exporter;
  }

}
