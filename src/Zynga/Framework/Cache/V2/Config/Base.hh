<?hh // strict

namespace Zynga\Framework\Cache\V2\Config;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Factory\V2\Config\Base as FactoryBaseConfig;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

abstract class Base extends FactoryBaseConfig
  implements DriverConfigInterface {

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {

    // --
    // The end developer needs to setup the storable object such that the field they want
    // hashed are marked as 'required'
    // --
    $requiredFields = $obj->fields()->getRequiredFields();

    if ($requiredFields->count() == 0) {
      throw new InvalidObjectForKeyCreationException(
        'there are no required fields defined on this object='.
        get_class($obj),
      );
    }

    $requiredFieldsWithDefaultValues =
      $obj->fields()->getRequiredFieldsWithDefaultValues();

    if ($requiredFieldsWithDefaultValues->count() > 0) {
      throw new InvalidObjectForKeyCreationException(
        'you have required fields with default values'.
        ' obj='.
        get_class($obj).
        ' fields='.
        json_encode($requiredFieldsWithDefaultValues),
      );
    }

    $dataToHash = '';

    foreach ($requiredFields as $requiredField) {

      $field = $obj->fields()->getTypedField($requiredField);

      if ($dataToHash != '') {
        $dataToHash .= '-';
      }

      $dataToHash .= strval($field->get());

    }

    // now make the generic cache key.
    $key = 'zfc:'.md5(get_class($obj)).':'.md5($dataToHash);

    return $key;

  }

}
