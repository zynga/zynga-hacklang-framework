<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO;

use Zynga\Framework\Database\V2\Driver\GenericPDO\Quoter;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class QuoterTest extends TestCase {
    public function testNormal(): void {
        $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock_Cluster');
        $quoter = new Quoter($driver);
        $this->assertEquals("3", $quoter->intValue(3));
    }

    public function testFloatValue(): void {
        $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock_Cluster');
        $quoter = new Quoter($driver);
        $this->assertEquals("3.3", $quoter->floatValue(3.3));
    }

    public function testTextValue(): void {
        $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock_Cluster');
        $quoter = new Quoter($driver);
        $this->assertEquals("3.3", $quoter->textValue("3.3"));
    }

    public function testTextVector(): void {
        $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock_Cluster');
        $quoter = new Quoter($driver);
        $vector = Vector{"1", "2"};
        $this->assertEquals("1, 2", $quoter->textVector($vector));
    }
}
