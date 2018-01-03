<?hh // strict

namespace Zynga\Framework\Database\V2\Mock;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Database\V2\Exceptions\Mock\BadResultOffsetException;
use
  Zynga\Framework\Database\V2\Exceptions\Mock\LoadTestDataNoFileFoundException
;
use Zynga\Framework\Exception\V1\Exception;

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

  public function loadResultsForTest(
    string $class,
    string $functionName,
  ): bool {

    $fileName = CodePath::getRoot();
    $fileName .=
      '/tests/data/'.str_replace('\\', '/', $class).'/'.$functionName.'.data';

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

              // TODO: JEO - Need a way to debug the datasets as they load.
              // var_dump($data);

              if ($data === null) {
                error_log(
                  'WARNING: mock data line failed to decode line='.$line,
                );
                continue;
              }

              // JEO: We might want to trap that data === null instead of blindly injecting that into the dataset
              // echo "rawData: $rawData\n";
              // var_dump($data);

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
