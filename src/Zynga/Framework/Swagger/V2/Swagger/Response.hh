<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Swagger\V2\Swagger;
use Zynga\Framework\Swagger\V2\Swagger\Schema as SwaggerSchema;

class Response extends StorableObjectBase {
  public StringBox $description;
  public SwaggerSchema $schema;

  private Swagger $_swagger;

  public function __construct(Swagger $obj) {

    parent::__construct();

    $this->_swagger = $obj;

    $this->description = new StringBox();
    $this->description->setIsRequired(true);

    $this->schema = new SwaggerSchema($this->_swagger);

  }

  public function swagger(): Swagger {
    return $this->_swagger;
  }

}
