<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Exception\V1\Exception;

/**
 * A GenericPDO specific implementation of QuoteInterface
 */
class Quoter implements QuoteInterface {
  private DriverInterface $driver;

  /**
   * Initializes this instance of Quoter.
   *
   * @param $driver Driver associated with this Quoter
   */
  public function __construct(DriverInterface $driver) {
    $this->driver = $driver;
  }

  /**
   * See @QuoteInterface
   */
  public function intValue(int $value): string {
    return sprintf('%d', $value);
  }

  /**
   * See @QuoteInterface
   */
  public function floatValue(float $value): string {
    return sprintf('%f', $value);
  }

  /**
   * See @QuoteInterface
   */
  public function textValue(string $value): string {
    return $this->driver->nativeQuoteString($value);
  }

  /**
   * See @QuoteInterface
   */
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
