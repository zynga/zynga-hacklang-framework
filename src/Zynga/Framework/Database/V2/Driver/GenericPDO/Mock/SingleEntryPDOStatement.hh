<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO\Mock;

use \PDO;
use \PDOStatement;

class SingleEntryPDOStatement extends PDOStatement {
    public function fetch (int $fetch_style = 0, int $cursor_orientation = PDO::FETCH_ORI_NEXT, int $cursor_offset = 0) : mixed {
        return array(1 => 2);
    }
}