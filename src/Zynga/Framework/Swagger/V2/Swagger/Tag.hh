<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use Zynga\Framework\Type\V1\StringBox;

class Tag extends StorableObjectBase {
  public StringBox $name;
  public StringBox $description;

  public function __construct() {

    parent::__construct();

    $this->name = new StringBox();
    $this->name->setIsRequired(true);

    $this->description = new StringBox();

  }

}
