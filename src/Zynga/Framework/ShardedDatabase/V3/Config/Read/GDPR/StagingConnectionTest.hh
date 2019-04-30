<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Read\GDPR;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Read\StagingConnectionTest as ReadStagingTest
;

class StagingConnectionTest extends ReadStagingTest {

  public function getDriverName(): string {
    return 'Read\GDPR';
  }

}
