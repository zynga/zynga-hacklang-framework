<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Mock;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Base as BaseDriverIterator;

class FailedConnect extends BaseDriverIterator {
    public function connectToShard(): bool {
        return false;
    }
}
