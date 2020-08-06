<?hh //strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\PgModelFactory;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\InventoryModel;

class InventoryModelFactory extends PgModelFactory {
  protected static function getRealModel(): InventoryModel {
    return new InventoryModel();
  }
}
