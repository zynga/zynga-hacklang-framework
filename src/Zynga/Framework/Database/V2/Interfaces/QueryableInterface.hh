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
}
