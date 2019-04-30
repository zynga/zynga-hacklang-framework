<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Mock;

use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

class Quoter implements QuoteInterface {
  private DriverInterface $_dbh;

  public function __construct(DriverInterface $driver) {
    $this->_dbh = $driver;
  }

  public function intValue(int $value): string {
    return sprintf('%d', $value);
  }

  public function floatValue(float $value): string {
    return sprintf('%f', $value);
  }

  public function textValue(string $value): string {
    return $this->_dbh->nativeQuoteString($value);
  }

  public function textVector(
    Vector<string> $vec,
    SnidBox $sn = SnidBoxFactory::facebook(),
    UidBox $uid = new UidBox(0),
  ): string {
    $quotedString = '';
    foreach ($vec as $vecValue) {
      if ($quotedString != '') {
        $quotedString .= ', ';
      }
      if (is_string($vecValue)) {
        $quotedString .= "'$vecValue'";
      }
    }
    return $quotedString;
  }

}
