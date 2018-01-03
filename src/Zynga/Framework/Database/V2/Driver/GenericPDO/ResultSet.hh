<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO;

use \PDO;
use \PDOStatement;
use Zynga\Framework\Database\V2\Driver\ResultSet\Base;
use Zynga\Framework\Database\V2\Exceptions\NoActiveCursorException;
use Zynga\Framework\Database\V2\Exceptions\OutOfBoundsForCursorException;
use Zynga\Framework\Exception\V1\Exception;

/**
 * A GenericPDO specific implementation of ResultSet
 */
class ResultSet extends Base {
  private string $sql;
  private int $rows;
  private int $currentPosition;
  private ?PDOStatement $pdoStatement;

  /**
   * Creates a wrapper for a PDO
   *
   * @param $sql SQL statement of query
   * @param $pdoStatement PDOStatement from which to construct a result set
   */
  public function __construct(string $sql, ?PDOStatement $pdoStatement) {
    parent::__construct();
    $this->rows = -1;
    $this->currentPosition = -1;
    $this->sql = $sql;
    $this->pdoStatement = $pdoStatement;
  }

  /**
   * See @Base
   */
  public function wasSuccessful(): bool {
    return $this->hasCursor();
  }

  /**
   * See @Base
   */
  public function hasCursor(): bool {
    if ($this->pdoStatement !== null) {
      return true;
    }

    return false;
  }

  /**
   * See @Base
   */
  public function freeCursor(): bool {
    if ($this->hasCursor() === true && $this->pdoStatement !== null) {
      $this->pdoStatement = null;
      $this->rows = -1;
      $this->currentPosition = -1;
      return true;
    }

    return false;
  }

  /**
   * See @Base
   */
  public function getNumRows(): int {
    if ($this->rows !== -1) {
      return $this->rows;
    }

    if ($this->pdoStatement === null) {
      throw new NoActiveCursorException(
        'getNumRows requires an active resultset sql='.$this->sql,
      );
    }

    $this->rows = $this->pdoStatement->rowCount();
    return $this->rows;
  }

  /**
   * See @Base
   */
  public function rewind(int $pos): bool {
    if ($this->pdoStatement === null) {
      throw new NoActiveCursorException(
        'getNumRows requires an active resultset sql='.$this->sql,
      );
    }

    $pdoStatement = $this->pdoStatement;
    $numRows = $this->getNumRows();
    if ($pos > $numRows || $pos < 0) {
      throw new OutOfBoundsForCursorException('OOB pos='.$pos);
    }

    $pdoStatement->execute();
    $this->currentPosition = $pos - 1;
    return true;
  }

  /**
   * See @Base
   */
  public function hasMore(): bool {
    $rows = $this->getNumRows();

    // if the row count isn't invalid, we should be below or equal to the row count.
    return ($rows !== -1 && ($this->currentPosition + 1) < $rows);
  }

  /**
   * See @Base
   */
  public function next(): bool {
    if ($this->pdoStatement === null) {
      throw new NoActiveCursorException(
        'next() requires an active resultset sql='.$this->sql,
      );
    }

    $hasMore = $this->hasMore();
    $this->currentPosition += $hasMore ? 1 : 0;
    return $hasMore;
  }

  /**
   * See @Base
   */
  public function fetchMap(): Map<string, mixed> {
    if ($this->pdoStatement === null) {
      throw new NoActiveCursorException(
        'fetchMap requires an active resultset sql='.$this->sql,
      );
    }

    $pdoStatement = $this->pdoStatement;
    $rawData = $pdoStatement->fetch(
      PDO::FETCH_ASSOC,
      PDO::FETCH_ORI_ABS,
      $this->currentPosition,
    );

    $data = Map {};
    if (is_array($rawData)) {
      foreach ($rawData as $key => $value) {
        $data->set($key, $value);
      }
    }

    return $data;
  }

  /**
   * See @Base
   */
  public function fetchVector(): Vector<int> {
    if ($this->pdoStatement === null) {
      throw new NoActiveCursorException(
        'next() requires an active resultset sql='.$this->sql,
      );
    }

    $rawData =
      $this->pdoStatement
        ->fetch(PDO::FETCH_NUM, PDO::FETCH_ORI_ABS, $this->currentPosition);

    $data = Vector {};
    foreach ($rawData as $value) {
      $data->add($value);
    }

    return $data;
  }

  /**
   * See @Base
   */
  public function setSql(string $sql): bool {
    $this->sql = $sql;
    return true;
  }

  /**
   * See @Base
   */
  public function getSql(): string {
    return $this->sql;
  }

  /**
   * See @Base
   */
  public function wasSqlDML(): bool {
    return (preg_match('/\s*(INSERT|UPDATE|DELETE)/i', $this->sql) === 1);
  }
}
