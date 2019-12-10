<?hh // strict

namespace Zynga\Framework\PgData\V1\Testing\Mock;

use Zynga\Framework\PgData\V1\Exceptions\UnsupportedOperandException;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\ReaderInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\WriterInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgResultSetInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;
use Zynga\Framework\PgData\V1\PgCachedResultSet;
use Zynga\Framework\PgData\V1\PgResultSet;
use Zynga\Framework\PgData\V1\PgWhereClause;
use Zynga\Framework\PgData\V1\PgWhereOperand;
use Zynga\Framework\PgData\V1\PgWhereOperand\PgPragma;
use Zynga\Framework\PgData\V1\PgWhereOperand\PgPragmaType;
use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UInt64Box;

/**
 * A implementation of PgData's Reader & Writer interfaces that keeps everything in memory for unit testing.
 */
class PgReaderWriter implements ReaderInterface, WriterInterface {

  public bool $addSucceeds = true;
  public bool $saveSucceeds = true;

  private StorableMap<StorableMap<PgRowInterface>> $tables;

  public function __construct() {
    $this->tables = new StorableMap(StorableMap::class);
  }

  public function getByPk<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    mixed $id,
    bool $getLocked,
  ): ?PgRowInterface {
    $table = $this->getTable($model);
    $key = (string) $id;
    return $table->get($key);
  }

  public function get<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    ?PgWhereClauseInterface $where = null,
  ): PgResultSetInterface<PgRowInterface> {
    $table = $this->getTable($model);
    $resultSet = new PgResultSet($model);
    foreach ($table->toArray() as $row) {
      if (self::matchesWhere($row, $where)) {
        $resultSet->add($row);
      }
    }
    return $resultSet;
  }

  public function createCachedResultSet<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $pgWhere,
  ): PgCachedResultSet<TypeInterface> {
    return new PgCachedResultSet(UInt64Box::class, $pgWhere, $model);
  }

  public function getTable(string $tableName): StorableMap<PgRowInterface> {
    $table = $this->tables->get($tableName);
    if ($table === null) {
      $table = new StorableMap(PgRowInterface::class);
      $this->tables->set($tableName, $table);
    }
    return $table;
  }

  public function add(PgRowInterface $row, bool $shouldUnlock = true): bool {
    if ($this->addSucceeds) {
      $table = $this->getTable(get_class($row));
      $key = $row->getPrimaryKeyTyped();

      // Set PK on new rows
      if ($key->get() === 0) {
        $key->set($table->count() + 1);
      }

      $key = (string) $key;
      $table->set($key, $row);
      return true;
    } else {
      return false;
    }
  }

  public function save(PgRowInterface $row, bool $shouldUnlock = true): bool {
    if ($this->saveSucceeds) {
      $table = $this->getTable(get_class($row));
      $key = (string) $row->getPrimaryKeyTyped();
      $table->set($key, $row);
      return true;
    } else {
      return false;
    }
  }

  public function delete(
    PgRowInterface $row,
    bool $shouldUnlock = true,
  ): bool {
    $table = $this->getTable(get_class($row));
    $key = (string) $row->getPrimaryKeyTyped();
    $table->remove($key);
    return true;
  }

  protected static function matchesWhere(
    PgRowInterface $row,
    ?PgWhereClauseInterface $where,
  ): bool {
    if ($where instanceof PgWhereClause) {
      $pragmas = $where->getPragmas();
      foreach ($pragmas as $pragma) {
        if ($pragma->getPragmaType() !== PgPragmaType::AND) {
          throw new UnsupportedOperandException(
            "Unsupported pragma type. Not yet implemented!",
          );
        }
        if (self::matchesPragma($row, $pragma) === false) {
          return false;
        }
      }
      return true;
    } else {
      return true;
    }
  }

  protected static function matchesPragma(
    PgRowInterface $row,
    PgPragma $pragma,
  ): bool {
    $field = $row->fields()->getTypedField($pragma->getField());
    switch ($pragma->getOperand()) {
      case PgWhereOperand::EQUALS:
        return $field->get() === $pragma->getValue();
      case PgWhereOperand::NOT_EQUALS:
        return $field->get() !== $pragma->getValue();
      case PgWhereOperand::GREATER_THAN:
        return $field->get() > $pragma->getValue();
      case PgWhereOperand::LESS_THAN:
        return $field->get() < $pragma->getValue();
      default:
        throw new UnsupportedOperandException(
          "Unsupported operand. Not yet implemented!",
        );
    }
  }
}
