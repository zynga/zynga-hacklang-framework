<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\Base as IteratorException;

/**
 * begin() was called more than once on an iterator
 */
class CalledBeginMultipleTimes extends IteratorException {}
