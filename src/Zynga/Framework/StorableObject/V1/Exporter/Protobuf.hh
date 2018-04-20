<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Exporter;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\StorableObject\V1\Exporter\Protobuf\WireType;
use Zynga\Framework\StorableObject\V1\Exporter\ProtobufBinary;
use Zynga\Framework\StorableObject\V1\Fields;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\FloatBox;
use Zynga\Framework\Type\V1\Int32Box;
use Zynga\Framework\Type\V1\Int64Box;
use Zynga\Framework\Type\V1\SInt32Box;
use Zynga\Framework\Type\V1\SInt64Box;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt16Box;
use Zynga\Framework\Type\V1\UInt32Box;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Type\V1\UInt8Box;

class Protobuf {

  public function asBinary(StorableObjectInterface $object): string {

    try {

      $binConverter = new ProtobufBinary();

      $fields = $object->fields()->getForObject();

      if ($fields->count() == 0) {
        throw new NoFieldsFoundException('class='.get_class($object));
      }

      $fieldAndTypes = $object->fields()->getFieldsAndTypesForObject();

      $tag = 0;

      foreach ($fields as $fieldName => $field) {

        $type = $fieldAndTypes[$fieldName];

        // --
        // TODO: This should be per field idx not just us forcing it based upon
        // the field order due to reflection.
        // --
        $tag++;

        if ($field instanceof TypeInterface) {
          $value = $field->get();
        } else {
          throw new UnsupportedTypeException('type='.get_class($field));
        }

        $this->encodeType($binConverter, $tag, $type, $value);

      }

      $buffer = $binConverter->getBuffer();

      //$this->debugMessage("returned buffer=" . bin2hex($buffer));

      return $buffer;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function getWireTypeForZyngaType(string $fieldType): int {
    switch ($fieldType) {
      case Int32Box::class:
      case Int64Box::class:
      case UInt32Box::class:
      case UInt64Box::class:
      case BoolBox::class:
        //case 'EnumBox':
        return WireType::VARINT;
      case SInt32Box::class:
      case SInt64Box::class:
        return WireType::VARINT;
    }

    throw new UnsupportedTypeException('unsupported type='.$fieldType);

    /*
     $map = array(
     Protobuf::TYPE_FIXED64 => self::WIRE_FIXED64,
     Protobuf::TYPE_SFIXED64 => self::WIRE_FIXED64,
     Protobuf::TYPE_DOUBLE => self::WIRE_FIXED64,
     Protobuf::TYPE_STRING => self::WIRE_LENGTH,
     Protobuf::TYPE_BYTES => self::WIRE_LENGTH,
     Protobuf::TYPE_MESSAGE => self::WIRE_LENGTH,
     Protobuf::TYPE_FIXED32 => self::WIRE_FIXED32,
     Protobuf::TYPE_SFIXED32 => self::WIRE_FIXED32,
     Protobuf::TYPE_FLOAT => self::WIRE_FIXED32
     );
     */
  }

  public function encodeType(
    ProtobufBinary $binConverter,
    int $tag,
    string $type,
    mixed $value,
  ): bool {

    try {

      $wireType = $this->getWireTypeForZyngaType($type);

      // convert booleans into ints for varints
      if ($wireType == WireType::VARINT && is_bool($value)) {
        if ($value === true) {
          $value = 1;
        } else {
          $value = 0;
        }
      }

      // --
      // The java encoder has a nill value optimization where it doesn't encode
      // the nill value
      // --
      if ($wireType == WireType::VARINT && is_int($value) && $value == 0) {
        return true;
      }

      // add the key to the stack.
      $key = $tag << 3 | $wireType;

      // $this->debugMessage("KEY addvarint=" . sprintf('%d', $key));

      $binConverter->addVarint($key);

      if ($this->classExtends($type, SInt32Box::class) && is_int($value)) {
        //$this->debugMessage("VALUE addZigzag=" . sprintf('%d', $value));
        return $binConverter->addZigzag($value, 32);
      } else if ($this->classExtends($type, SInt64Box::class) &&
                 is_int($value)) {
        //$this->debugMessage("VALUE addZigzag=" . sprintf('%d', $value));
        return $binConverter->addZigzag($value, 64);
      } else if ($wireType === WireType::VARINT && is_int($value)) {
        // $this->debugMessage("VALUE addvarint=" . sprintf('%d', $value));
        return $binConverter->addVarint($value);
      }

      throw new UnsupportedTypeException('type='.$type);

    } catch (Exception $e) {
      throw $e;
    }

  }

  private function classExtends(string $class, string $extends): bool {
    return is_subclass_of($class, $extends) || $class === $extends;
  }

  /*
   public function debugMessage(string $message): bool {
   echo '[DEBUG] - ' . $message . "\n";
   return true;
   }
   */

}
