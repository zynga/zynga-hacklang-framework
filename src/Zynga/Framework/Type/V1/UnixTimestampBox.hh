<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\UInt64Box;

use Zynga\Framework\Exception\V1\Exception;

class UnixTimestampBox extends UInt64Box {

  <<__Override>>
  protected function importFromString(string $value): bool {

    $pregs = null;

    if (preg_match(
          '/^(\d\d\d\d)(\d\d)(\d\d)\-(\d\d)(\d\d)(\d\d)$/',
          $value,
          $pregs,
        )) {

      // YYYYMMDD-HHMMSS format, break that down into a int timestamp.
      if (is_array($pregs)) {
        list($year, $mon, $day, $hour, $min, $sec) = $pregs;

        $utime = mktime($hour, $min, $sec, $mon, $day, $year);

        return $this->importFromInteger($utime);
      }

    }

    try {
      return parent::importFromString($value);
    } catch (Exception $e) {
      throw $e;
    }

  }

}
