<?hh // strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Inventory\Sharded;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\PgRow\PkComesFromMemcache;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Type\V1\StringBox;

class ItemType extends PkComesFromMemcache {
  public UInt64Box $id;
  public UInt64Box $user_id;
  public UInt64Box $item_type_id;

  public function __construct(PgModelInterface $pgModel) {

    $this->id = new UInt64Box();
    $this->user_id = new UInt64Box();
    $this->item_type_id = new UInt64Box();

    parent::__construct($pgModel);

  }

  public function getTableName(): string {
    return 'user_item_type';
  }

  public function getPrimaryKey(): string {
    return 'id';
  }

  public function getShardedKey(): string {
    return 'user_id';
  }

}
