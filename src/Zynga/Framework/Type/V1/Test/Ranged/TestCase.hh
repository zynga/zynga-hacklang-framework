<?hh // strict

namespace Zynga\Framework\Type\V1\Test\Ranged;

use Zynga\Framework\Type\V1\Test\TestCase as TypeTestCase;
use Zynga\Framework\Exception\V1\Exception;

abstract class TestCase extends TypeTestCase {
  private int $_min;
  private int $_max;

  public function __construct(string $name) {

    parent::__construct($name);

    // @codeCoverageIgnoreStart
    $this->_min = 0;
    $this->_max = 0;
    // @codeCoverageIgnoreEnd

  }

  public function getIterations(): int {
    return 100;
  }

  public function setMin(int $min): bool {
    $this->_min = $min;
    return true;
  }

  public function setMax(int $max): bool {
    $this->_max = $max;
    return true;
  }

  public function generateValidValues(): Vector<mixed> {
    $values = Vector {};
    for ($i = 0; $i < $this->getIterations(); $i++) {
      $value = mt_rand(0, $this->_max);
      if (!in_array($value, $values)) {
        $values[] = $value;
      }
    }
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $vec = Vector {};
    $vec[] = 'Not numeric Data';
    $vec[] = 'of';
    $vec[] = 'non';
    $vec[] = 'numeric';
    return $vec;
  }

  public function testRange(): void {

    $obj = $this->getTypeObject();

    // bring it in with the via the intval
    $this->assertTrue($obj->set($this->_min));
    $this->assertEquals($this->_min, $obj->get());

    $this->assertTrue($obj->set($this->_max));
    $this->assertEquals($this->_max, $obj->get());

    // Do the test with the value converted to string.
    $this->assertTrue($obj->set(strval($this->_min)));
    $this->assertEquals($this->_min, $obj->get());

    $this->assertTrue($obj->set(strval($this->_max)));
    $this->assertEquals($this->_max, $obj->get());

  }

  public function testOOBMin(): void {

    try {

      $oobMin = $this->_min;

      // --
      // JEO: All current _min values are <= 0, therfor commenting out the
      // non-needed code for now.
      // --
      // if ( $this->_min > 0 ) {
      //  $oobMin = $this->_min - 1;
      //} else {
      $oobMin = $this->_min + -1;
      //}

      $obj = $this->getTypeObject();

      $obj->set($oobMin);

      // @codeCoverageIgnoreStart
      $this->fail(
        'value='.
        strval($oobMin).
        ' should of failed, it exceeds the min value for this range',
      );
      // @codeCoverageIgnoreEnd

    } catch (Exception $e) {
      $this->assertTrue(true);
    }

  }

  public function testOOBMax(): void {
    try {

      $oobMax = $this->_max + 1;

      $obj = $this->getTypeObject();

      $obj->set($oobMax);

      // @codeCoverageIgnoreStart
      $this->fail(
        'value='.
        strval($oobMax).
        ' should of failed, it exceeds the max value for this range',
      );
      // @codeCoverageIgnoreEnd

    } catch (Exception $e) {
      $this->assertTrue(true);
    }
  }

}
