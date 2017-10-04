<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO;

use \PDOStatement;
use Zynga\Framework\Database\V2\Driver\GenericPDO\Mock\SingleEntryPDOStatement;
use Zynga\Framework\Database\V2\Driver\GenericPDO\ResultSet;
use Zynga\Framework\Database\V2\Exceptions\NoActiveCursorException;
use Zynga\Framework\Database\V2\Exceptions\OutOfBoundsForCursorException;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ResultSetTest extends TestCase {
    public function testWasSuccessfulFalse(): void {
        $resultSet = new ResultSet('', null);
        $this->assertFalse($resultSet->wasSuccessful());
    }

    public function testWasSuccessfulTrue(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->assertTrue($resultSet->wasSuccessful());
    }

    public function testFreeCursorTrue(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->assertTrue($resultSet->freeCursor());
    }

    public function testFreeCursorFalse(): void {
        $resultSet = new ResultSet('', null);
        $this->assertFalse($resultSet->freeCursor());
    }

    public function testGetNumRowsZero(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->assertEquals(0, $resultSet->getNumRows());
    }

    public function testGetNumRowsNoActiveCursorException(): void {
        $resultSet = new ResultSet('', null);
        $this->expectException(NoActiveCursorException::class);
        $resultSet->getNumRows();
    }

    public function testGetNumRowsZeroTwice(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->assertEquals(0, $resultSet->getNumRows());
        $this->assertEquals(0, $resultSet->getNumRows());
    }

    public function testRewindNoActiveCursorException(): void {
        $resultSet = new ResultSet('', null);
        $this->expectException(NoActiveCursorException::class);
        $resultSet->rewind(0);
    }

    public function testRewindNegativePosition(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->expectException(OutOfBoundsForCursorException::class);
        $resultSet->rewind(-1);
    }

    public function testValidRewind(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->assertTrue($resultSet->rewind(0));
    }

    public function testHasMoreFalse(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->assertFalse($resultSet->hasMore());
    }

    public function testNextNoActiveCursorException(): void {
        $resultSet = new ResultSet('', null);
        $this->expectException(NoActiveCursorException::class);
        $resultSet->next();
    }

    public function testNextNoneLeft(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->assertFalse($resultSet->next());
    }

    public function testFetchMapNoActiveCursorException(): void {
        $resultSet = new ResultSet('', null);
        $this->expectException(NoActiveCursorException::class);
        $resultSet->fetchMap();
    }

    public function testFetchMapOneResult(): void {
        $resultSet = new ResultSet('', new SingleEntryPDOStatement());
        $this->assertEquals(1, count($resultSet->fetchMap()));
    }

    public function testFetchVectorNoActiveCursorException(): void {
        $resultSet = new ResultSet('', null);
        $this->expectException(NoActiveCursorException::class);
        $resultSet->fetchVector();
    }

    public function testFetchVectorOneResult(): void {
        $resultSet = new ResultSet('', new SingleEntryPDOStatement());
        $this->assertEquals(1, count($resultSet->fetchVector()));
    }

    public function testSetSqlSuccess(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->assertTrue($resultSet->setSql(''));
    }

    public function testGetSqlSuccess(): void {
        $sql = 'testing';
        $resultSet = new ResultSet('', new PDOStatement());
        $resultSet->setSql($sql);
        $this->assertEquals($sql, $resultSet->getSql());
    }

    public function testWasSqlDMLFalse(): void {
        $resultSet = new ResultSet('', new PDOStatement());
        $this->assertFalse($resultSet->wasSqlDML());
    }
}