<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\StringBoxTest;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

class EmailBoxTest extends StringBoxTest {

  /**
   * @expectedException Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException
   */
  public function testCannotImportFromInt(): void {
    $emailBox = new EmailBox();
    $emailBox->set(1234);
  }

  /**
   * @expectedException Zynga\Framework\Type\V1\Exceptions\FailedToImportFromBoolException
   */
  public function testCannotImportFromBool(): void {
    $emailBox = new EmailBox();
    $emailBox->set(true);
  }

  /**
   * @expectedException Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException
   */
  public function testCannotImportFromFloat(): void {
    $emailBox = new EmailBox();
    $emailBox->set(1.1234);
  }

  public function testCannotImportFromInvalidString(): void {

    $data = $this->getInvalidStrings();

    foreach ($data as $stringValue => $stringMessage) {
      try {
        $emailBox = new EmailBox();
        $emailBox->set($stringValue);
      } catch (FailedToImportFromStringException $e) {
        $this->assertTrue(true);
      }
    }

  }

  public function getInvalidStrings(): Map<string, string> {
    $data = Map {};
    $data->set('', 'Empty string is not an acceptable format');
    $data->set('12345', 'Numeric string is not an acceptable format');
    $data->set(
      'foo',
      'Username without the domain is not an acceptable format',
    );
    $data->set('@bar', 'Domain without username is not acceptable');
    $data->set('user@email', 'Malformed domain is not acceptable');
    return $data;
  }

  public function testValidEmailCanBeAdded(): void {
    $emailBox = new EmailBox();
    $this->assertTrue($emailBox->set('user@domain.com'));
  }
}
