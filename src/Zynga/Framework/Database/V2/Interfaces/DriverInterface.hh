<?hh // strict

namespace Zynga\Framework\Database\V2\Interfaces;

use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use
  Zynga\Framework\Factory\V2\Interfaces\DriverInterface as FactoryDriverInterface
;

interface DriverInterface extends FactoryDriverInterface, QueryableInterface {

  public function getRequiresMockQueries(): bool;
  public function enableRequireMockQueries(): bool;
  public function disableRequireMockQueries(): bool;

  public function getConfig(): DriverConfigInterface;

  public function setIsConnected(bool $state): bool;

  public function getIsConnected(): bool;

  public function isConnected(): bool;

  /**
   * Has the connection experienced a error
   *
   * @return bool
   * @access public
   */
  public function hadError(): bool;

  /**
   * Get the last error message string of a connection
   *
   * @return string Error message
   * @access public
   */
  public function getLastError(): string;

  /**
   * Provides the ability to quote values into sql safe strings.
   * @return QuoteInterface
   * @access public
   */
  public function quote(): QuoteInterface;

  /**
   * Provides the ability to quote values into sql safe strings.
   * @return QuoteInterface
   * @access public
   */
  public function getQuoter(): QuoteInterface;

  /**
   * Databases hand quoting values differently, we let the native driver
   * handle the work of quoting off values when possible.
   * @param string value
   * @return string
   */
  public function nativeQuoteString(string $value): string;

  /**
   * Provides the ability to engage with transactions if your database supports
   * them.
   * @return TransactionInterface
   */
  public function transaction(): TransactionInterface;

  /**
   * Provides the ability to engage with transactions if your database supports
   * them.
   * @return TransactionInterface
   */
  public function getTransaction(): TransactionInterface;

}
