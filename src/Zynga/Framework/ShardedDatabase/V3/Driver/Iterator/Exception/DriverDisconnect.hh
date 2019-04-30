<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\Base as IteratorException;

/**
 * There was a failure disconnecting from a driver
 */
class DriverDisconnect extends IteratorException {}
