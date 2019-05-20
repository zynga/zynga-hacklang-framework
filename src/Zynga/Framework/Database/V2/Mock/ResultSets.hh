<?hh // strict

namespace Zynga\Framework\Database\V2\Mock;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Database\V2\Exceptions\Mock\BadResultOffsetException;
use
  Zynga\Framework\Database\V2\Exceptions\Mock\LoadTestDataNoFileFoundException
;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Logging\V1\StaticLogger;

class ResultSets {
  private ?Vector<Vector<mixed>> $_resultSets;

  public function __construct() {
    $this->_resultSets = null;
  }

  public function clear(): bool {
    $this->_resultSets = null;
    return true;
  }

  public function getResultSets(): Vector<Vector<mixed>> {
    if ($this->_resultSets === null) {
      $this->_resultSets = Vector {};
    }
    return $this->_resultSets;
  }

  public function addEmptyResultSet(): bool {
    $resultSets = $this->getResultSets();
    $resultSets[] = Vector {};
    return true;
  }

  public function addFailingResultSet(): bool {
    $resultSets = $this->getResultSets();
    $resultSets[] = Vector {false};
    return true;
  }

  public function addResultSet(Vector<mixed> $resultSet): bool {
    $resultSets = $this->getResultSets();
    $resultSets[] = $resultSet;
    return true;
  }

  public function getResultSet(string $context, int $rsOffset): Vector<mixed> {

    try {

      $resultSets = $this->getResultSets();

      if ($resultSets->containsKey($rsOffset) &&
          $resultSets[$rsOffset] instanceof Vector) {
        $rsData = $resultSets[$rsOffset];
        return $rsData;
      }

      throw new BadResultOffsetException($context.' offset='.$rsOffset);

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function getFileNameForTest(
    string $class,
    string $functionName,
  ): string {

    $fileName = CodePath::getRoot();
    $fileName .=
      '/tests/data/'.str_replace('\\', '/', $class).'/'.$functionName.'.data';

    if (is_file($fileName)) {
      return $fileName;
    }

    $frameworkFile = dirname(__FILE__);
    $frameworkFile = dirname($frameworkFile); // Mock
    $frameworkFile = dirname($frameworkFile); // V2
    $frameworkFile = dirname($frameworkFile); // Database
    $frameworkFile = dirname($frameworkFile); // Framework
    $frameworkFile = dirname($frameworkFile); // Zynga
    $frameworkFile = dirname($frameworkFile); // src
    $frameworkFile .=
      '/tests/data/'.str_replace('\\', '/', $class).'/'.$functionName.'.data';

    return $frameworkFile;

  }

  public function loadResultsForTest(
    string $class,
    string $functionName,
  ): bool {

    $fileName = $this->getFileNameForTest($class, $functionName);

    try {

      return $this->loadResultsFromFile($fileName);

    } catch (Exception $e) {

      throw $e;

    }

  }

  public function loadResultsFromFile(string $fileName): bool {

    // reset the results from to empty
    $this->clear();

    // get our result set stack.
    $resultSets = $this->getResultSets();

    if (is_file($fileName)) {

      $fh = fopen($fileName, 'r');

      if (is_resource($fh)) {

        while (feof($fh) != true) {

          $line = fgets($fh);

          // exclude comment lines
          if (preg_match('/^\/\//', $line)) {
            continue;
          }

          // load the data row
          $pregs = array();

          if (preg_match('/^(\d+)\|(.*)$/', $line, $pregs)) {

            $dataset = intval($pregs[1]);
            $rawData = $pregs[2];

            if ($resultSets->containsKey($dataset) !== true) {
              $resultSets[] = Vector {};
            }

            if ($rawData == 'false') {
              $resultSets[$dataset][] = false;
              continue;
            }

            if (preg_match('/^\[.*\]$/', $rawData)) {
              // vector data set
              $data = json_decode($rawData);
              $vec = new Vector($data);
              $resultSets[$dataset][] = $vec;
            } else if ($rawData == '{}') {
              // noop data set
            } else {

              // Default map
              $data = json_decode($rawData, true);

              if ($data === null) {
                // JEO, this error_log is unfortun
                StaticLogger::warning(
                  'WARNING: mock data line failed to decode',
                  Map {'line' => $line},
                );
                continue;
              }

              $map = new Map($data);

              $resultSets[$dataset][] = $map;

            }

          }

        }
        fclose($fh);

        return true;
      }
    }

    throw new LoadTestDataNoFileFoundException('file='.$fileName);
  }

}
