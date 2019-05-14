<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\Mock;

use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;

class QueryableDriverThrowingQueryFailedException
  implements QueryableInterface {

  public function connect(): bool {
    return false;
  }

  public function disconnect(): bool {
    return false;
  }

  public function query(string $sql): ResultSetInterface {
    throw new QueryFailedException();
  }

}
