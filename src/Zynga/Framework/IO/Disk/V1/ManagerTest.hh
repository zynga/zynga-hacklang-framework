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
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;


class ManagerTest extends TestCase {

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
    $handle = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/gdManagerTestpr/1', 'y');
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
    $directory = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/2');
    $this->assertEquals('/home/deploy/tmp/ManagerTest', $directory);
  }

  public function testMakeDirectoryReturnsFalseOnExistingDirectory(): void {
    mkdir('/home/deploy/tmp/ManagerTest/3', 0000, false);
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('makeDirectory');
    $method->setAccessible(true);
    $success = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/3', 0000, false);
    rmdir('/home/deploy/tmp/ManagerTest/3');
    $this->assertFalse($success);
  }

  public function testUnlinkNonexistentFileReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('unlink');
    $method->setAccessible(true);
    $success = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/4');
    $this->assertFalse($success);
  }

  public function testFwriteWithValidHandleReturnsCorrectCount(): void {
    mkdir('/home/deploy/tmp/ManagerTest/5', 0777, true);
    $handle = fopen('/home/deploy/tmp/ManagerTest/5/1', 'w');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fwrite');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle, 'test');
    fclose($handle);
    unlink('/home/deploy/tmp/ManagerTest/5/1');
    rmdir('/home/deploy/tmp/ManagerTest/5');
    $this->assertEquals($result, 4);
  }

  public function testFwriteWithInvalidHandleReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fwrite');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), null, 'test');
    $this->assertFalse($result);
  }

  public function testFCloseWithValidHanldeReturnsTrue(): void {
    mkdir('/home/deploy/tmp/ManagerTest/6', 0777, true);
    $handle = fopen('/home/deploy/tmp/ManagerTest/6', 'r');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fclose');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle);
    $this->assertTrue($result);

    // Just in case it failed to close
    if (!$result) {
      fclose($handle);
    }

    rmdir('/home/deploy/tmp/ManagerTest/6');
  }

  public function testFCloseWithInvalidHanldeReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fclose');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), null);
    $this->assertFalse($result);
  }

  public function testIsReadableWithInvalidFilenameReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('isReadable');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/7');
    $this->assertFalse($result);
  }

  public function testIsWriteableWithInvalidFilenameReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('isReadable');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/8');
    $this->assertFalse($result);
  }

  public function testBzopenWithBadModeReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzopen');
    $method->setAccessible(true);
    $handle = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/9', 'y');
    $this->assertFalse($handle);

    // Just in case it didn't fail opening
    if (is_resource($handle)) {
      bzclose($handle);
    }
  }

  public function testBzopenWithValidModeReturnsResource(): void {
    mkdir('/home/deploy/tmp/ManagerTest/10', 0777, true);
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzopen');
    $method->setAccessible(true);
    $handle = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/10/1.tar.bz2', 'w');
    $this->assertTrue(is_resource($handle));

    if ($handle !== false) {
      bzclose($handle);
      unlink('/home/deploy/tmp/ManagerTest/10/1.tar.bz2');
    }

    rmdir('/home/deploy/tmp/ManagerTest/10');
  }

  public function testBzcloseWithInvalidHandleReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzclose');
    $method->setAccessible(true);
    $handle = $method->invoke(DiskIOManager::instance(), null);
    $this->assertFalse($handle);
  }

  public function testBzcloseWithValidHandleReturnsTrue(): void {
    mkdir('/home/deploy/tmp/ManagerTest/11', 0777, true);
    $handle = bzopen('/home/deploy/tmp/ManagerTest/11/1.tar.bz2', 'w');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzclose');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle);
    $this->assertTrue($result);
    unlink('/home/deploy/tmp/ManagerTest/11/1.tar.bz2');
    rmdir('/home/deploy/tmp/ManagerTest/11');
  }

  public function testCheckOrCreatePathWithNewPathReturnsTrue(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('checkOrCreatePath');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/12', 0777);
    $this->assertTrue($result);
    rmdir('/home/deploy/tmp/ManagerTest/12');
  }

  public function testCheckOrCreatePathWithExistingPathReturnsTrue(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('checkOrCreatePath');
    $method->setAccessible(true);
    $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/13', 0777);
    $result = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/13', 0777);
    $this->assertTrue($result);
    rmdir('/home/deploy/tmp/ManagerTest/13');
  }

  public function testDeleteFileWithExistingFileReturnsTrue(): void {
    mkdir('/home/deploy/tmp/ManagerTest/14', 0777, true);
    touch('/home/deploy/tmp/ManagerTest/14/1');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('deleteFile');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/14/1');
    $this->assertTrue($result);
    rmdir('/home/deploy/tmp/ManagerTest/14');
  }

  public function testDeleteFileWithNonexistingFileReturnsTrue(): void {
    mkdir('/home/deploy/tmp/ManagerTest/15', 0777, true);
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('deleteFile');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), '/home/deploy/tmp/ManagerTest/15/1');
    $this->assertTrue($result);
    rmdir('/home/deploy/tmp/ManagerTest/15');
  }

  public function testFeofReturnsFalseForNonResource(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('feof');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), '');
    $this->assertFalse($result);
  }

  public function testFeofReturnsFalseForResourceWithMoreToRead(): void {
    mkdir('/home/deploy/tmp/ManagerTest/16', 0777, true);
    touch('/home/deploy/tmp/ManagerTest/16/1');
    $handle = fopen('/home/deploy/tmp/ManagerTest/16/1', 'wr');
    fwrite($handle, '12345');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('feof');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle);
    $this->assertFalse($result);
    unlink('/home/deploy/tmp/ManagerTest/16/1');
    rmdir('/home/deploy/tmp/ManagerTest/16');
  }

  public function testFgetsReturnsEmptyStringForNonResource(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fgets');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), '', 100);
    $this->assertEquals('', $result);
  }

  public function testFgetsReturnsValidCountForResource(): void {
    mkdir('/home/deploy/tmp/ManagerTest/17', 0777, true);
    touch('/home/deploy/tmp/ManagerTest/17/1');
    $handle = fopen('/home/deploy/tmp/ManagerTest/17/1', 'w');
    error_log("jsimmer: fwrite=".fwrite($handle, '12345'));
    fclose($handle);
    $handle = fopen('/home/deploy/tmp/ManagerTest/17/1', 'r');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('fgets');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle, 20);
    $this->assertEquals('12345', $result);
    fclose($handle);
    unlink('/home/deploy/tmp/ManagerTest/17/1');
    rmdir('/home/deploy/tmp/ManagerTest/17');
  }

  public function testBzwriteReturnsZeroForNonResource(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzwrite');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), '', '', 100);
    $this->assertEquals(0, $result);
  }

  public function testBZWriteReturnsValidCountForResource(): void {
    mkdir('/home/deploy/tmp/ManagerTest/18', 0777, true);
    $handle = bzopen('/home/deploy/tmp/ManagerTest/18/1', 'w');
    $class = new ReflectionClass('Zynga\Framework\IO\Disk\V1\Manager');
    $method = $class->getMethod('bzwrite');
    $method->setAccessible(true);
    $result = $method->invoke(DiskIOManager::instance(), $handle, '1', 20);
    $this->assertEquals(1, $result);
    bzclose($handle);
    unlink('/home/deploy/tmp/ManagerTest/18/1');
    rmdir('/home/deploy/tmp/ManagerTest/18');
  }

  public function testWriteFileThrowsFailedToCreateDirectoryException(): void {
    $this->expectException(FailedToCreateDirectoryException::class);
    ManagerWithFailedCheckOrCreatePath::instance()->writeFile('', '', 0777);
  }

  public function testWriteFileThrowsFailedToOpenFileException(): void {
    $this->expectException(FailedToOpenFileException::class);
    ManagerWithFailedFileOpen::instance()->writeFile('', '', 0777);
  }

  public function testWriteFileThrowsFailedToWriteToFileException(): void {
    $this->expectException(FailedToWriteToFileException::class);
    ManagerWithFailedToWriteToFile::instance()->writeFile('', '', 0777);
  }

  public function testWriteToFileWithIncorrectBytesThrowsFailedToWriteToFileException(): void {
    $this->expectException(FailedToWriteToFileException::class);
    ManagerWithFileWriteZeroBytes::instance()->writeFile('', 'asdf', 0777);
  }

  public function testWriteFileThrowsFailedToCloseFileException(): void {
    $this->expectException(FailedToCloseFileException::class);
    ManagerWithFailedToCloseFile::instance()->writeFile('', '', 0777);
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

}
