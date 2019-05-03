<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Mock;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Base as BaseDriverIterator;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class FailedConnect<TType as TypeInterface> extends BaseDriverIterator<TType> {
    public function connectToShard(): bool {
        return false;
    }
}
