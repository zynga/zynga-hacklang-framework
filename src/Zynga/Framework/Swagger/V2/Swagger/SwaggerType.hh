<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Swagger\V2\Swagger\SwaggerType\Base as SwaggerTypeBase;
use Zynga\Framework\Swagger\V2\Swagger\Item as SwaggerItem;
use Zynga\Framework\Swagger\V2\Swagger\Schema as SwaggerSchema;
use Zynga\Framework\Swagger\V2\Exceptions\UnknownZyngaTypeException;
use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;

use Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;

use Zynga\Framework\Swagger\V2\Swagger\DefinitionReference;

class SwaggerType extends SwaggerTypeBase {

  public SwaggerItem $items;
  public DefinitionReference $ref;

  private Swagger $_swagger;

  public function __construct(Swagger $obj) {

    parent::__construct();

    $this->_swagger = $obj;

    $this->items = new SwaggerItem();

    $this->ref = new DefinitionReference();

  }

  public function swagger(): Swagger {
    return $this->_swagger;
  }

  public function addSubObjectToSchema(string $objName): bool {

    try {

      $t_obj = DynamicClassCreation::createClassByName($objName, Vector {});

      // bring all the storable objects into the schema format.
      if ($t_obj instanceof StorableObjectInterface) {

        // setup a schema object for the child object.
        $obj = new SwaggerSchema($this->swagger());

        $obj->convertStorableToSchema($t_obj);

        // pull a reference name.
        $refName = $obj->getReferenceNameForStorable($t_obj);

        // if the definition isn't already there, add it.
        if ($this->swagger()->definitions->getMap()->containsKey($refName) !==
            true) {
          $this->swagger()->definitions->set($refName, $obj);
        }

        $this->ref->set($refName);

      }

      return true;

    } catch (UnableToFindClassException $e) {
      $wrappedException =
        new UnknownZyngaTypeException('UnableToFindClass type='.$objName);
      $wrappedException->setPrevious($e);
      throw $wrappedException;
    } catch (Exception $e) {
      throw $e;
    }

  }

  public function addSubTypeToSchema(string $subType): bool {

    $t_obj = DynamicClassCreation::createClassByName($subType, Vector {});

    // bring all the storable objects into the schema format.
    if ($t_obj instanceof StorableObjectInterface) {

      // setup a schema object for the child object.
      $obj = new SwaggerSchema($this->swagger());

      $obj->convertStorableToSchema($t_obj);

      // pull a reference name.
      $refName = $obj->getReferenceNameForStorable($t_obj);

      // if the definition isn't already there, add it.
      if ($this->swagger()->definitions->getMap()->containsKey($refName) !==
          true) {
        $this->swagger()->definitions->set($refName, $obj);
      }

      // add the reference into the ref value for the items array.
      $this->items->ref->set($refName);

      return true;

    }

    return false;

  }

  public function convertZyngaTypeToSwaggerType(string $type): bool {

    try {

      parent::convertZyngaTypeToSwaggerType($type);

      if ($this->type->get() === self::ARRAY_TYPE) {

        $pregs = array();

        if (preg_match('/\<(.*)\>/', $type, $pregs)) {
          $subType = $pregs[1];
          try {
            if (preg_match('/Box/', $subType)) {
              return parent::convertZyngaTypeToSwaggerType($subType);
            } else {
              return $this->addSubTypeToSchema($subType);
            }
          } catch (UnableToFindClassException $e) {
            throw new UnknownZyngaTypeException(
              'UnableToFindClass type='.$type,
            );
          } catch (Exception $e) {
            throw $e;
          }
        } else {
          throw new UnknownZyngaTypeException(
            'UnConvertableType type='.$type,
          );
        }

      }

      return true;

    } catch (UnknownZyngaTypeException $e) {
      if ($this->addSubObjectToSchema($type) === true) {
        return true;
      }
      // --
      // JEO: This is a failsafe throw, everything should be succesful due to test for
      // storable, causing it to get a UnknownZyngaType, so it's some form of a zynga
      // object we are working with.
      // --
      // @codeCoverageIgnoreStart
      throw $e;
      // @codeCoverageIgnoreEnd
    } catch (Exception $e) {
      throw $e;
    }

  }

}
