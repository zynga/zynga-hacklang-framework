<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Mock;

use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class Quoter<TType as TypeInterface> implements QuoteInterface {
  private DriverInterface<TType> $_dbh;

  public function __construct(DriverInterface<TType> $driver) {
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
