<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Type\V1\UInt64Box;

use Zynga\Framework\Swagger\V2\Swagger\SwaggerType;
use Zynga\Framework\Swagger\V2\Swagger\ParameterInType;
use Zynga\Framework\Swagger\V2\Swagger;
use Zynga\Framework\Swagger\V2\Swagger\Schema;

class Parameter extends SwaggerType {

  public StringBox $name;
  public ParameterInType $in;
  public StringBox $description;
  public BoolBox $required;
  public Schema $schema;

  public function __construct(Swagger $obj) {

    parent::__construct($obj);

    $this->name = new StringBox();
    $this->name->setIsRequired(true);

    $this->in = new ParameterInType();
    $this->in->setIsRequired(true);

    $this->description = new StringBox();

    $this->required = new BoolBox();
    $this->required->setIsRequired(true);

    $this->schema = new Schema($obj);

  }

}
