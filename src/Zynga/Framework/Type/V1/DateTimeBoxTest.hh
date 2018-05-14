<?hh //strict

namespace Zynga\Framework\Type\V1;

use \DateTime;
use Zynga\Framework\Type\V1\DateTimeBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromBoolException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;
use Zynga\Framework\Type\V1\StringBoxTest;

class DateTimeBoxTest extends StringBoxTest {

  public function testCannotImportFromInt(): void {
    $dateTimeBox = new DateTimeBox();
    $this->expectException(FailedToImportFromIntegerException::class);
    $dateTimeBox->set(1234);
  }

  public function testCannotImportFromBool(): void {
    $dateTimeBox = new DateTimeBox();
    $this->expectException(FailedToImportFromBoolException::class);
    $dateTimeBox->set(true);
  }

  public function testIsStringValid(): void {
    $dateTimeBox = new DateTimeBox();
    $this->assertTrue($dateTimeBox->isStringValid('1970'));
    $this->assertFalse($dateTimeBox->isStringValid('198.161.0.0'));
  }

  public function testCannotImportFromFloat(): void {
    $dateTimeBox = new DateTimeBox();
    $this->expectException(FailedToImportFromFloatException::class);
    $dateTimeBox->set(1.1234);
  }

  public function testCannotImportFromInvalidString(): void {
    $data = $this->getInvalidStrings();
    foreach ($data as $stringValue => $stringMessage) {
      // We can't use $this->expectException instead of this try/catch block
      // because the resulting assertion will only be made once rather than
      // for each instantiation of a DateTimeBox
      try {
        $dateTimeBox = new DateTimeBox();
        $dateTimeBox->set($stringValue);
        $this->assertTrue(false);
      } catch (FailedToImportFromStringException $e) {
        $this->assertTrue(true);
      }
    }

  }

  public function testCanImportFromValidString(): void {
    $data = $this->getValidStrings();
    foreach ($data as $stringValue) {
      try {
        $dateTimeBox = new DateTimeBox();
        $this->assertTrue($dateTimeBox->set($stringValue));
      } catch (FailedToImportFromStringException $e) {
        $this->assertTrue(false);
      }
    }
  }

  public function testToStringDefaultValueIsUnknownDateTime(): void {
    $dateTimeBox = new DateTimeBox();
    $this->assertEquals("Unknown DateTime", $dateTimeBox->__toString());
  }

  public function testResetSucceeds(): void {
    $dateTimeBox = new DateTimeBox();
    $this->assertTrue($dateTimeBox->reset());
  }

  public function testGetReturnsCorrectValue(): void {
    $dateTimeBox = new DateTimeBox();
    $dateTimeBox->set("2005-08-15T15:52:01+00:00");
    $expected = new DateTime("2005-08-15T15:52:01+00:00");
    $actual = $dateTimeBox->get();
    $this->assertEquals($expected, $actual);
  }

  private function getInvalidStrings(): Map<string, string> {
    $data = Map {};
    $data->set('12345', 'Pure numeric string is not an acceptable format');
    $data->set('foo', 'Pure alpha string is not an acceptable format');
    return $data;
  }

  private function getValidStrings(): Set<string> {
    $data = Set {};
    $data->add("2005-08-15T15:52:01+00:00"); // ATOM = "Y-m-d\TH:i:sP"
    $data->add("Monday, 15-Aug-2005 15:52:01 UTC"); // COOKIE = "l, d-M-Y H:i:s T"
    $data->add("2005-08-15T15:52:01+0000"); // ISO8601 = "Y-m-d\TH:i:sO"
    $data->add("Mon, 15 Aug 05 15:52:01 +0000"); // RFC822 = "D, d M y H:i:s O"
    $data->add("Monday, 15-Aug-05 15:52:01 UTC"); // RFC850 = "l, d-M-y H:i:s T"
    $data->add("Mon, 15 Aug 05 15:52:01 +0000"); // RFC1036 = "D, d M y H:i:s O"
    $data->add("Mon, 15 Aug 2005 15:52:01 +0000"); // RFC1123 = "D, d M Y H:i:s O"
    $data->add("Mon, 15 Aug 2005 15:52:01 +0000"); // RFC2822 = "D, d M Y H:i:s O"
    $data->add("2005-08-15T15:52:01+00:00"); // RFC3339 = "Y-m-d\TH:i:sP"
    $data->add("Mon, 15 Aug 2005 15:52:01 +0000"); // RSS = "D, d M Y H:i:s O"
    $data->add("2005-08-15T15:52:01+00:00"); // W3C = "Y-m-d\TH:i:sP"
    return $data;
  }
}
