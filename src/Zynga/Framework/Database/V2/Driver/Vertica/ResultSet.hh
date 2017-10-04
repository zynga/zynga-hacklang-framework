<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\Vertica;

use Zynga\Framework\Database\V2\Driver\ResultSet\Base;
use Zynga\Framework\Database\V2\Exceptions\NoActiveCursorException;
use Zynga\Framework\Database\V2\Exceptions\OutOfBoundsForCursorException;
use Zynga\Framework\Exception\V1\Exception;

class ResultSet extends Base {
  const int PGSQL_ASS = 1;
  const int PGSQL_NUM = 2;

  private string $_sql;
  private int $_rows;
  private int $_currentPosition;
  private ?resource $_rs;

  /**
   * Creates a wrapper for a postgres
   * @param resource $rs
   * @return ResultSet
   */
  public function __construct(string $sql, resource $rs) {

    parent::__construct();

    $this->_rows = -1;
    $this->_currentPosition = -1;
    $this->_sql = $sql;
    $this->_rs = $rs;

  }

  public function __destruct() {
    $this->freeCursor();
  }

  public function wasSuccessful(): bool {
    if ($this->hasCursor() === true) {
      return true;
    }
    return false;
  }

  public function hasCursor(): bool {
    if (is_resource($this->_rs) && $this->_rs !== null) {
      return true;
    }
    return false;
  }

  public function freeCursor(): bool {
    if ($this->hasCursor() === true && $this->_rs !== null) {

      $rs = $this->_rs;

      $this->errorCapture()->start();

      pg_free_result($rs);

      $this->errorCapture()->stop();

      $this->_rs = null;
      $this->_currentPosition = -1;
      $this->_rows = -1;

      return true;
    }
    return false;
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

  public function getNumRows(): int {

    if ($this->_rs === null) {
      throw new NoActiveCursorException('NO_CURSOR');
    }

    if ($this->_rows !== -1) {
      return $this->_rows;
    }

    $rs = $this->_rs;

    // --
    // JEO: On vertica it seems inserts like pg_num_rows, it should be
    //   pg_affected_rows for DML. Leaving the code here jic we change this
    //   in the future.
    // --
    //if ( $this->wasSqlDML() === true ) {
    //  $this->_rows = pg_affected_rows($rs);
    //} else {
    //  $this->_rows = pg_num_rows($rs);
    //}
    //--

    $this->errorCapture()->start();

    $this->_rows = pg_num_rows($rs);

    $this->errorCapture()->stop();

    return $this->_rows;

  }

  public function rewind(int $pos): bool {

    try {

      if ($this->_rs === null) {
        throw new NoActiveCursorException('NO_CURSOR');
      }

      $rs = $this->_rs;

      $numRows = $this->getNumRows();

      if ($pos > $numRows || $pos < 0) {
        throw new OutOfBoundsForCursorException('OOB');
      }

      $this->_currentPosition = $pos - 1;

      $this->errorCapture()->start();

      $rv = pg_result_seek($rs, $pos);

      $this->errorCapture()->stop();

      return $rv;

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

    if ($this->_rs === null) {
      // we have no cursor to execute on
      throw new NoActiveCursorException('NO_CURSOR');
    }

    // do the heavy lifting with postgres
    $rs = $this->_rs;

    $this->errorCapture()->start();

    $data = pg_fetch_assoc($rs, $this->_currentPosition);

    $this->errorCapture()->stop();

    // now make a map to hand back to our caller.
    $row = Map {};

    if (is_array($data)) {

      foreach ($data as $key => $val) {
        $row->set($key, $val);
      }

    }

    return $row;

  }

  public function fetchVector(): Vector<int> {

    if ($this->_rs === null) {
      // we have no cursor to execute on
      throw new NoActiveCursorException('NO_CURSOR');
    }

    $rs = $this->_rs;

    $this->errorCapture()->start();

    $data = pg_fetch_array($rs, $this->_currentPosition, self::PGSQL_NUM);

    $this->errorCapture()->stop();

    $vec = Vector {};

    if (is_array($data)) {
      foreach ($data as $val) {
        $vec->add($val);
      }
    }

    return $vec;

  }

}
