<?hh // strict

namespace Zynga\Framework\IO\Web\V1;

use \CURLFile;
use \resource;
use Zynga\Framework\IO\Web\V1\Exception\FailedExecution as FailedExecutionException;
use Zynga\Framework\IO\Web\V1\Exception\FailedInitialization as FailedInitializationException;
use Zynga\Framework\IO\Web\V1\Exception\UnexpectedHttpCode as UnexpectedHttpCodeException;
use Zynga\Framework\Type\V1\UrlBox;

/**
 * Lightweight class for managing Web IO
 */
class Manager {

  /**
   * Uploads $fileName to $uploadUrl using a PUT request
   *
   * @throws Zynga\Framework\IO\Web\V1\Exception\WebIO
   */
  public static function putFile(UrlBox $uploadUrl, string $fileName, string $mimeType): void {
    $ch = static::initializeCurl($uploadUrl);
    if ($ch === false) {
      throw new FailedInitializationException('Failed to initialize cURL handle');
    }

    $cfile = static::createCurlFile($fileName, $mimeType, $fileName);
    $data = array('file' => $cfile);
    static::setCurlOption($ch, CURLOPT_POSTFIELDS, http_build_query($data));
    static::setCurlOption($ch, CURLOPT_CUSTOMREQUEST, "PUT");

    if (static::executeCurl($ch) === false) {
      throw new FailedExecutionException('Failed to execute cURL');
    }

    $returnCode = static::getCurlInfo($ch, CURLINFO_HTTP_CODE);
    if ($returnCode !== 200) {
      throw new UnexpectedHttpCodeException((string)$returnCode);
    }
  }

  // Have to use mixed for $curlHandle here because Hack doesn't allow
  // passing resource types to functions
  protected static function getCurlInfo(mixed $curlHandle, int $flag): mixed {
    if (is_resource($curlHandle)) {
      return curl_getinfo($curlHandle, $flag);
    } else {
      return false;
    }
  }

  // Have to use mixed for $curlHandle here because Hack doesn't allow
  // passing resource types to functions
  protected static function executeCurl(mixed $curlHandle): mixed {
    if (is_resource($curlHandle)) {
      return curl_exec($curlHandle);
    } else {
      return false;
    }
  }

  protected static function initializeCurl(UrlBox $url): mixed {
    return curl_init($url->get());
  }

  protected static function createCurlFile(string $fileName,
                                         string $mimeType,
                                         string $newFileName): CURLFile {
    return curl_file_create($fileName, $mimeType, $fileName);
  }

  // Have to use mixed for $curlHandle here because Hack doesn't allow
  // passing resource types to functions
  protected static function setCurlOption(mixed $curlHandle, int $option, mixed $value): bool {
    if (is_resource($curlHandle)) {
      return curl_setopt($curlHandle, $option, $value);
    } else {
      return false;
    }
  }

}
