<?hh // strict

namespace Zynga\Framework\Service\V2\Swagger;

use Zynga\Framework\Service\V2\Swagger\ServiceFile;
use Zynga\Framework\Service\V2\Service\Group\Config\Pattern;

class ServiceFinder {
  private Pattern $_pattern;
  private Vector<ServiceFile> $_services;

  public function __construct(Pattern $pattern) {
    $this->_services = Vector {};
    $this->_pattern  = $pattern;
  }

  public function getServices(): Vector<ServiceFile> {
    return $this->_services;
  }

  public function isFileNameService(string $fileName): bool {
    if (!preg_match('/(Request|Response|Test|Base|Interface|README)\.(hh|md)$/', $fileName) ===
        true) {
      return true;
    }
    return false;
  }

  public function find(): bool {
    return $this->findServicesOnPath($this->_pattern->filePath->get());
  }

  public function findServicesOnPath(string $path): bool {

    $filePathBase         = $this->_pattern->filePath->get();
    $codePath             = $this->_pattern->codePath->get();
    $mustImplementFilter  = $this->_pattern->mustImplementFilter;

    $fds = scandir($path);

    foreach ($fds as $fd) {

      if ($fd == '.' || $fd == '..') {
        continue;
      }

      $candidate = $path.'/'.$fd;

      if (is_file($candidate) && $this->isFileNameService($fd) === true) {

        $serviceFile = new ServiceFile();
        $serviceFile->setFilePath($filePathBase, $candidate);
        $serviceFile->setCodePath($codePath);

        // verify if there are services availble within the file, otherwise skip
        if ( $serviceFile->doesContainService() !== true ) {
          continue;
        }

        if ( $mustImplementFilter->count() > 0 ) {

          $wasImplemented = false;

          foreach ($mustImplementFilter as $needsClass ) {
            if ( $serviceFile->doesImplementClass($needsClass) === true ) {
              $wasImplemented = true;
            }
          }

          if ( $wasImplemented === false ) {
            continue;
          }

        }

        $this->_services[] = $serviceFile;

      } else if (is_dir($candidate)) {
        $this->findServicesOnPath($candidate);
      }

    }

    return true;

  }

}
