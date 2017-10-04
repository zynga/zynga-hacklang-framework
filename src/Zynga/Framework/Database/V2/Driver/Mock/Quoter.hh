<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\Mock;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Type\V1\SnidBox;
use Zynga\Type\V1\UidBox;

class Quoter implements QuoteInterface {
  private DriverInterface $_dbh;

  public function __construct(DriverInterface $driver) {
    $this->_dbh = $driver;
  }

  public function intValue(int $value): string {
    return sprintf('%d', $value);
  }

  public function floatValue(float $value): string {
    return sprintf('%f', $value);
  }

  public function textValue(string $value): string {
    return $this->_dbh->nativeQuoteString($value);
  }

  public function textVector(Vector<string> $vec): string {
    $quotedString = '';
    foreach ($vec as $vecValue) {
      if ($quotedString != '') {
        $quotedString .= ', ';
      }
      if (is_string($vecValue)) {
        $quotedString .= "'$vecValue'";
      }
    }
    return $quotedString;
  }

}
