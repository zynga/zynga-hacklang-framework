<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\StorableObject\V1\Base as StorableObjectBase;
use Zynga\Framework\StorableObject\V1\StorableMap;
use Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Swagger\V2\Swagger;
use Zynga\Framework\Swagger\V2\Swagger\SwaggerType;
use Zynga\Framework\Swagger\V2\Swagger\DefinitionReference;

use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;

class Schema extends StorableObjectBase {
  public StringBox $type;
  public StorableVector<StringBox> $required;
  public StorableMap<SwaggerType> $properties;
  public DefinitionReference $ref;
  private Swagger $_swagger;

  public function __construct(Swagger $obj) {

    parent::__construct();

    $this->type = new StringBox();
    $this->required = new StorableVector(StringBox::class);
    $this->properties = new StorableMap();
    $this->ref = new DefinitionReference();

    $this->type->set('object');

    $this->_swagger = $obj;

  }

  public function swagger(): Swagger {
    return $this->_swagger;
  }

  public function getReferenceNameForStorable(
    StorableObjectInterface $object,
  ): string {
    $refName = get_class($object);
    $refName = str_replace('\\', '_', $refName);
    return $refName;
  }

  public function convertStorableToSchema(
    StorableObjectInterface $object,
  ): bool {

    $fieldsAndTypes = $object->fields()->getFieldsAndTypesForObject();
    $fields = $object->fields()->getForObject();

    foreach ($fieldsAndTypes as $fieldName => $type) {

      $typeObject = new SwaggerType($this->swagger());
      $typeObject->convertZyngaTypeToSwaggerType($type);

      $this->properties->set($fieldName, $typeObject);

    }

    foreach ($fields as $fieldName => $field) {

      list($isRequired, $isDefaultValue) =
        FieldsGeneric::getIsRequiredAndIsDefaultValue($field);

      if ($isRequired === true) {
        $f_box = new StringBox();
        $f_box->set($fieldName);
        $this->required->add($f_box);
      }

    }

    return true;

  }

}
