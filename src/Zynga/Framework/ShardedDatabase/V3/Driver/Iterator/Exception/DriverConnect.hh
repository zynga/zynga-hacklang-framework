<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\Base as IteratorException;

/**
 * There was a failure connecting to a driver
 */
class DriverConnect extends IteratorException {}
