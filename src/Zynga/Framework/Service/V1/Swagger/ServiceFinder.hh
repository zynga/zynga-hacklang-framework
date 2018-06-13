<?hh // strict

namespace Zynga\Framework\Service\V1\Swagger;

use Zynga\Framework\Service\V1\Swagger\ServiceFile;

class ServiceFinder {
  private string $_classPathBase;
  private string $_codePathBase;
  private Vector<ServiceFile> $_services;

  public function __construct(string $_codePathBase, string $_classPathBase) {
    $this->_services = Vector {};
    $this->_classPathBase = $_classPathBase;
    $this->_codePathBase = $_codePathBase;
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
    return $this->findServicesOnPath($this->_classPathBase);
  }

  public function findServicesOnPath(string $path): bool {

    $fds = scandir($path);

    foreach ($fds as $fd) {

      if ($fd == '.' || $fd == '..') {
        continue;
      }

      $candidate = $path.'/'.$fd;

      if (is_file($candidate) && $this->isFileNameService($fd) === true) {

        $serviceFile = new ServiceFile();
        $serviceFile->setFilePath($this->_classPathBase, $candidate);
        $serviceFile->setCodePath($this->_codePathBase);

        // var_dump($serviceFile);

        // verify if there are services availble within the file, otherwise skip
        if ( $serviceFile->doesContainService() === true ) {
          $this->_services[] = $serviceFile;
        }

      } else if (is_dir($candidate)) {
        $this->findServicesOnPath($candidate);
      }

    }

    return true;

  }

}
