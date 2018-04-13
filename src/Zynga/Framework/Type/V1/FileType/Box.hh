<?hh // strict

namespace Zynga\Framework\Type\V1\FileType;

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\FileType\BoxFactory as FileTypeBoxFactory;
use Zynga\Framework\Type\V1\FileType\Exception\UnknownFileTypeException;

/**
 * Representation of a file type
 */
class Box extends StringBox {

  <<__Override>>
  protected function importFromString(string $value): bool {
      if (FileTypeBoxFactory::isValidValue($value)) {
        return parent::importFromString($value);
      }

      throw new UnknownFileTypeException('value='.$value);
  }

}
