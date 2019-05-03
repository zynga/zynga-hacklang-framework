<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class Quoter<TType as TypeInterface> implements QuoteInterface<TType> {
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

    // use the mysql function
    try {
      return $this->_dbh->nativeQuoteString($value);
    } catch (Exception $e) {
      throw $e;
    }

  }

  public function textVector(
    Vector<string> $vec,
  ): string {
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
