<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;

use Zynga\Framework\Type\V1\StringBox;

class License extends StorableObjectBase {
  public StringBox $name;
  public StringBox $url;

  public function __construct() {

    parent::__construct();

    $this->name = new StringBox();
    $this->url = new StringBox();

  }

}
