<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Swagger\V2\Swagger\ParameterInType;

class ParameterInTypeTest extends TestCase {

  public function testValid(): void {

    $obj = new ParameterInType();

    $validValues = array();
    $validValues[] = 'query';
    $validValues[] = 'header';
    $validValues[] = 'path';
    $validValues[] = 'formData';
    $validValues[] = 'body';

    foreach ($validValues as $validValue) {
      $this->assertTrue($obj->set($validValue));
    }

  }

  <<
  expectedException(
    "Zynga\Framework\Swagger\V2\Exceptions\UnSupportedTypeException",
  )
  >>
  public function testInvalid(): void {
    $obj = new ParameterInType();
    $obj->set('kansas');
  }

}
