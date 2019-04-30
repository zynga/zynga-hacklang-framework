<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Comparitor\Exception;

use Zynga\Framework\Exception\V1\Exception;

/**
 * An method was called on a ComparitorIterator where none should be called.
 */
class OperationOnComparitor extends Exception {}
