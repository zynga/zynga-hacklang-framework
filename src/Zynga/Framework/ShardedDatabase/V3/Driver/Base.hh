<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver;

use Zynga\Framework\Database\V2\Exceptions\MissingUserIdException;
use Zynga\Framework\Environment\ErrorCapture\V1\Handler\Noop as ErrorCaptureNoop;
use Zynga\Framework\Environment\ErrorCapture\V1\Interfaces\ErrorCaptureInterface;
use Zynga\Framework\ShardedDatabase\V3\Exceptions\UnknownShardTypeException;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

abstract class Base<TType as TypeInterface> implements DriverInterface<TType> {
  private DriverConfigInterface<TType> $_config;
  private ErrorCaptureInterface $_errorCapture;
  private ?TType $_shardType;
  
  public function __construct(DriverConfigInterface<TType> $config) {
    $this->_config = $config;
    $this->_errorCapture = new ErrorCaptureNoop();
  }

  public function getConfig(): DriverConfigInterface<TType> {
    return $this->_config;
  }

  public function quote(): QuoteInterface<TType> {
    return $this->getQuoter();
  }

  public function transaction(): TransactionInterface<TType> {
    return $this->getTransaction();
  }
  
  public function setShardType(TType $shardType): bool {
    // Nothing to validate for TypeInterface
    $this->_shardType = $shardType;
    return true;
  }
  
  public function getShardType(): TType {
    if($this->_shardType === null) {
      throw new UnknownShardTypeException("Shard type is null");
    }
    
    return $this->_shardType;
  }
  
  public function onDriverConnectionChange(bool $from, bool $to): void {
   // NOOP - for the moment.
  }

  abstract public function setIsConnected(bool $state): bool;

  abstract public function getIsConnected(): bool;

  public function isConnected(): bool {
    return $this->getIsConnected();
  }

  public function isSqlDML(string $sql): bool {
    if (preg_match('/^(TRUNCATE|REPLACE|INSERT|UPDATE|DELETE)/i', $sql)) {
      return true;
    }
    return false;
  }
}
