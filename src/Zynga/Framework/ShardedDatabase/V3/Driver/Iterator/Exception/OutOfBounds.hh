<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\Base as IteratorException;

/**
 * next() was called on an iterator when there were no more drivers to iterate over
 */
class OutOfBounds extends IteratorException {}
