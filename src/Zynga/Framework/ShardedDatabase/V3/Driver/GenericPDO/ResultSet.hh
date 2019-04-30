<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\ShardedDatabase\V3\Driver\ResultSet\Base;

use Zynga\Framework\Database\V2\Exceptions\NoActiveCursorException;
use Zynga\Framework\Database\V2\Exceptions\OutOfBoundsForCursorException;

use \PDO;
use \PDOStatement;

class ResultSet extends Base {
  private string $_sql;
  private int $_rows;
  private int $_currentPosition;
  private ?PDOStatement $_rs;

  /**
   * Creates a wrapper for a PDO
   * @param resource $rs
   * @return ResultSet
   */
  public function __construct(string $sql, ?PDOStatement $rs) {

    parent::__construct();

    $this->_rows = -1;
    $this->_currentPosition = -1;
    $this->_sql = $sql;
    $this->_rs = $rs;
  }

  public function wasSuccessful(): bool {
    if ($this->hasCursor()) {
      return true;
    }
    return false;
  }

  public function hasCursor(): bool {
    if ($this->_rs !== null) {
      return true;
    }
    return false;
  }

  public function freeCursor(): bool {

    if ($this->hasCursor() === true && $this->_rs !== null) {
      $this->_rs = null;
      $this->_rows = -1;
      $this->_currentPosition = -1;
      return true;
    }
    return false;
  }

  public function getNumRows(): int {
    try {
      if ($this->_rows != -1) {
        return $this->_rows;
      }
      if ($this->_rs === null) {
        throw new NoActiveCursorException(
          'getNumRows requires a active resultset sql='.$this->_sql,
        );
      }
      $this->_rows = $this->_rs->rowCount();
      return $this->_rows;
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function rewind(int $pos): bool {
    try {

      if ($this->_rs === null) {
        throw new NoActiveCursorException(
          'getNumRows requires a active resultset sql='.$this->_sql,
        );
      }

      $rs = $this->_rs;

      $numRows = $this->getNumRows();

      if ($pos > $numRows || $pos < 0) {
        throw new OutOfBoundsForCursorException('OOB pos='.$pos);
      }

      // tell the sth to go back to first position
      $rs->execute();

      // reset our position to the appropriate value.
      $pos = $pos - 1;

      $this->_currentPosition = $pos;

      return true;
    } catch (Exception $e) {
      throw $e;
    }

  }

  public function hasMore(): bool {
    try {

      // load up the number of rows for this query.
      $rows = $this->getNumRows();

      // if the row count isn't invalid, we should be below or equal to the row count.
      if ($rows !== -1 && ($this->_currentPosition + 1) < $rows) {
        return true;
      }

    } catch (Exception $e) {
      throw $e;
    }

    return false;
  }

  public function next(): bool {
    // do we have more to give? move the cursor position.
    try {

      if ($this->_rs === null) {
        throw new NoActiveCursorException(
          'next() requires a active resultset sql='.$this->_sql,
        );
      }

      $rs = $this->_rs;

      if ($this->hasMore() === true) {

        $this->_currentPosition++;

        return true;
      }

    } catch (Exception $e) {
      throw $e;
    }
    return false;
  }

  public function fetchMap(): Map<string, mixed> {

    try {

      if ($this->_rs === null) {
        throw new NoActiveCursorException(
          'fetchMap requires a active resultset sql='.$this->_sql,
        );
      }

      $rs = $this->_rs;

      $rawData = $rs->fetch(
        PDO::FETCH_ASSOC,
        PDO::FETCH_ORI_ABS,
        $this->_currentPosition,
      );

      $data = Map {};

      if (is_array($rawData)) {
        foreach ($rawData as $key => $value) {
          $data->set($key, $value);
        }
      } else {
        // We had a error of some sort if we didn't return an array.
      }

      return $data;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function fetchVector(): Vector<int> {

    try {

      if ($this->_rs === null) {
        throw new NoActiveCursorException(
          'next() requires a active resultset sql='.$this->_sql,
        );
      }

      $rawData =
        $this->_rs->fetch(
          PDO::FETCH_NUM,
          PDO::FETCH_ORI_ABS,
          $this->_currentPosition,
        );

      $data = Vector {};

      foreach ($rawData as $value) {
        $data->add($value);
      }

      return $data;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function setSql(string $sql): bool {
    $this->_sql = $sql;
    return true;
  }

  public function getSql(): string {
    return $this->_sql;
  }

  public function wasSqlDML(): bool {
    if (preg_match('/\s*(INSERT|UPDATE|DELETE)/i', $this->_sql)) {
      return true;
    }
    return false;
  }

}
