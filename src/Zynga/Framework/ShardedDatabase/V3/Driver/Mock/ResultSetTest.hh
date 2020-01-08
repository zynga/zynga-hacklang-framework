<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Mock;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Database\V2\Exceptions\Mock\ResultSetConfiguredWithNonMapException;
use Zynga\Framework\Database\V2\Exceptions\Mock\ResultSetConfiguredWithNonVectorException;
use Zynga\Framework\Database\V2\Exceptions\OutOfBoundsForCursorException;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;


use Zynga\Framework\ShardedDatabase\V3\Driver\Mock;
use Zynga\Framework\ShardedDatabase\V3\Driver\Mock\ResultSet;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class ResultSetTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::enableMockDrivers();
    DatabaseFactory::clear();

    return true;

  }

  public function testWasSuccessful(): void {
    $obj = new ResultSet();
    $this->assertTrue($obj->wasSuccessful());
  }

  public function testHasCursor(): void {
    $obj = new ResultSet();
    $this->assertTrue($obj->hasCursor());
  }

  public function testFreeCursor(): void {
    $obj = new ResultSet();
    $this->assertTrue($obj->freeCursor());
  }

  public function testGetNumRows(): void {
    $obj = new ResultSet();
    $this->assertEquals(0, $obj->getNumRows());
  }

  public function getMockDatabase(string $class, string $function): Mock<UInt64Box> {

    try {

      $mock = DatabaseFactory::factory(DriverInterface::class, 'Mock');

      $mock->setShardType(new UInt64Box(1));

      if ($mock instanceof Mock) {

        $mock->loadResultsForTest($class, $function);
        return $mock;

      }

      throw new Exception('MockDatabase failure class=' . get_class($mock));

    } catch ( Exception $e ) {
      throw $e;
    }

  }

  public function testRewind(): void {

    // first test rewind with no results.
    $mock = $this->getMockDatabase(self::class, __FUNCTION__);

    // no data...
    $obj = $mock->query('SELECT * FROM dual');
    $this->assertFalse($obj->rewind(0));

  }

  public function testRewindWithData(): void {

    // first test rewind with results.
    $mock = $this->getMockDatabase(self::class, __FUNCTION__);

    $obj = $mock->query('SELECT * FROM dual');

    // Test to position(s)
    $this->assertTrue($obj->rewind(0));
    $this->assertTrue($obj->rewind(1));
    $this->assertFalse($obj->rewind(2));

  }

  public function testHasMore(): void {
    $obj = new ResultSet();
    $this->assertFalse($obj->hasMore());
  }

  public function testNext(): void {
    $obj = new ResultSet();
    $this->assertFalse($obj->next());
  }

  public function testFetchMap(): void {

    $data = Vector {};
    $data[] = Map {'a' => 'b'};

    $obj = new ResultSet();
    $obj->setResultData($data);

    $this->assertTrue($obj->hasMore());
    $obj->next();
    $this->assertEquals(Map::class, get_class($obj->fetchMap()));

  }

  public function testFetchMapInvalidDataType(): void {

    $data = Vector {};
    $data[] = Vector {'a', 'b'};

    $obj = new ResultSet();
    $obj->setResultData($data);

    $obj->next();

    $this->expectException(ResultSetConfiguredWithNonMapException::class);
    $obj->fetchMap();

  }

  public function testFetchMapOOB(): void {

    $data = Vector {};
    $obj = new ResultSet();
    $obj->setResultData($data);
    $this->expectException(OutOfBoundsForCursorException::class);
    $obj->fetchMap();

  }

  public function testFetchVector(): void {

    $data = Vector {};
    $data[] = Vector {'a', 'b'};

    $obj = new ResultSet();
    $obj->setResultData($data);

    $obj->next();

    $this->assertEquals(Vector::class, get_class($obj->fetchVector()));

  }

  public function testFetchVectorInvalidDataType(): void {

    $data = Vector {};
    $data[] = Map {'a' => 'b'};

    $obj = new ResultSet();
    $obj->setResultData($data);

    $obj->next();

    $this->expectException(ResultSetConfiguredWithNonVectorException::class);
    $this->assertEquals(Vector::class, get_class($obj->fetchVector()));

  }

  public function test_FetchVectorOOB(): void {

    $data = Vector {};
    $obj = new ResultSet();
    $obj->setResultData($data);
    $this->expectException(OutOfBoundsForCursorException::class);
    $obj->fetchVector();

  }

  public function test_SetSql(): void {
    $obj = new ResultSet();
    $this->assertTrue($obj->setSql('SELECT 1'));
  }

  public function testGetSql(): void {
    $obj = new ResultSet();
    $this->assertEquals('', $obj->getSql());
  }

  public function testWasSqlDML(): void {
    $obj = new ResultSet();
    $this->assertFalse($obj->wasSqlDML());
  }

}
