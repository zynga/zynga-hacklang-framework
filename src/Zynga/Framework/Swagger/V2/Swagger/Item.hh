<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Swagger\V2\Swagger\SwaggerType\Base as SwaggerTypeBase;

use Zynga\Framework\Swagger\V2\Swagger\DefinitionReference;

// Should have the same compositite fields as swaggertypebase
class Item extends SwaggerTypeBase {
  public DefinitionReference $ref;

  public function __construct() {
    parent::__construct();
    $this->ref = new DefinitionReference();
  }

}
