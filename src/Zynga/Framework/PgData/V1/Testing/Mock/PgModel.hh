<?hh //strict

namespace Zynga\Framework\PgData\V1\Testing\Mock;

use Zynga\Framework\PgData\V1\Interfaces\PgModel\ReaderInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\StatsInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\WriterInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Testing\Mock\PgReaderWriter;
use Zynga\Framework\PgData\V1\PgModel as Base;

class PgModel extends Base {
  private PgReaderWriter $_readerWriter;

  public function __construct() {
    $this->_readerWriter = new PgReaderWriter();
  }

  public function getMockReaderWriter(): PgReaderWriter {
    return $this->_readerWriter;
  }

  public function getDataCacheName(): string {
    return 'Mock\PgData';
  }

  public function getResultSetCacheName(): string {
    return 'Mock\PgResultSet';
  }

  public function getReadDatabaseName(): string {
    return 'Mock';
  }

  public function getWriteDatabaseName(): string {
    return 'Mock';
  }

  public function createReaderObject(): ReaderInterface {
    return $this->_readerWriter;
  }

  public function createWriterObject(): WriterInterface {
    return $this->_readerWriter;
  }
}
