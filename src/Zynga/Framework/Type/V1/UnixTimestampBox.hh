<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\UInt64Box;

use Zynga\Framework\Exception\V1\Exception;

class UnixTimestampBox extends UInt64Box {

  const int SECONDS_IN_DAY = 86400;

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

  /**
   * Computes the time span in days. The return value is always
   * floored instead of rounding to the Closest full day. i.e. if the span would
   * be 4.9999 the return value is 4.
   * @param null|UnixTimestampBox $endTime the timestamp for the end of the timespan.
   *   if null, will be set to the current system time.
   */
  public function timeSpanInDays(?UnixTimestampBox $endTime = null): int {
    $startTimestamp = $this->get();
    $endTimestamp = $endTime === null ? time() : $endTime->get();
    return (int) (($endTimestamp - $startTimestamp) / self::SECONDS_IN_DAY);
  }

}
