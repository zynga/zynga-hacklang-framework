<?hh // strict

namespace Zynga\Framework\Type\V1\Test;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

abstract class TestCase extends ZyngaTestCase {
  abstract public function getTypeObject(): TypeInterface;

  abstract public function generateValidValues(): Vector<mixed>;

  abstract public function generateInvalidValues(): Vector<mixed>;

  /**
   * @expectedException Zynga\Framework\Type\V1\Exceptions\UnSupportedTypeException
   */
  public function testInvalidType_Import(): void {
    $e = new Exception('BreakingStuff');
    $obj = $this->getTypeObject();
    $obj->set($e);
  }

  public function testValidValues(): void {
    $validValues = $this->generateValidValues();
    // --
    // Every type check should have some version of a set of data that is good
    // for it.
    // --
    if ($validValues->count() == 0) {
      // @codeCoverageIgnoreStart
      $this->fail('all type check tests should have valid values');
      // @codeCoverageIgnoreEnd
    }

    foreach ($validValues as $validValue) {
      // do it in the native test type, if you need to test a string version
      // of a value add it to your valid values set.
      $obj = $this->getTypeObject();
      $this->assertTrue($obj->set($validValue));
    }

    foreach ($validValues as $validValue) {
      $obj = $this->getTypeObject();
      $this->assertTrue($obj->setDefaultValue($validValue));
      list($isDefaultValue, $messages) = $obj->isDefaultValue();
      $this->assertTrue($isDefaultValue);
    }
  }

  public function testInvalidValues(): void {
    $invalidValues = $this->generateInvalidValues();
    // --
    // Every type check should have some version of a set of data that is bad
    // for it.
    // --
    if ($invalidValues->count() == 0) {
      // @codeCoverageIgnoreStart
      $this->fail('all type check tests should have invalid values');
      // @codeCoverageIgnoreEnd
    }

    foreach ($invalidValues as $invalidValue) {
      // do it in the native test type first.
      try {
        $obj = $this->getTypeObject();
        $obj->set($invalidValue);
        // @codeCoverageIgnoreStart
        $this->fail(
          'value='.strval($invalidValue).' should of been rejected',
        );
        // @codeCoverageIgnoreEnd
      } catch (Exception $e) {
        $this->assertTrue(true);
      }

      // now convert the value into a string, and test again.
      try {
        $stringValue = strval($invalidValue);
        $obj = $this->getTypeObject();
        $obj->set($stringValue);
        // @codeCoverageIgnoreStart
        $this->fail('value='.$stringValue.' should of been rejected for set');
        // @codeCoverageIgnoreEnd
      } catch (Exception $e) {
        $this->assertTrue(true);
      }
    }

    foreach ($invalidValues as $invalidValue) {
      // do it in the native test type first.
      try {
        $obj = $this->getTypeObject();
        $obj->setDefaultValue($invalidValue);
        // @codeCoverageIgnoreStart
        $this->fail(
          'value='.
          strval($invalidValue).
          ' should of been rejected for setDefaultValue',
        );
        // @codeCoverageIgnoreEnd
      } catch (Exception $e) {
        $this->assertTrue(true);
      }
    }
  }

  protected function injectionTestScaffold(Vector<string> $badData): void {
    $testData = $this->generateValidValues();
    foreach ($testData as $k => $testValue) {
      foreach ($badData as $bad) {
        // --
        // If it isn't a string convert it if possible to a string value. so we
        // can trap failures.
        // --
        if (is_int($testValue) || is_float($testValue)) {
          $testValue = strval($testValue);
        }

        if (is_string($testValue)) {
          $testString = $testValue.$bad;
          $obj = $this->getTypeObject();
          try {
            $obj->set($testString);
            // @codeCoverageIgnoreStart
            $this->fail('value='.$testString.' should of failed');
            // @codeCoverageIgnoreEnd
          } catch (Exception $e) {
            $this->assertTrue(true);
          }
        }
      }
    }
  }

  public function testFacebookTagInjection(): void {
    $badData = Vector {};
    $badData[] = '<fb:';
    $this->injectionTestScaffold($badData);
  }

  public function testImgTagInjection(): void {
    $badData = Vector {};
    $badData[] = '<img';
    $this->injectionTestScaffold($badData);
  }

  public function testOnLoadInjection(): void {
    $badData = Vector {};
    $badData[] =
      "<tag src=\"htttp://www.example.com/image.jpeg\" onload = \"javascript:alert('xss');\">";
    $badData[] = "onLoad='exploit'";
    $this->injectionTestScaffold($badData);
  }

  public function testScriptTagInjection(): void {
    $badData = Vector {};
    $badData[] = '<script';
    $badData[] = '<a href="script:';
    $badData[] = 'script=';
    $this->injectionTestScaffold($badData);
  }

  public function testOnFocusInjection(): void {
    $badData = Vector {};
    $badData[] = '<img onfocus="script:';
    $badData[] = '<obj onFocus=';
    $this->injectionTestScaffold($badData);
  }

  public function testIFrameTagInjection(): void {
    $badData = Vector {};
    $badData[] = '<iframe';
    $badData[] = '<fb:iframe';
    $this->injectionTestScaffold($badData);
  }

  public function testFromCharCodeInjection(): void {
    $badData = Vector {};
    $badData[] = 'String.fromCharCode(65, 66, 67);';
    $badData[] = urlencode('String.fromCharCode(65, 66, 67);');
    $this->injectionTestScaffold($badData);
  }

  public function testReset(): void {

    $obj = $this->getTypeObject();

    $this->assertTrue($obj->reset());

  }

}
