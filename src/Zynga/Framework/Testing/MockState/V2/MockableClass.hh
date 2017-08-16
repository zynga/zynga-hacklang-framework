<?hh // strict

namespace Zynga\Framework\Testing\MockState\V2;

use Zynga\Framework\Dynamic\V1\DynamicMethodCall;

class MockableClass {
  const string METHOD_UNKNOWN = 'unknown';

  private string $_className;
  private bool $_currentState;
  private string $_enableFunction;
  private string $_disableFunction;
  private string $_getStatusFunction;
  private string $_clearFunction;

  public function __construct(string $className) {

    $this->_className = $className;
    $this->_currentState = false;

    $this->_disableFunction = self::METHOD_UNKNOWN;
    $this->_enableFunction  = self::METHOD_UNKNOWN;
    $this->_getStatusFunction = 'unknown';
    $this->_clearFunction = 'unknown';

  }

  public function detect(): bool {

    $this->_disableFunction = $this->detectFunctionName(Vector {
      'disableMockDrivers', 'disableMocks'
    });

    $this->_enableFunction = $this->detectFunctionName(Vector {
      'enableMockDrivers', 'enableMocks'
    });

    $this->_getStatusFunction = $this->detectFunctionName(Vector {
      'getUseMockDrivers', 'getMockState'
    });

    $this->_clearFunction = $this->detectFunctionName(Vector {
      'clear'
    });

    if ( $this->_disableFunction != 'unknown' &&
         $this->_enableFunction  != 'unknown' &&
         $this->_getStatusFunction != 'unknown' ) {
            return true;
         }

    return false;


  }

  public function detectFunctionName(Vector<string> $possibleFunctionNames): string {

    if ( class_exists($this->_className, true) != true ) {
      return 'unknown';
    }

    foreach ( $possibleFunctionNames as $functionName ) {
      if ( method_exists($this->_className, $functionName ) ) {
        return $functionName;
      }
    }

    return 'unknown';

  }

  public function enable(): bool {

    $methodReturn = DynamicMethodCall::callMethod(
      $this->_className,
      $this->_enableFunction,
      false
    );

    return boolval($methodReturn);

  }

  public function disable(): bool {

    $methodReturn = DynamicMethodCall::callMethod(
      $this->_className,
      $this->_disableFunction,
      false
    );

    return boolval($methodReturn);

  }


}
