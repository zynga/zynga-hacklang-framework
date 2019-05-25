<?hh // strict

namespace Zynga\Framework\IO\Web\V1;

use \CURLFile;
use \resource;
use
  Zynga\Framework\IO\Web\V1\Exception\FailedExecution as FailedExecutionException
;
use
  Zynga\Framework\IO\Web\V1\Exception\FailedInitialization as FailedInitializationException
;
use
  Zynga\Framework\IO\Web\V1\Exception\UnexpectedHttpCode as UnexpectedHttpCodeException
;
use Zynga\Framework\Type\V1\UrlBox;
use Zynga\Framework\IO\Web\V1\Curl\CurlRequest;
use Zynga\Framework\IO\Web\V1\Curl\MockedCurlRequest;
use Zynga\Framework\IO\Web\V1\Curl\CurlResponsePayload;
use Zynga\Framework\IO\Web\V1\Interfaces\CurlInterface;

/**
 * Lightweight class for managing Web IO
 */
class Manager {

  private static bool $_useMock = false;
  private static bool $_mockOptionsReturn = false;
  private static ?CurlResponsePayload $_mockExecReturn = null;
  private static Map<int, mixed> $_mockInfoReturn = Map {};

  public static function clear(): bool {
    self::$_useMock = false;
    self::$_mockOptionsReturn = false;
    self::$_mockExecReturn = null;
    self::$_mockInfoReturn->clear();
    return true;
  }

  public static function getUseMock(): bool {
    return self::$_useMock;
  }

  public static function setUseMock(bool $tf): bool {
    self::$_useMock = $tf;
    if ($tf === false) {
      self::clear();
    }
    return true;
  }

  public static function getMockOptionsReturn(): bool {
    return self::$_mockOptionsReturn;
  }

  public static function setOptionsReturn(bool $tf): bool {
    self::$_mockOptionsReturn = $tf;
    return true;
  }

  public static function getMockExecReturn(): CurlResponsePayload {

    if (self::$_mockExecReturn instanceof CurlResponsePayload) {
      return self::$_mockExecReturn;
    }

    self::$_mockExecReturn = new CurlResponsePayload(false, array());
    return self::$_mockExecReturn;

  }

  public static function setMockExecReturn(
    CurlResponsePayload $payload,
  ): bool {
    self::$_mockExecReturn = $payload;
    return true;
  }

  public static function getMockInfoReturn(): Map<int, mixed> {
    return self::$_mockInfoReturn;
  }

  public static function setMockInfoReturn(Map<int, mixed> $info): bool {
    self::$_mockInfoReturn = $info;
    return true;
  }

  /**
   * Uploads $fileName to $uploadUrl using a PUT request
   *
   * @throws Zynga\Framework\IO\Web\V1\Exception\WebIO
   */
  public static function putFile(
    UrlBox $uploadUrl,
    string $fileName,
    string $mimeType,
  ): bool {
    $returnCode = 0;
    $returnCodes = array();

    // TODO: This is calling out to curl on the commandline, we should use the curl_* instead.
    $cmd = 'set -o pipefail; ';
    $cmd .= "curl -s -o /dev/null -w '%{http_code}' -T";
    $cmd .= ' ';
    $cmd .= escapeshellarg($fileName);
    $cmd .= ' ';
    $cmd .= escapeshellarg($uploadUrl->get());
    $cmd .= ' ';
    $cmd .= "| awk {'print $1'}";
    exec($cmd, $returnCodes, $returnCode);

    if ($returnCode !== 0 || count($returnCodes) === 0) {
      throw new FailedExecutionException('Failed to execute cURL');
    }

    $returnCode = $returnCodes[count($returnCodes) - 1];
    if ($returnCode != 200) {
      throw new UnexpectedHttpCodeException((string) $returnCode);
    }

    return true;

  }

  public static function getCurlRequest(UrlBox $url): CurlInterface {
    if (self::getUseMock()) {

      return new MockedCurlRequest(
        self::getMockOptionsReturn(),
        self::getMockExecReturn(),
        self::getMockInfoReturn(),
      );
    }

    return new CurlRequest($url);
  }
}
