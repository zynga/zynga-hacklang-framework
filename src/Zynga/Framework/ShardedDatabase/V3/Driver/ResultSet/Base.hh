<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\ResultSet;

use Zynga\Framework\Database\V2\Driver\ResultSet\Base as BaseResultSet;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\ResultSetInterface;

abstract class Base extends BaseResultSet implements ResultSetInterface {}
