<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\Vertica;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;

class Quoter implements QuoteInterface {
  private DriverInterface $_dbh;

  public function __construct(DriverInterface $driver) {
    $this->_dbh = $driver;
  }

  public function intValue(int $value): string {
    return sprintf('%d', $value);
  }

  public function boolValue(bool $value): string {
    return sprintf('%d', (int) $value);
  }

  public function floatValue(float $value): string {
    return sprintf('%f', $value);
  }

  public function textValue(string $value): string {
    return $this->_dbh->nativeQuoteString($value);
  }

  public function textVector(Vector<string> $vec): string {
    $value = '';
    foreach ($vec as $vecValue) {
      if ($value != '') {
        $value .= ', ';
      }
      $value .= $this->textValue($vecValue);
    }
    return $value;
  }
}
