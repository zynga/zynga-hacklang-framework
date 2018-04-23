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

/**
 * Lightweight class for managing Web IO
 */
class Manager {

  /**
   * Uploads $fileName to $uploadUrl using a PUT request
   *
   * @throws Zynga\Framework\IO\Web\V1\Exception\WebIO
   */
  public static function putFile(
    UrlBox $uploadUrl,
    string $fileName,
    string $mimeType
  ): void {
    $returnCode = 0;
    $returnCodes = array();
    $cmd = "curl -s -w '%{http_code}' -T";
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
      throw new UnexpectedHttpCodeException((string)$returnCode);
    }
  }
}
