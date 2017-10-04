<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\Mock;

use Zynga\Framework\Database\V2\Driver\ResultSet\Base;

use Zynga\Framework\Database\V2\Exceptions\OutOfBoundsForCursorException;
use Zynga\Framework\Database\V2\Exceptions\Mock\ResultSetConfiguredWithNonMapException;
use
  Zynga\Framework\Database\V2\Exceptions\Mock\ResultSetConfiguredWithNonVectorException
;

use Zynga\Framework\Exception\V1\Exception;

class ResultSet extends Base {
  private string $_sql;
  private int $_resultOffset;
  private Vector<mixed> $_resultSet;
  private bool $_hasCursor;

  public function __construct(string $sql = '') {

    parent::__construct();

    $this->_sql = $sql;
    $this->_resultOffset = -1;
    $this->_resultSet = Vector {};
    $this->_hasCursor = true;

  }

  public function wasSuccessful(): bool {
    return true;
  }

  public function hasCursor(): bool {
    return $this->_hasCursor;
  }

  public function freeCursor(): bool {
    $this->_hasCursor = false;
    return true;
  }

  public function getNumRows(): int {
    return $this->_resultSet->count();
  }

  public function rewind(int $pos): bool {

    if (is_int($pos) && $this->_resultSet->containsKey($pos)) {
      $this->_resultOffset = $pos;
      return true;
    }

    return false;

  }

  public function hasMore(): bool {

    $offset = $this->getResultOffset() + 1;

    if (is_int($offset) && $this->_resultSet->containsKey($offset)) {
      return true;
    }

    return false;

  }

  public function next(): bool {

    $offset = $this->getResultOffset() + 1;

    if (is_int($offset) && $this->_resultSet->containsKey($offset)) {
      $this->_resultOffset = $offset;
      return true;
    }

    return false;
  }

  public function fetchMap(): Map<string, mixed> {

    $offset = $this->getResultOffset();

    if (is_int($offset) && $this->_resultSet->containsKey($offset)) {

      $data = $this->_resultSet[$offset];

      if ($data instanceof Map) {
        return $data;
      }

      throw new ResultSetConfiguredWithNonMapException(
        'offset='.$offset.' had='.get_class($data),
      );

    }

    throw new OutOfBoundsForCursorException('No more data to fetch');

  }

  public function fetchVector(): Vector<int> {

    $offset = $this->getResultOffset();

    if (is_int($offset) && $this->_resultSet->containsKey($offset)) {

      $data = $this->_resultSet[$offset];

      if ($data instanceof Vector) {
        return $data;
      }

      throw new ResultSetConfiguredWithNonVectorException(
        'offset='.$offset.' had='.get_class($data),
      );

    }

    throw new OutOfBoundsForCursorException('No more data to fetch');

  }

  public function setSql(string $sql): bool {
    $this->_sql = $sql;
    return true;
  }

  public function getSql(): string {
    return $this->_sql;
  }

  public function wasSqlDML(): bool {
    return false;
  }

  public function getResultOffset(): int {
    return $this->_resultOffset;
  }

  public function setResultData(Vector<mixed> $rsData): bool {
    $this->_resultSet = $rsData;
    return true;
  }

}
