<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Interfaces;

use
  Zynga\Framework\Factory\V2\Interfaces\DriverInterface as FactoryDriverInterface
;

use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface
;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\ResultSetInterface
;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface
;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;

interface DriverInterface extends FactoryDriverInterface, QueryableInterface {

  /**
   * Developer productivity optimization, we allow you to set a default uid and
   * snid to be passed to queries, and the quoting interfaces.
   * [Only used on drivers that are sharded by sn]
   *
   * @param SnidBox $sn
   * @param UidBox $uid
   * @return bool
   */
  public function setSnUid(SnidBox $sn, UidBox $uid): bool;

  /**
   * Return the current default sn/uid for this driver.
   * [Only used on drivers that are sharded by sn]
   *
   * @return tuple(SnidBox, UidBox) sn, uid
   */
  public function getSnUid(): (SnidBox, UidBox);

  public function getConfig(): DriverConfigInterface;

  public function connect(): bool;

  public function connectToShard(
    int $shardIndex,
    string $connectionString,
  ): bool;

  public function disconnect(): bool;

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
