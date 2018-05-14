<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\Base as BaseBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

class StringBox extends BaseBox {
  protected string $_value;
  private bool $_doExploitDetection;

  public function __construct() {
    parent::__construct();
    $this->_value = '';
    $this->_doExploitDetection = true;
  }

  public function __toString(): string {
    return $this->get();
  }

  <<__Override>>
  public function reset(): bool {
    $this->_value = '';
    $this->setIsDefaultValue(true);
    return true;
  }

  public function isExploitDetectionEnabled(): bool {
    return $this->_doExploitDetection;
  }

  public function enableExploitDetection(): bool {
    $this->_doExploitDetection = true;
    return true;
  }

  public function disableExploitDetection(): bool {
    $this->_doExploitDetection = false;
    return true;
  }

  <<__Override>>
  public function get(): string {
    return $this->_value;
  }

  public function isStringValid(string $value): bool {
    $string = new self();
    try {
      $string->set($value);
      return true;
    } catch (FailedToImportFromStringException $e) {
      return false;
    }
  }

  <<__Override>>
  protected function importFromBool(bool $value): bool {
    return $this->importFromString(strval($value));
  }

  <<__Override>>
  protected function importFromFloat(float $value): bool {
    return $this->importFromString(strval($value));
  }

  <<__Override>>
  protected function importFromInteger(int $value): bool {
    return $this->importFromString(strval($value));
  }

  <<__Override>>
  protected function importFromString(string $value): bool {

    $exploit = false;

    if ($this->_doExploitDetection === true &&
        $this->hasKnownExploit($value) === true) {
      $exploit = true;
    }

    if ($exploit === true) {
      throw new FailedToImportFromStringException('value='.$value);
    }

    $this->_value = $value;

    return true;

  }

  // --
  // JEO: Keeping this here because it shows a unique way to detect recursively
  // url encoded values.
  // --
  /*
   public function urlDecodeRecursive(string $value, int $depth = 0): string {
   if (! preg_match("/^[a-zA-Z0-9%+-_]*$/", $value)) {
   return $value;
   }
   // we limit the depth to 4 deep, otherwise we boot them.
   if ($depth > 2) {
   throw new FailedToImportFromStringException(
   'urldecode-depth value='.$value,
   );
   }
   $urldecode = urldecode($value);
   if (strlen($urldecode) !== strlen($value)) {
   $depth++;
   $urldecode = $this->urlDecodeRecursive($value, $depth);
   }
   return $urldecode;
   }
   */

  public function hasKnownExploit(string $value): bool {

    $exploitRegex = '';
    $exploitRegex .= '/(';
    $exploitRegex .= $this->getScriptTagRegex();
    $exploitRegex .= '|';
    $exploitRegex .= $this->getImageTagRegex();
    $exploitRegex .= '|';
    $exploitRegex .= $this->getOnLoadRegex();
    $exploitRegex .= '|';
    $exploitRegex .= $this->getOnFocusRegex();
    $exploitRegex .= '|';
    $exploitRegex .= $this->getFromCharCodeRegex();
    $exploitRegex .= '|';
    $exploitRegex .= $this->getFacebookTagRegex();
    $exploitRegex .= '|';
    $exploitRegex .= $this->getIframeTagRegex();
    $exploitRegex .= ')/i';

    if (preg_match($exploitRegex, $value)) {
      return true;
    }

    return false;

  }

  public function getScriptTagRegex(): string {
    return
      '(script\:|script\=|[\<\xbc]script|\{$bc}script%3Cscript|%BCscript)';
  }

  public function getImageTagRegex(): string {
    return '([\<\xbc]img|%3Cimg|%BCimg)';
  }

  public function getOnLoadRegex(): string {
    return '(onload[\t\r\n ]*=|onload(%08|%0D|%0A|%20|\+)*%3D)';
  }

  public function getOnFocusRegex(): string {
    return '(onfocus)';
  }

  public function getFromCharCodeRegex(): string {
    return '(fromcharcode)';
  }

  public function getFacebookTagRegex(): string {
    return '([\<\xbc]fb\:|%3Cfb%3A|%BCfb%3A)';
  }

  public function getIframeTagRegex(): string {
    return '([\<\xbc]iframe|%3Ciframe|%BCiframe)';
  }

  /**
   * For UTF8 characters, this will count them as 1 character.
   * This is different than what strlen returns where certain UTF8 characters are counted as double the size.
   * Because we take into account UTF8 characters, this is slower than just calling strlen.
   * @see http://php.net/manual/en/function.strlen.php
   * @param $encoding
   */
  public function getLength(string $encoding = mb_internal_encoding()): int {

    $result = mb_strlen($this->_value, $encoding);

    // --
    // Normally, I would argue for === false here, but I was unable to create
    // a string that would enduce a false return out of mb_strlen. I think
    // the implementation within hacklang is a smidge better than the php one.
    // --
    if ($result == false) {
      return 0;
    }

    return $result;

  }
}
