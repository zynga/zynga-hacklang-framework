<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Mock;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Base as BaseDriverIterator;

class FailedDisconnect extends BaseDriverIterator {
    public function disconnectFromCurrentDriver(): bool {
        return false;
    }
}
