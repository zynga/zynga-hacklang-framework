<?hh // strict

namespace Zynga\Framework\Database\V2\Interfaces;

use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

interface QueryableInterface {

  /**
   * Running a sql query against the database.
   * @param string $sql
   * @return ResultSetInterface
   */
  public function query(string $sql): ResultSetInterface;

  /**
   * Attempts to connect to a given database type.
   * @return bool result of connection.
   */
  public function connect(): bool;

  /**
   * Attempts to disconnect the database connection.
   * @return bool result of disconnect attempt.
   */
  public function disconnect(): bool;

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

  /**
   * Changes the connection state for the driver.
   * @param bool connection state for the driver.
   * @return bool set success.
   */
  public function setIsConnected(bool $state): bool;

  /**
   * Drivers current connection state
   * @return bool connection state.
   */
  public function getIsConnected(): bool;

  /**
   * Drivers current connection state
   * @return bool connection state.
   */
  public function isConnected(): bool;

}
