<?hh //strict

namespace Zynga\Framework\IO\Disk\V1;

use \ReflectionClass;
use \ReflectionMethod;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToCloseFileException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToCreateDirectoryException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToOpenFileException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToWriteToFileException;
use Zynga\Framework\IO\Disk\V1\Exception\ReadPermissionsException;
use Zynga\Framework\IO\Disk\V1\Exception\WritePermissionsException;
use Zynga\Framework\IO\Disk\V1\Manager as DiskIOManager;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFailedBZOpen;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFailedCheckOrCreatePath;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFailedFileOpen;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFailedReadPermissions;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFailedToCloseFile;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFailedToWriteToFile;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFailedWritePermissions;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFeofFalseOnce;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFeofFalseOnceBzcloseFails;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFeofFalseOnceFcloseFails;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFileWriteZeroBytes;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithIsDirectoryTrueAndScanDirectoryReturnsNonsense;
use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithRmdirFalse;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Environment\CodePath\V1\CodePath;


class ManagerTest extends TestCase {

  public function doTearDownAfterClass(): bool {
    DiskIOManager::instance()->recursivelyDeleteDirectory(CodePath::getRoot().'/ManagerTest');
    return parent::doTearDownAfterClass();
  }

  public function testDoesFileExistReturnsFalseOnEmptyString(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('doesFileExist');
    $method->setAccessible(true);
    $fileExists = $method->invoke(DiskIOManager::instance(), '');
    $this->assertFalse($fileExists);
  }

  public function testFileOpenWithBadModeReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fileOpen');
    $method->setAccessible(true);
    $handle = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/1', 'y');
    $this->assertFalse($handle);

