<?hh

require_once dirname(dirname(__FILE__)).'/include/path.hh';

use Zynga\Framework\Testing\BootStrap\V2\BootStrap;
use Zynga\Framework\Testing\TestCase\V2\Config\Manager as TestCaseManager;
use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;

// Stand up the bootstrap for all of our tests.
BootStrap::init();

// Detect what development enviornment we are within.
DevelopmentMode::detect();
