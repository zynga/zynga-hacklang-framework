<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Mock;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Base as BaseDriverIterator;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class FailedDisconnect<TType as TypeInterface> extends BaseDriverIterator<TType> {
    public function disconnectFromCurrentDriver(): bool {
        return false;
    }
}