    // Just in case it didn't fail opening
    if (is_resource($handle)) {
      fclose($handle);
    }
  }

  public function testDirectoryNameReturnsCorrectDirectory(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('directoryName');
    $method->setAccessible(true);
    $directory = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/2');
    $this->assertEquals(CodePath::getRoot().'/ManagerTest', $directory);
  }

  public function testMakeDirectoryReturnsFalseOnExistingDirectory(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('makeDirectory');
    $method->setAccessible(true);
    $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/3', 0000, false);
    $success = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/3', 0000, false);
    $this->assertFalse($success);
  }

  public function testUnlinkNonexistentFileReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('unlink');
    $method->setAccessible(true);
    $success = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/4');
    $this->assertFalse($success);
  }

  public function testFwriteWithValidHandleReturnsCorrectCount(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/5', 0777, true);
    $handle = fopen(CodePath::getRoot().'/ManagerTest/5/1', 'w');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fwrite');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle, 'test');
    fclose($handle);
    unlink(CodePath::getRoot().'/ManagerTest/5/1');
    rmdir(CodePath::getRoot().'/ManagerTest/5');
    $this->assertEquals($result, 4);
  }

  public function testFCloseWithValidHanldeReturnsTrue(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/6', 0777, true);
    $handle = fopen(CodePath::getRoot().'/ManagerTest/6', 'r');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fclose');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle);
    $this->assertTrue($result);

    // Just in case it failed to close
    if (!$result) {
      fclose($handle);
    }

    rmdir(CodePath::getRoot().'/ManagerTest/6');
  }

  public function testIsReadableWithInvalidFilenameReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('isReadable');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/7');
    $this->assertFalse($result);
  }

  public function testIsWriteableWithInvalidFilenameReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('isReadable');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/8');
    $this->assertFalse($result);
  }

  public function testBzopenWithBadModeReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzopen');
    $method->setAccessible(true);
    $handle = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/9', 'y');
    $this->assertFalse($handle);

    // Just in case it didn't fail opening
    if (is_resource($handle)) {
      bzclose($handle);
    }
  }

  public function testBzopenWithValidModeReturnsResource(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/10', 0777, true);
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzopen');
    $method->setAccessible(true);
    $handle = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/10/1.tar.bz2', 'w');
    $this->assertTrue(is_resource($handle));

    if ($handle !== false) {
      bzclose($handle);
      unlink(CodePath::getRoot().'/ManagerTest/10/1.tar.bz2');
    }

    rmdir(CodePath::getRoot().'/ManagerTest/10');
  }

  public function testBzcloseWithValidHandleReturnsTrue(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/11', 0777, true);
    $handle = bzopen(CodePath::getRoot().'/ManagerTest/11/1.tar.bz2', 'w');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzclose');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle);
    $this->assertTrue($result);
    unlink(CodePath::getRoot().'/ManagerTest/11/1.tar.bz2');
    rmdir(CodePath::getRoot().'/ManagerTest/11');
  }

  public function testCheckOrCreatePathWithNewPathReturnsTrue(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('checkOrCreatePath');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/12', 0777);
    $this->assertTrue($result);
    rmdir(CodePath::getRoot().'/ManagerTest/12');
  }

  public function testCheckOrCreatePathWithExistingPathReturnsTrue(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('checkOrCreatePath');
    $method->setAccessible(true);
    $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/13', 0777);
    $result = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/13', 0777);
    $this->assertTrue($result);
    rmdir(CodePath::getRoot().'/ManagerTest/13');
  }

  public function testDeleteFileWithExistingFileReturnsTrue(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/14', 0777, true);
    touch(CodePath::getRoot().'/ManagerTest/14/1');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('deleteFile');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/14/1');
    $this->assertTrue($result);
    rmdir(CodePath::getRoot().'/ManagerTest/14');
  }

  public function testDeleteFileWithNonexistingFileReturnsTrue(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/15', 0777, true);
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('deleteFile');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), CodePath::getRoot().'/ManagerTest/15/1');
    $this->assertTrue($result);
    rmdir(CodePath::getRoot().'/ManagerTest/15');
  }

  public function testFeofReturnsFalseForResourceWithMoreToRead(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/16', 0777, true);
    touch(CodePath::getRoot().'/ManagerTest/16/1');
    $handle = fopen(CodePath::getRoot().'/ManagerTest/16/1', 'wr');
    fwrite($handle, '12345');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('feof');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle);
    $this->assertFalse($result);
    unlink(CodePath::getRoot().'/ManagerTest/16/1');
    rmdir(CodePath::getRoot().'/ManagerTest/16');
  }

  public function testFgetsReturnsValidCountForResource(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/17', 0777, true);
    touch(CodePath::getRoot().'/ManagerTest/17/1');
    $handle = fopen(CodePath::getRoot().'/ManagerTest/17/1', 'w');
    fwrite($handle, '12345');
    fclose($handle);
    $handle = fopen(CodePath::getRoot().'/ManagerTest/17/1', 'r');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fgets');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle, 20);
    $this->assertEquals('12345', $result);
    fclose($handle);
    unlink(CodePath::getRoot().'/ManagerTest/17/1');
    rmdir(CodePath::getRoot().'/ManagerTest/17');
  }

  public function testBZWriteReturnsValidCountForResource(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/18', 0777, true);
    $handle = bzopen(CodePath::getRoot().'/ManagerTest/18/1', 'w');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzwrite');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle, '1', 20);
    $this->assertEquals(1, $result);
    bzclose($handle);
    unlink(CodePath::getRoot().'/ManagerTest/18/1');
    rmdir(CodePath::getRoot().'/ManagerTest/18');
  }

  public function testWriteFileThrowsFailedToCreateDirectoryException(): void {
    $this->expectException(FailedToCreateDirectoryException::class);
    ManagerWithFailedCheckOrCreatePath::instance()->writeFile('', '', 0777, true);
  }

  public function testWriteFileThrowsFailedToOpenFileException(): void {
    $this->expectException(FailedToOpenFileException::class);
    ManagerWithFailedFileOpen::instance()->writeFile('', '', 0777, true);
  }

  public function testWriteFileThrowsFailedToWriteToFileException(): void {
    $this->expectException(FailedToWriteToFileException::class);
    ManagerWithFailedToWriteToFile::instance()->writeFile('', '', 0777, true);
  }

  public function testWriteToFileWithIncorrectBytesThrowsFailedToWriteToFileException(): void {
    $this->expectException(FailedToWriteToFileException::class);
    ManagerWithFileWriteZeroBytes::instance()->writeFile('', 'asdf', 0777, true);
  }

  public function testWriteFileThrowsFailedToCloseFileException(): void {
    $this->expectException(FailedToCloseFileException::class);
    ManagerWithFailedToCloseFile::instance()->writeFile('', '', 0777, true);
  }

  public function testBZip2ThrowsReadPermissionsException(): void {
    $this->expectException(ReadPermissionsException::class);
    ManagerWithFailedReadPermissions::instance()->bzip2('', '');
  }

  public function testBZip2ThrowsWritePermissionsException(): void {
    $this->expectException(WritePermissionsException::class);
    ManagerWithFailedWritePermissions::instance()->bzip2('', '');
  }

  public function testBZip2ThrowsFailedToOpenFileExceptionForFOpen(): void {
    $this->expectException(FailedToOpenFileException::class);
    ManagerWithFailedFileOpen::instance()->bzip2('', '');
  }

  public function testBZip2ThrowsFailedToOpenFileExceptionForBZOpen(): void {
    $this->expectException(FailedToOpenFileException::class);
    ManagerWithFailedBZOpen::instance()->bzip2('', '');
  }

  public function testBZip2ThrowsFailedToCloseFileExceptionForFclose(): void {
    $this->expectException(FailedToCloseFileException::class);
    ManagerWithFeofFalseOnceFcloseFails::instance()->bzip2('', '');
  }

  public function testBZip2ThrowsFailedToCloseFileExceptionForBzclose(): void {
    $this->expectException(FailedToCloseFileException::class);
    ManagerWithFeofFalseOnceFcloseFails::instance()->bzip2('', '');
  }

  public function testChown(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/19', 0777, true);
    $result = DiskIOManager::instance()->chown(CodePath::getRoot().'/ManagerTest/19', get_current_user());
    $this->assertTrue($result);
    rmdir(CodePath::getRoot().'/ManagerTest/19');
  }

  public function testRecursivelyDeleteDirectoryForNestedDirectoriesReturnsTrue(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/20', 0777, true);
    touch(CodePath::getRoot().'/ManagerTest/20/0.txt');
    mkdir(CodePath::getRoot().'/ManagerTest/20/0', 0777, true);
    touch(CodePath::getRoot().'/ManagerTest/20/0/0.txt');
    mkdir(CodePath::getRoot().'/ManagerTest/20/0/0', 0777, true);
    mkdir(CodePath::getRoot().'/ManagerTest/20/1', 0777, true);
    mkdir(CodePath::getRoot().'/ManagerTest/20/1/0', 0777, true);
    mkdir(CodePath::getRoot().'/ManagerTest/20/1/0/0', 0777, true);
    touch(CodePath::getRoot().'/ManagerTest/20/1/0/0/0.txt');
    $this->assertTrue(file_exists(CodePath::getRoot().'/ManagerTest/20'));
    $this->assertTrue(file_exists(CodePath::getRoot().'/ManagerTest/20/0.txt'));
    $this->assertTrue(file_exists(CodePath::getRoot().'/ManagerTest/20/0'));
    $this->assertTrue(file_exists(CodePath::getRoot().'/ManagerTest/20/0/0.txt'));
    $this->assertTrue(file_exists(CodePath::getRoot().'/ManagerTest/20/0/0'));
    $this->assertTrue(file_exists(CodePath::getRoot().'/ManagerTest/20/1'));
    $this->assertTrue(file_exists(CodePath::getRoot().'/ManagerTest/20/1/0'));
    $this->assertTrue(file_exists(CodePath::getRoot().'/ManagerTest/20/1/0/0'));
    $this->assertTrue(file_exists(CodePath::getRoot().'/ManagerTest/20/1/0/0/0.txt'));
    $result = DiskIOManager::instance()->recursivelyDeleteDirectory(CodePath::getRoot().'/ManagerTest/20');
    $this->assertTrue($result);
    $this->assertFalse(file_exists(CodePath::getRoot().'/ManagerTest/20'));
    $this->assertFalse(file_exists(CodePath::getRoot().'/ManagerTest/20/0.txt'));
    $this->assertFalse(file_exists(CodePath::getRoot().'/ManagerTest/20/0'));
    $this->assertFalse(file_exists(CodePath::getRoot().'/ManagerTest/20/0/0.txt'));
    $this->assertFalse(file_exists(CodePath::getRoot().'/ManagerTest/20/0/0'));
    $this->assertFalse(file_exists(CodePath::getRoot().'/ManagerTest/20/1'));
    $this->assertFalse(file_exists(CodePath::getRoot().'/ManagerTest/20/1/0'));
    $this->assertFalse(file_exists(CodePath::getRoot().'/ManagerTest/20/1/0/0'));
    $this->assertFalse(file_exists(CodePath::getRoot().'/ManagerTest/20/1/0/0/0.txt'));
  }

  public function testRecursivelyDeleteDirectoryWithNonExistentPath(): void {
    $result = DiskIOManager::instance()->recursivelyDeleteDirectory(CodePath::getRoot().'/ManagerTest/21');
    $this->assertFalse($result);
  }

  public function testRecursivelyDeleteDirectoryWithRecursiveFailure(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/22', 0777, true);
    $result = ManagerWithIsDirectoryTrueAndScanDirectoryReturnsNonsense::instance()->recursivelyDeleteDirectory(CodePath::getRoot().'/ManagerTest/22');
    $this->assertFalse($result);
    rmdir(CodePath::getRoot().'/ManagerTest/22');
  }

  public function testRecursivelyDeleteDirectoryWithRmdirFailure(): void {
    mkdir(CodePath::getRoot().'/ManagerTest/23', 0777, true);
    $result = ManagerWithRmdirFalse::instance()->recursivelyDeleteDirectory(CodePath::getRoot().'/ManagerTest/23');
    $this->assertFalse($result);
    rmdir(CodePath::getRoot().'/ManagerTest/23');
  }

}
