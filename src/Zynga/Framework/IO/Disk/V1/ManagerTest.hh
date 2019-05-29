<?hh //strict

namespace Zynga\Framework\IO\Disk\V1;

use \ReflectionClass;
use \ReflectionMethod;
use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToCloseFileException;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToCreateDirectoryException;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToOpenFileException;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToWriteToFileException;
use Zynga\Framework\IO\Disk\V1\Exceptions\InvalidFileNameException;
use Zynga\Framework\IO\Disk\V1\Exceptions\ReadPermissionsException;
use Zynga\Framework\IO\Disk\V1\Exceptions\WritePermissionsException;
use Zynga\Framework\IO\Disk\V1\Manager as DiskIOManager;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithDoesFileExistFalse;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFailedBZClose;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFailedBZOpen;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFailedCheckOrCreatePath;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFailedFileOpen;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFailedReadPermissions;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFailedToCloseFile;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFailedToWriteToFile;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFailedWritePermissions;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFeofFalseOnce;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFeofFalseOnceFcloseFails;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithFileWriteZeroBytes;
use
  Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithIsWriteableFalseAndDoesFileExistTrueAndIsReadableTrue
;
use
  Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithIsWriteableTrueAndDoesFileExistTrueAndIsReadableTrue
;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithRmdirFalse;
use Zynga\Framework\IO\Disk\V1\Test\Mock\ManagerWithTarballValidFalse;
use Zynga\Framework\IO\Disk\V1\Interfaces\ManagerInterface;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ManagerTest extends TestCase {

  private function getTempTestDir(): string {
    return CodePath::getRoot().'/tmp/ManagerTest';
  }

  <<__Override>>
  public function doSetUpBeforeClass(): bool {
    DiskIOManager::instance()
      ->recursivelyDeleteDirectory($this->getTempTestDir());
    return parent::doSetUpBeforeClass();
  }

  <<__Override>>
  public function doTearDownAfterClass(): bool {
    DiskIOManager::instance()
      ->recursivelyDeleteDirectory($this->getTempTestDir());
    return parent::doTearDownAfterClass();
  }

  public function testDoesFileExistReturnsFalseOnEmptyString(): void {
    $manager = DiskIOManager::instance();
    $this->assertFalse($manager->doesFileExist(''));
  }

  public function testFileOpenWithBadModeReturnsFalse(): void {
    $manager = DiskIOManager::instance();
    $handle = $manager->fileOpen($this->getTempTestDir().'/1', 'y');

    if (is_bool($handle)) {
      $this->assertFalse($handle);
    } else {
      $this->fail('Expected no handle for this run');
    }

  }

  public function testDirectoryNameReturnsCorrectDirectory(): void {

    $manager = DiskIOManager::instance();
    $directory = $manager->directoryName($this->getTempTestDir().'/2');
    $this->assertEquals($this->getTempTestDir(), $directory);

  }

  public function testMakeDirectoryReturnsFalseOnExistingDirectory(): void {

    $manager = DiskIOManager::instance();
    $manager->makeDirectory($this->getTempTestDir().'/3', 0000, false);
    $success =
      $manager->makeDirectory($this->getTempTestDir().'/3', 0000, false);
    $this->assertFalse($success);

  }

  public function testUnlinkNonexistentFileReturnsFalse(): void {
    $manager = DiskIOManager::instance();
    $success = $manager->unlink($this->getTempTestDir().'/4');
    $this->assertFalse($success);
  }

  public function testFwriteWithValidHandleReturnsCorrectCount(): void {
    mkdir($this->getTempTestDir().'/5', 0777, true);
    $handle = fopen($this->getTempTestDir().'/5/1', 'w');

    $manager = DiskIOManager::instance();
    $result = $manager->fwrite($handle, 'test');

    // cleanup
    fclose($handle);
    unlink($this->getTempTestDir().'/5/1');
    rmdir($this->getTempTestDir().'/5');

    $this->assertEquals($result, 4);
  }

  public function testFCloseWithValidHanldeReturnsTrue(): void {
    mkdir($this->getTempTestDir().'/6', 0777, true);
    $handle = fopen($this->getTempTestDir().'/6', 'r');

    $manager = DiskIOManager::instance();
    $result = $manager->fclose($handle);

    $this->assertTrue($result);

    // cleanup: Just in case it failed to close
    if (!$result) {
      fclose($handle);
    }

    rmdir($this->getTempTestDir().'/6');
  }

  public function testIsReadableWithInvalidFilenameReturnsFalse(): void {
    $manager = DiskIOManager::instance();
    $result = $manager->isReadable($this->getTempTestDir().'/7');
    $this->assertFalse($result);
  }

  public function testIsWriteableWithInvalidFilenameReturnsFalse(): void {
    $manager = DiskIOManager::instance();
    $result = $manager->isReadable($this->getTempTestDir().'/8');
    $this->assertFalse($result);
  }

  public function testBzopenWithBadModeReturnsFalse(): void {

    $manager = DiskIOManager::instance();

    $handle = $manager->bzopen($this->getTempTestDir().'/9', 'y');

    if (is_bool($handle)) {
      $this->assertFalse($handle);
    } else {
      $this->fail('expected false return');
    }

  }

  public function testBzopenWithValidModeReturnsResource(): void {

    mkdir($this->getTempTestDir().'/10', 0777, true);

    $manager = DiskIOManager::instance();

    $handle = $manager->bzopen($this->getTempTestDir().'/10/1.tar.bz2', 'w');

    $this->assertTrue(is_resource($handle));

    if ($handle !== false) {
      bzclose($handle);
      unlink($this->getTempTestDir().'/10/1.tar.bz2');
    }

    rmdir($this->getTempTestDir().'/10');
  }

  public function testBzcloseWithValidHandleReturnsTrue(): void {

    mkdir($this->getTempTestDir().'/11', 0777, true);
    $handle = bzopen($this->getTempTestDir().'/11/1.tar.bz2', 'w');

    $manager = DiskIOManager::instance();
    $result = $manager->bzclose($handle);

    $this->assertTrue($result);

    unlink($this->getTempTestDir().'/11/1.tar.bz2');
    rmdir($this->getTempTestDir().'/11');

  }

  public function testCheckOrCreatePathWithNewPathReturnsTrue(): void {

    $manager = DiskIOManager::instance();

    $result =
      $manager->checkOrCreatePath($this->getTempTestDir().'/12', 0777);

    $this->assertTrue($result);

    rmdir($this->getTempTestDir().'/12');

  }

  public function testCheckOrCreatePathWithExistingPathReturnsTrue(): void {

    $manager = DiskIOManager::instance();

    $manager->checkOrCreatePath($this->getTempTestDir().'/13', 0777);

    $result =
      $manager->checkOrCreatePath($this->getTempTestDir().'/13', 0777);

    $this->assertTrue($result);

    rmdir($this->getTempTestDir().'/13');

  }

  public function testDeleteFileWithExistingFileReturnsTrue(): void {

    mkdir($this->getTempTestDir().'/14', 0777, true);
    touch($this->getTempTestDir().'/14/1');

    $manager = DiskIOManager::instance();

    $result = $manager->deleteFile($this->getTempTestDir().'/14/1');

    $this->assertTrue($result);

    rmdir($this->getTempTestDir().'/14');

  }

  public function testDeleteFileWithNonexistingFileReturnsTrue(): void {

    mkdir($this->getTempTestDir().'/15', 0777, true);

    $manager = DiskIOManager::instance();

    $result = $manager->deleteFile($this->getTempTestDir().'/15/1');

    $this->assertTrue($result);

    rmdir($this->getTempTestDir().'/15');

  }

  public function testFeofReturnsFalseForResourceWithMoreToRead(): void {

    mkdir($this->getTempTestDir().'/16', 0777, true);
    touch($this->getTempTestDir().'/16/1');
    $handle = fopen($this->getTempTestDir().'/16/1', 'wr');
    fwrite($handle, '12345');

    $manager = DiskIOManager::instance();

    $result = $manager->feof($handle);
    $this->assertFalse($result);

    unlink($this->getTempTestDir().'/16/1');
    rmdir($this->getTempTestDir().'/16');

  }

  public function testFgetsReturnsValidCountForResource(): void {

    mkdir($this->getTempTestDir().'/17', 0777, true);
    touch($this->getTempTestDir().'/17/1');
    $handle = fopen($this->getTempTestDir().'/17/1', 'w');
    fwrite($handle, '12345');
    fclose($handle);
    $handle = fopen($this->getTempTestDir().'/17/1', 'r');

    $manager = DiskIOManager::instance();

    $result = $manager->fgets($handle, 20);
    $this->assertEquals('12345', $result);

    fclose($handle);
    unlink($this->getTempTestDir().'/17/1');
    rmdir($this->getTempTestDir().'/17');

  }

  public function testBZWriteReturnsValidCountForResource(): void {

    mkdir($this->getTempTestDir().'/18', 0777, true);
    $handle = bzopen($this->getTempTestDir().'/18/1', 'w');

    $manager = DiskIOManager::instance();

    $result = $manager->bzwrite($handle, '1', 20);

    $this->assertEquals(1, $result);

    bzclose($handle);
    unlink($this->getTempTestDir().'/18/1');
    rmdir($this->getTempTestDir().'/18');

  }

  public function testWriteFileThrowsFailedToCreateDirectoryException(): void {
    $this->expectException(FailedToCreateDirectoryException::class);
    $manager = new ManagerWithFailedCheckOrCreatePath();
    $manager->writeFile('', '', 0777, true);
  }

  public function testWriteFileThrowsFailedToOpenFileException(): void {
    $this->expectException(FailedToOpenFileException::class);
    $manager = new ManagerWithFailedFileOpen();
    $manager->writeFile('', '', 0777, true);
  }

  public function testWriteFileThrowsFailedToWriteToFileException(): void {
    $this->expectException(FailedToWriteToFileException::class);
    $manager = new ManagerWithFailedToWriteToFile();
    $manager->writeFile('', '', 0777, true);
  }

  public function testWriteToFileWithIncorrectBytesThrowsFailedToWriteToFileException(
  ): void {
    $this->expectException(FailedToWriteToFileException::class);
    $manager = new ManagerWithFileWriteZeroBytes();
    $manager->writeFile('', 'asdf', 0777, true);
  }

  public function testWriteFileThrowsFailedToCloseFileException(): void {
    $this->expectException(FailedToCloseFileException::class);
    $manager = new ManagerWithFailedToCloseFile();
    $manager->writeFile('', '', 0777, true);
  }

  public function testBZip2ThrowsReadPermissionsException(): void {
    $this->expectException(ReadPermissionsException::class);
    $manager = new ManagerWithFailedReadPermissions();
    $manager->bzip2('', '');
  }

  public function testBZip2ThrowsWritePermissionsException(): void {
    $this->expectException(WritePermissionsException::class);
    $manager = new ManagerWithFailedWritePermissions();
    $manager->bzip2('', '');
  }

  public function testBZip2ThrowsFailedToOpenFileExceptionForFOpen(): void {
    $this->expectException(FailedToOpenFileException::class);
    $manager = new ManagerWithFailedFileOpen();
    $manager->bzip2('', '');
  }

  public function testBZip2ThrowsFailedToOpenFileExceptionForBZOpen(): void {
    $this->expectException(FailedToOpenFileException::class);
    $manager = new ManagerWithFailedBZOpen();
    $manager->bzip2('', '');
  }

  public function testBZip2ThrowsFailedToCloseFileExceptionForFclose(): void {
    $this->expectException(FailedToCloseFileException::class);
    $manager = new ManagerWithFeofFalseOnceFcloseFails();
    $manager->bzip2('', '');
  }

  public function testBZip2ThrowsFailedToCloseFileExceptionForBzclose(): void {
    $this->expectException(FailedToCloseFileException::class);
    $manager = new ManagerWithFeofFalseOnceFcloseFails();
    $manager->bzip2('', '');
  }

  public function testChown(): void {
    mkdir($this->getTempTestDir().'/19', 0777, true);
    $result =
      DiskIOManager::instance()
        ->chown($this->getTempTestDir().'/19', get_current_user());
    $this->assertTrue($result);
    rmdir($this->getTempTestDir().'/19');
  }

  public function testRecursivelyDeleteDirectoryForNestedDirectoriesReturnsCorrectCount(
  ): void {
    mkdir($this->getTempTestDir().'/20', 0777, true);
    touch($this->getTempTestDir().'/20/0.txt');
    mkdir($this->getTempTestDir().'/20/0', 0777, true);
    touch($this->getTempTestDir().'/20/0/0.txt');
    mkdir($this->getTempTestDir().'/20/0/0', 0777, true);
    mkdir($this->getTempTestDir().'/20/1', 0777, true);
    mkdir($this->getTempTestDir().'/20/1/0', 0777, true);
    mkdir($this->getTempTestDir().'/20/1/0/0', 0777, true);
    touch($this->getTempTestDir().'/20/1/0/0/0.txt');
    $this->assertTrue(file_exists($this->getTempTestDir().'/20'));
    $this->assertTrue(file_exists($this->getTempTestDir().'/20/0.txt'));
    $this->assertTrue(file_exists($this->getTempTestDir().'/20/0'));
    $this->assertTrue(file_exists($this->getTempTestDir().'/20/0/0.txt'));
    $this->assertTrue(file_exists($this->getTempTestDir().'/20/0/0'));
    $this->assertTrue(file_exists($this->getTempTestDir().'/20/1'));
    $this->assertTrue(file_exists($this->getTempTestDir().'/20/1/0'));
    $this->assertTrue(file_exists($this->getTempTestDir().'/20/1/0/0'));
    $this->assertTrue(file_exists($this->getTempTestDir().'/20/1/0/0/0.txt'));
    $result =
      DiskIOManager::instance()
        ->recursivelyDeleteDirectory($this->getTempTestDir().'/20');
    $this->assertEquals(9, $result);
    $this->assertFalse(file_exists($this->getTempTestDir().'/20'));
    $this->assertFalse(file_exists($this->getTempTestDir().'/20/0.txt'));
    $this->assertFalse(file_exists($this->getTempTestDir().'/20/0'));
    $this->assertFalse(file_exists($this->getTempTestDir().'/20/0/0.txt'));
    $this->assertFalse(file_exists($this->getTempTestDir().'/20/0/0'));
    $this->assertFalse(file_exists($this->getTempTestDir().'/20/1'));
    $this->assertFalse(file_exists($this->getTempTestDir().'/20/1/0'));
    $this->assertFalse(file_exists($this->getTempTestDir().'/20/1/0/0'));
    $this->assertFalse(
      file_exists($this->getTempTestDir().'/20/1/0/0/0.txt'),
    );
  }

  public function testRecursivelyDeleteDirectoryWithNonExistentPathReturns0(
  ): void {
    $result =
      DiskIOManager::instance()
        ->recursivelyDeleteDirectory($this->getTempTestDir().'/21');
    $this->assertEquals(0, $result);
  }

  public function testRecursivelyDeleteDirectoryWithRmdirFailureReturns0(
  ): void {
    mkdir($this->getTempTestDir().'/23', 0777, true);
    $manager = new ManagerWithRmdirFalse();
    $result =
      $manager->recursivelyDeleteDirectory($this->getTempTestDir().'/23');
    $this->assertEquals(0, $result);
    rmdir($this->getTempTestDir().'/23');
  }

  public function testDeleteWithNonZeroMtimeFilter(): void {
    $dir = $this->getTempTestDir().'/'.__FUNCTION__;
    mkdir($dir, 0777, true);
    $oneHourAgo = time() - 3600;
    touch("$dir/oldFileToBeDeleted.txt", $oneHourAgo);
    touch("$dir/recentlyCreatedFileShouldNotBeDeleted.txt");
    $result =
      DiskIOManager::instance()->recursivelyDeleteDirectory($dir, 300);

    $this->assertEquals(1, $result);
  }

  public function testDeleteWithNonZeroMtimeFilterWithSubDirs(): void {
    $dir = $this->getTempTestDir().'/'.__FUNCTION__;
    mkdir($dir, 0777, true);
    mkdir("$dir/subDir1", 0777, true);
    mkdir("$dir/subDir2", 0777, true);

    $oneHourAgo = time() - 3600;
    touch("$dir/subDir1/oldFileToBeDeleted.txt", $oneHourAgo);
    touch("$dir/subDir2/recentlyCreatedFileShouldNotBeDeleted.txt");
    $result =
      DiskIOManager::instance()->recursivelyDeleteDirectory($dir, 300);

    $this->assertEquals(1, $result);
  }

  public function testChownOnAlreadyOwnedDirectory(): void {
    mkdir($this->getTempTestDir().'/24', 0777, true);
    $result =
      DiskIOManager::instance()
        ->chown($this->getTempTestDir().'/24', get_current_user());
    $this->assertTrue($result);
    $result =
      DiskIOManager::instance()
        ->chown($this->getTempTestDir().'/24', get_current_user());
    $this->assertTrue($result);
    rmdir($this->getTempTestDir().'/24');
  }

  public function testTarballSucceeds(): void {
    mkdir($this->getTempTestDir().'/25/0', 0777, true);
    DiskIOManager::instance()->tarball(
      $this->getTempTestDir().'/25/0',
      $this->getTempTestDir().'/25/0.tar',
    );
    $this->assertTrue(file_exists($this->getTempTestDir().'/25/0.tar'));
    unlink($this->getTempTestDir().'/25/0.tar');
    rmdir($this->getTempTestDir().'/25/0');
    rmdir($this->getTempTestDir().'/25');
  }

  public function testTarballWithDoesFileExistFalseThrowsReadPermissionsException(
  ): void {
    $this->expectException(ReadPermissionsException::class);
    $manager = new ManagerWithDoesFileExistFalse();
    $manager->tarball('', '');
  }

  public function testTarballWithIsWriteableFalseAndDoesFileExistTrueAndIsReadableTrueThrowsWritePermissionsException(
  ): void {
    $this->expectException(WritePermissionsException::class);
    $manager =
      new ManagerWithIsWriteableFalseAndDoesFileExistTrueAndIsReadableTrue();
    $manager->tarball('', '');
  }

  public function testTarballWithQuotesInFileNamesThrowsInvalidFileNameException(
  ): void {
    $this->expectException(InvalidFileNameException::class);
    $manager =
      new ManagerWithIsWriteableTrueAndDoesFileExistTrueAndIsReadableTrue();
    $manager->tarball('\'', '\'');
  }

  public function testTarballWithExistingFileThrowsInvalidFileNameException(
  ): void {
    mkdir($this->getTempTestDir().'/26/0', 0777, true);
    touch($this->getTempTestDir().'/26/0.tar', 0777, true);
    try {
      $manager =
        new ManagerWithIsWriteableTrueAndDoesFileExistTrueAndIsReadableTrue();
      $manager->tarball(
        $this->getTempTestDir().'/26/0',
        $this->getTempTestDir().'/26/0.tar',
      );
      $this->fail();
    } catch (InvalidFileNameException $e) {
    }
    unlink($this->getTempTestDir().'/26/0.tar');
    rmdir($this->getTempTestDir().'/26/0');
    rmdir($this->getTempTestDir().'/26');
    $this->assertFalse(file_exists($this->getTempTestDir().'/26'));
  }

  public function testTarballWithEmptyFileNameThrowsFailedToWriteToFileException(
  ): void {
    mkdir($this->getTempTestDir().'/27/0', 0777, true);
    try {
      $manager =
        new ManagerWithIsWriteableTrueAndDoesFileExistTrueAndIsReadableTrue();
      $manager->tarball('', '');
      $this->fail();
    } catch (FailedToWriteToFileException $e) {
    }
    rmdir($this->getTempTestDir().'/27/0');
    rmdir($this->getTempTestDir().'/27');
    $this->assertFalse(file_exists($this->getTempTestDir().'/27'));
  }

  public function testManagerWithTarballValidFalseThrowsFailedToWriteToFileException(
  ): void {
    mkdir($this->getTempTestDir().'/28/0', 0777, true);
    try {
      $manager = new ManagerWithTarballValidFalse();
      $manager->tarball(
        $this->getTempTestDir().'/28/0',
        $this->getTempTestDir().'/28/0.tar',
      );
      $this->fail();
    } catch (FailedToWriteToFileException $e) {
    }
    $this->assertTrue(file_exists($this->getTempTestDir().'/28/0.tar'));
    unlink($this->getTempTestDir().'/28/0.tar');
    rmdir($this->getTempTestDir().'/28/0');
    rmdir($this->getTempTestDir().'/28');
  }

  public function testManagerTarballDoesNotExist(): void {
    $this->assertFalse(
      DiskIOManager::instance()
        ->isTarballValid('/dev/null/file-does-not-exist'),
    );
  }

  public function testManagerBzipWithFailedClose(): void {
    $this->expectException(FailedToCloseFileException::class);
    $manager = new ManagerWithFailedBZClose();
    $manager->bzip2($this->getTempTestDir().'/29.bz', '');
  }

  public function testChownFailure(): void {
    // --
    // On most secure unixes this will be false
    // --
    $testFile = $this->getTempTestDir().'/30.testfile';
    touch($testFile);
    $manager = DiskIOManager::instance();
    $this->assertFalse($manager->chown($testFile, "root"));
    unlink($testFile);
  }

  public function testBzipWriteFailureTrap(): void {

    $testFile = $this->getTempTestDir().'/31.testfile';

    $manager = DiskIOManager::instance();
    $fh = $manager->fileOpen($testFile, 'w');

    if (is_resource($fh)) {
      fclose($fh);
      $this->assertEquals(0, $manager->bzwrite($fh, 'some-data-here', 12));
    } else {
      $this->fail('setup task did not make a fh');
    }

    // cleanup
    unlink($testFile);

  }

  public function testScandirInvalidDir(): void {

    $testDir = $this->getTempTestDir().'/32';

    $manager = DiskIOManager::instance();
    $vec = $manager->scanDirectory($testDir);
    $this->assertEquals(0, $vec->count());

  }

  public function testManagerClear(): void {

    $this->assertInstanceOf(ManagerInterface::class, Manager::instance());
    $this->assertTrue(Manager::clear());
    $this->assertInstanceOf(ManagerInterface::class, Manager::instance());

  }
}
