<?hh //strict

namespace Zynga\Framework\Service\V2\Client;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Service\V2\Client\Batch;
use Zynga\Framework\Service\V2\Client\Mock\Batch as MockBatch;
use Zynga\Framework\Service\V2\Client\Mock\BatchIsStartedBatch;
use Zynga\Framework\Service\V2\Client\Mock\RequestSentBatch;
use Zynga\Framework\Service\V2\Client\Mock\HasResponseBatch;
use Zynga\Framework\Service\V2\Response\Mock\Base as MockResponse;
use Zynga\Framework\Service\V2\Request\Mock\Base as MockRequest;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Service\V2\Test\Response;

class BatchTest extends TestCase {

  public function test_init(): void {
    $serviceUrl = 'http://localhost';
    $serviceResponseObject = Response::class;

    $obj = new Batch();
    $obj->setServiceResponseObject($serviceResponseObject);
    $obj->setServiceUrl($serviceUrl);
    $this->assertEquals($serviceUrl, $obj->getServiceUrl());
    $this->assertEquals(
      $serviceResponseObject,
      $obj->getServiceResponseObject(),
    );
    $this->assertEquals(0, $obj->getRequestId());
  }

  public function test_wasRequestSuccessful_invalidId(): void {
    $requestId = 239874;

    $serviceUrl = 'http://localhost';
    $serviceResponseObject = Response::class;

    $obj = new Batch();
    $obj->setServiceUrl($serviceUrl);
    $obj->setServiceResponseObject($serviceResponseObject);
    $this->assertFalse($obj->wasRequestSuccessful($requestId));
  }

  public function testWasRequestSuccessful(): void {
    $requestSentHasResponseBatch =
      new RequestSentBatch(new HasResponseBatch(new MockBatch()));
    $this->assertTrue($requestSentHasResponseBatch->wasRequestSuccessful(0));

    $requestSentBatch = new RequestSentBatch(new MockBatch());
    $this->assertFalse($requestSentBatch->wasRequestSuccessful(0));

    $hasResponseBatch = new HasResponseBatch(new MockBatch());
    $this->assertFalse($hasResponseBatch->wasRequestSuccessful(0));
  }

  public function testHasResponse(): void {
    $batch = new Batch();
    $this->assertFalse($batch->hasResponse(0));
  }

  public function testGetResponse(): void {
    $batch = new Batch();
    $batch->setResponse(0, new MockResponse(true, 200));
    $this->assertTrue($batch->getResponse(0)->success()->get());

    $exceptionBatch = new Batch();
    $this->expectException(Exception::class);
    $exceptionBatch->getResponse(0);
  }

  public function testAddRequest(): void {
    $batch = new BatchIsStartedBatch(new MockBatch());
    $this->expectException(Exception::class);
    $batch->addRequest(new MockRequest());
  }

  public function testExecute(): void {
    $batch = new BatchIsStartedBatch(new MockBatch());
    $this->expectException(Exception::class);
    $batch->execute();
  }

  public function testGetRequestIdsEmpty(): void {
    $batch = new MockBatch();
    $this->assertEmpty($batch->getRequestIds());
  }
}
