<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\Mock;

use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;

class QueryableDriverThrowingQueryFailedException implements QueryableInterface {

  public function query(string $sql): ResultSetInterface {
    throw new QueryFailedException();
  }

}
