<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver;

use Zynga\Framework\Database\V2\Exceptions\MissingUserIdException;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface;
use Zynga\Framework\Environment\ErrorCapture\V1\Handler\Noop as ErrorCaptureNoop;
use Zynga\Framework\Environment\ErrorCapture\V1\Interfaces\ErrorCaptureInterface;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

abstract class Base implements DriverInterface {
  private DriverConfigInterface $_config;
  private ErrorCaptureInterface $_errorCapture;
  private SnidBox $_sn;
  private UidBox $_uid;

  public function __construct(DriverConfigInterface $config) {
    $this->_config = $config;
    $this->_errorCapture = new ErrorCaptureNoop();
    $this->_sn = new SnidBox();
    $this->_uid = new UidBox();
  }

  public function getConfig(): DriverConfigInterface {
    return $this->_config;
  }

  public function quote(): QuoteInterface {
    return $this->getQuoter();
  }

  public function transaction(): TransactionInterface {
    return $this->getTransaction();
  }

  public function hasValidSocialNetworkConditions(SnidBox $sn, UidBox $uid): bool {
    if ($uid->get() <= 0) {
      throw new MissingUserIdException('sn='.$this->_sn.' uid='.$this->_uid);
    }

    return true;
  }

  public function setSnUid(SnidBox $sn, UidBox $uid): bool {

    $this->hasValidSocialNetworkConditions($sn, $uid);

    $this->_sn = $sn;
    $this->_uid = $uid;

    return true;

  }

  public function getSnUid():(SnidBox, UidBox) {

    $this->hasValidSocialNetworkConditions($this->_sn, $this->_uid);

    return tuple(
      $this->_sn,
      $this->_uid
    );

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
