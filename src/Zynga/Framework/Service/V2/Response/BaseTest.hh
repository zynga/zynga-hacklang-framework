<?hh //strict

namespace Zynga\Framework\Service\V2\Response;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Environment\V1\SuperGlobals;
use Zynga\Framework\Service\V2\Test\Response;

use Zynga\Framework\Type\V1\HttpResponseCodeBox;
use Zynga\Framework\Type\V1\StringBox;

class BaseTest extends TestCase {

  public function testValidResponse(): void {
    $obj = new Response();

    $this->assertTrue($obj->success()->set(true));
    $this->assertEquals(true, $obj->success()->get());

    // Response codes are http codes even if the proto isn't http.
    $this->assertTrue($obj->code()->set(HttpResponseCodeBox::HTTP_OK));
    $this->assertEquals(HttpResponseCodeBox::HTTP_OK, $obj->code()->get());

    $this->assertTrue($obj->code()->set(HttpResponseCodeBox::HTTP_FAILURE));
    $this->assertEquals(
      HttpResponseCodeBox::HTTP_FAILURE,
      $obj->code()->get(),
    );

    $value = 'this is a test of the emergency broadcast system';

    $failureMessage = new StringBox();
    $failureMessage->set($value);
    $result = $obj->message()->add($failureMessage);
    $retrivedValue = $obj->message()->at(0);
    if ($retrivedValue instanceof StringBox) {
      $this->assertEquals($value, $retrivedValue->get());
    }
  }

}
