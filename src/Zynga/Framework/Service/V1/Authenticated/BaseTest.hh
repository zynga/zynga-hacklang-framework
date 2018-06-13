<?hh //strict

namespace Zynga\Framework\Service\V1\Authenticated;

use Zynga\Poker\Framework\Database\UserSharded\V2\Factory as UserShardedDatabaseFactory;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Testing\MockState\V2\MockState;
use Zynga\Framework\Service\V1\Test\Authenticated\Valid as AuthenticatedService;
use Zynga\Framework\Service\V1\Test\Authenticated\WithParams\Valid as AuthenticatedServiceWithParams;

class BaseTest extends TestCase {

  // --
  // As this is authentication, it is considered core to our game and should
  // always be hitting the database infrastructure in order to make sure
  // we didn't break sql or something else in the stack.
  // --
  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    UserShardedDatabaseFactory::disableMockDrivers();
    UserShardedDatabaseFactory::clear();
    return true;
  }

  public function testFailure_NotAuthenticated(): void {
    $obj = new AuthenticatedService();
    $obj->request()->snid->set(1);
    $obj->request()->uid->set(1);
    $obj->request()->passcode->set('86e1c0bdeb106f4caefee25c0b8f101i');
    $obj->setHandlerFromString(AuthenticatedService::NOOP_HANDLER);
    $this->assertFalse($obj->handle());
  }

  public function testHelperFunctions(): void {
    $obj = new AuthenticatedService();
    $this->assertInstanceOf(
      \Zynga\Framework\Service\V1\Authenticated\Request\Importer::class,
      $obj->request()->import()
    );
    $this->assertInstanceOf(
      \Zynga\Framework\Type\V1\ChecksumBox::class,
      $obj->request()->checksum()
    );
    $this->assertInstanceOf(
      \Zynga\Framework\Service\V1\Authenticated\Request\LegacyParameters::class,
      $obj->request()->legacyParameters()
    );
  }

  public function hideLogs(): bool { return false; }

  public function testImport(): void {
    $obj = new AuthenticatedService();
    $data = Map {};
    $data['snid'] = 1;
    $data['uid'] = 1;
    $data['passcode'] = md5('somevalue');
    $data['checksum'] = 'c273cab2b90916e7e207088dfca88d4f';
    $data['legacyParameters'] = Map {
      'systemMemory' => 1024
    };
    $this->assertTrue($obj->request()->import()->fromMap($data));
  }

  public function testImport_WithParams(): void {
    $obj = new AuthenticatedServiceWithParams();
    $data = Map {};
    $data['snid'] = 1;
    $data['uid'] = 1;
    $data['passcode'] = md5('somevalue');
    $data['checksum'] = 'c68a59514cd03a3c5cd2e0dbea4e0cf0';

    $data['serviceData'] = Map {
      'aInt' => 12387598745
    };

    $data['legacyParameters'] = Map {
      'systemMemory' => 1024
    };
    $this->assertTrue($obj->request()->import()->fromMap($data));
  }


  public function testImportMap_BadChecksum(): void {
    $obj = new AuthenticatedService();
    $data = Map {};
    $data['snid'] = 1;
    $data['uid'] = 1;
    $data['passcode'] = md5('somevalue');
    $data['checksum'] = '53c246c38703ca02e1c3f67af025cde7';
    $data['legacyParameters'] = Map {
      'systemMemory' => 1024
    };
    $this->expectException(\Zynga\Framework\Service\V1\Authenticated\Request\Exceptions\IncorrectChecksumException::class);
    $obj->request()->import()->fromMap($data);
  }

  public function testImportMap_BadParent(): void {

    // JEO: We need some generic mocks created on the framework itself we
    //      shouldn't have to bind against rando services.
    $parent = new \Zynga\Poker\Services\Internal\Economy\Dashboard\V1\Request();
    $child = new \Zynga\Framework\Service\V1\Authenticated\Request\Importer($parent);

    $obj = new AuthenticatedService();
    $data = Map {};
    $data['snid'] = 1;
    $data['uid'] = 1;
    $data['passcode'] = md5('somevalue');
    $data['checksum'] = '53c246c38703ca02e1c3f67af025cde7';
    $data['legacyParameters'] = Map {
      'systemMemory' => 1024
    };
    $this->expectException(\InvalidArgumentException::class);
    $child->fromMap($data);
  }

}
