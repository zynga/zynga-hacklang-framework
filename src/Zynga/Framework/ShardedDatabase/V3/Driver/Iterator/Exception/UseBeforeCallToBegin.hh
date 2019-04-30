<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\Base as IteratorException;

/**
 * An operation was attempted on an iterator before having called begin()
 */
class UseBeforeCallToBegin extends IteratorException {}
