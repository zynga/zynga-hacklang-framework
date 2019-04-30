<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Test\UserSharded\BadPDO;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\ResultSetInterface;

class Mock extends GenericPDO {

  public function query(string $sql): ResultSetInterface {
    throw new Exception('The world has ended');
  }

  public function nativeQuoteString(string $value): string {
    throw new Exception('The quoting');
  }

}
