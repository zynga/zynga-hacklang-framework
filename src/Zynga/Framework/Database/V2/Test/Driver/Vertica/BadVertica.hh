<?hh // strict

namespace Zynga\Framework\Database\V2\Test\Driver\Vertica;

use Zynga\Framework\Database\V2\Driver\Vertica;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Database\V2\Test\Driver\Vertica\BadVerticaException;

class BadVertica extends Vertica {

  public function query(string $sql): ResultSetInterface {
    throw new BadVerticaException('The world has ended');
  }

  public function nativeQuoteString(string $value): string {
    throw new BadVerticaException('The quoting');
  }

}
