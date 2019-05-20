<?hh // strict

namespace Zynga\Framework\Database\V2\Interfaces;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;

interface QuoteInterface {

  /**
   * Creates a database sepcific quoting object
   * @param DriverInterface $driver
   * @return QuoteInterface
   */
  //public function __construct(DriverInterface $driver);

  /**
   * Creates a sql query safe value out of a integer.
   * @param int $value
   * @return string
   */
  public function intValue(int $value): string;

  /**
   * Creates a sql query safe value out of a float
   * @param  float $value
   * @return string
   */
  public function floatValue(float $value): string;

  /**
   * Creates a sql query safe value out of a string.
   * @param  string $value
   * @return string
   */
  public function textValue(string $value): string;

  /**
   * Creates a comma deliminated string value ex: 'foo', 'bar', 'baz'
   * @param  Vector<string> $vec
   * @return string
   */
  public function textVector(Vector<string> $vec): string;

}
