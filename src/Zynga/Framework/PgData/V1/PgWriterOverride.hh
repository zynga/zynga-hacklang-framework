<?hh //strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\StorableObject\V1\Base;
use Zynga\Framework\Type\V1\BoolBox;

class PgWriterOverride extends Base {

  public BoolBox $override;

  public function __construct(): void {
    parent::__construct();
    $this->override = new BoolBox();
    $this->override->setDefaultValue(true);
  }
}
