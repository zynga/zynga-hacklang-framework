<?hh //strict

namespace Zynga\Framework\Service\V1\Test;

use Zynga\Framework\Service\V1\Test\ValidNoFields;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ValidNoFieldsTest extends TestCase {

  public function testResponse(): void {
    $validNoFields = new ValidNoFields();
    $response = $validNoFields->response();
    $this->assertNotNull($response);
  }

  public function testHandle(): void {
    $validNoFields = new ValidNoFields();
    $response = $validNoFields->handle();
    $this->assertTrue($response);
  }
}
