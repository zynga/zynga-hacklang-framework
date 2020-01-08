<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;

use Zynga\Framework\Swagger\V2\Swagger\Operation as SwaggerOperation;

use Zynga\Framework\Type\V1\StringBox;

class Path extends StorableObjectBase {
  public SwaggerOperation $get;
  public SwaggerOperation $post;

  public function __construct() {

    parent::__construct();

    $this->get = new SwaggerOperation();
    $this->post = new SwaggerOperation();

  }

}
