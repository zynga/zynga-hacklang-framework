<?hh //strict

namespace Zynga\Framework\Service\V2;

use Zynga\Framework\Service\V2\Client;
use Zynga\Framework\Service\V2\Exceptions\InvalidServiceResponseException;
use Zynga\Framework\Service\V2\Test\Request;
use Zynga\Framework\Service\V2\Test\RequestNoRequired;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Response;

class ClientTest extends TestCase {

  public function test_serviceUrl(): void {
    $serviceUrl = 'http://localhost';
    $client = new Client();

    $this->assertTrue($client->setServiceUrl($serviceUrl));
    $this->assertEquals($serviceUrl, $client->getServiceUrl());
  }

  public function test_serviceResponseObject(): void {

    $serviceResponseObject = Response::class;
    $client = new Client();
    $this->assertTrue(
      $client->setServiceResponseObject($serviceResponseObject),
    );
    $this->assertEquals(
      $serviceResponseObject,
      $client->getServiceResponseObject(),
    );

  }

  public function test_batchLimit(): void {
    $client = new Client();
    $this->assertEquals(100, $client->getBatchLimit());
    $this->assertTrue($client->setBatchLimit(105));
    $this->assertEquals(105, $client->getBatchLimit());
  }

  public function test_addSingleRequest(): void {

    $serviceUrl = 'http://localhost';
    $serviceResponseObject = Response::class;

    $client = new Client();
    $client->setServiceUrl($serviceUrl);
    $client->setServiceResponseObject($serviceResponseObject);

    $obj = new RequestNoRequired();
    list($batchId, $requestId) = $client->addRequest($obj);
    $this->assertGreaterThan(0, $batchId);
    $this->assertGreaterThan(0, $requestId);
    $this->assertEquals(1, $client->getBatchCount());
    $this->assertEquals(1, $client->getRequestCount());

    $this->assertTrue($client->clear());
    $this->assertEquals(0, $client->getBatchCount());
    $this->assertEquals(1, $client->getRequestCount());

  }

  public function test_getExecutableBatches(): void {
    $client = new Client();
    $batches = $client->getExcutableBatches();
    $this->assertEquals(0, $batches->count());
  }

  <<
  expectedException(
    "Zynga\Framework\Service\V2\Exceptions\UnknownBatchException",
  )
  >>
  public function test_executeBatch_invalidBatch(): void {
    $client = new Client();
    $client->executeBatch(1);
  }

  <<
  expectedException(
    "Zynga\Framework\Service\V2\Exceptions\UnknownBatchException",
  )
  >>
  public function test_isBatchStillRunning_invalidBatch(): void {
    $client = new Client();
    $client->isBatchStillRunning(1);
  }

  public function testSetMock(): void {
    $client = new Client();
    $this->assertFalse($client->getEnableMock());
    $client->setEnableMock(true);
    $this->assertTrue($client->getEnableMock());
    $client->setEnableMock(false);
    $this->assertFalse($client->getEnableMock());
  }

  public function testGetBatchResultsMock(): void {
    $client = new Client();
    $client->setEnableMock(true);
    $this->assertTrue($client->getBatchResults(0));
  }

  public function testIsBatchStillRunningMock(): void {
    $client = new Client();
    $client->setEnableMock(true);
    $this->assertFalse($client->isBatchStillRunning(0));
  }

  public function testAddRequestWithBadResponseObjectThrowsException(): void {
    $client = new Client();
    $client->setEnableMock(true);
    $this->expectException(InvalidServiceResponseException::class);
    $client->addRequest(new Request());
  }

}
