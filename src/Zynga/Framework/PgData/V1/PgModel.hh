<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;

use Zynga\Framework\PgData\V1\Exceptions\PgRowInterfaceRequiredException;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\CacheInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DataInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DbInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\ReaderInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\StatsInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\WriterInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgResultSetInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;

// --
// Default implementations, all are bound to interfaces for overload (if needed).
// --
use Zynga\Framework\PgData\V1\PgModel\Cache;
use Zynga\Framework\PgData\V1\PgModel\Data;
use Zynga\Framework\PgData\V1\PgModel\Db;
use Zynga\Framework\PgData\V1\PgModel\Reader;
use Zynga\Framework\PgData\V1\PgModel\Stats;
use Zynga\Framework\PgData\V1\PgModel\Writer;
use Zynga\Framework\PgData\V1\PgResultSet;
use Zynga\Framework\PgData\V1\SqlGenerator;

use \Exception;

abstract class PgModel implements PgModelInterface {
  private ?CacheInterface $_cache = null;
  private ?DataInterface $_data = null;
  private ?DbInterface $_db = null;
  private ?StatsInterface $_stats = null;
  private ?ReaderInterface $_reader = null;
  private ?WriterInterface $_writer = null;

  public function createCacheObject(): CacheInterface {
    return $cache = new Cache($this);
  }

  final public function cache(): CacheInterface {

    $cache = $this->_cache;

    if ($cache instanceof CacheInterface) {
      return $cache;
    }

    $cache = $this->createCacheObject();

    $this->_cache = $cache;

    return $cache;

  }

  public function createDataObject(): DataInterface {
    return new Data($this);
  }

  final public function data(): DataInterface {

    $data = $this->_data;

    if ($data instanceof DataInterface) {
      return $data;
    }

    $data = $this->createDataObject();

    $this->_data = $data;

    return $data;

  }

  public function createDbObject(): DbInterface {
    return new Db($this);
  }

  final public function db(): DbInterface {

    $db = $this->_db;

    if ($db instanceof DbInterface) {
      return $db;
    }

    $db = $this->createDbObject();

    $this->_db = $db;

    return $db;

  }

  public function createReaderObject(): ReaderInterface {
    return new Reader($this);
  }

  final public function reader(): ReaderInterface {

    $reader = $this->_reader;

    if ($reader instanceof ReaderInterface) {
      return $reader;
    }

    $reader = $this->createReaderObject();

    $this->_reader = $reader;

    return $this->_reader;

  }

  public function createWriterObject(): WriterInterface {
    return new Writer($this);
  }

  public function writer(): WriterInterface {

    $writer = $this->_writer;

    if ($writer instanceof WriterInterface) {
      return $writer;
    }

    $writer = $this->createWriterObject();

    $this->_writer = $writer;

    return $writer;

  }

  public function stats(): StatsInterface {

    $stats = $this->_stats;

    if ($stats instanceof StatsInterface) {
      return $stats;
    }

    $stats = new Stats($this);

    $this->_stats = $stats;

    return $this->_stats;
  }

  public function add(PgRowInterface $row): bool {
    try {
      return $this->writer()->add($row);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function getByPk<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    mixed $id,
  ): ?PgRowInterface {

    try {
      return $this->reader()->getByPk($model, $id);
    } catch (Exception $e) {
      throw $e;
    }

  }

  // As this can return a result set this doesn't let you lock all the tiems within
  // a result set as the number of edge cases introduced by that logic is not wanted.
  public function get<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    ?PgWhereClauseInterface $where = null,
  ): PgResultSetInterface<PgRowInterface> {

    try {
      return $this->reader()->get($model, $where);
    } catch (Exception $e) {
      throw $e;
    }

  }

  abstract public function getDataCacheName(): string;

  abstract public function getReadDatabaseName(): string;

  abstract public function getWriteDatabaseName(): string;

}

