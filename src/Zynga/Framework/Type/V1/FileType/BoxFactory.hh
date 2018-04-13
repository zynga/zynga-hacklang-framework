<?hh // strict

namespace Zynga\Framework\Type\V1\FileType;

use Zynga\Framework\Type\V1\FileType\Box as FileTypeBox;
use Zynga\Framework\Type\V1\FileType\Enum\FileType as FileTypeEnum;

/**
 * Generates and validates file types
 */
class BoxFactory {

  public static function json(): FileTypeBox {
    $value = new FileTypeBox();
    $value->set(FileTypeEnum::JSON);
    return $value;
  }

  public static function csv(): FileTypeBox {
    $value = new FileTypeBox();
    $value->set(FileTypeEnum::CSV);
    return $value;
  }

  public static function isValidValue(string $value): bool {
    return FileTypeEnum::coerce($value) !== null;
  }

}
