<?hh // strict

namespace Zynga\Framework\Database\V2\Interfaces;

use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;

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

}
