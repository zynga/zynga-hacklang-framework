<?hh

require_once
  dirname(__FILE__).
  '/Zynga/Framework/AutoLoader/V2/ZyngaFrameworkAutoloader.hh'
;
require_once
  dirname(__FILE__).
  '/Zynga/Framework/AutoLoader/V2/ZyngaFrameworkAutoloader/Cache.hh'
;
require_once
  dirname(__FILE__).
  '/Zynga/Framework/AutoLoader/V2/ZyngaFrameworkAutoloader/Path.hh'
;

use Zynga\Framework\AutoLoader\V2\ZyngaFrameworkAutoLoader;

// --
// JEO: Sometimes you need to turn on debugging for the auto loader to see what is going on.
// --
// ZyngaFrameworkAutoLoader::enableDebug();

// --
// JEO: Similarly the ttl for the caching might get in the way if you are doing large refactors,
//      or file renames.
// --
ZyngaFrameworkAutoLoader::setCacheMisses(false);
ZyngaFrameworkAutoLoader::setCacheFounds(false);

// Add the path of the src directory.
ZyngaFrameworkAutoLoader::addPath(
  'Zynga\Framework',
  dirname(__FILE__),
  Vector {'hh'},
);

// Wire up the phpunit dependencies.
ZyngaFrameworkAutoLoader::addPath(
  'Zynga\PHPUnit',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'hh'},
);

// converted, but needs to be moved wihtin the Zynga\PHPUnit namespace.
ZyngaFrameworkAutoLoader::addPath(
  'Zynga\CodeBase',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'hh'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\Comparator',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'hh'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\Diff',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'hh'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\CodeCoverage',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'hh'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\Environment',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'hh'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\TokenStream',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'hh'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\Exporter',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'hh'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\TextTemplate',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'hh'},
);

// -- These all need conversion to hacklang proper.
ZyngaFrameworkAutoLoader::addPath(
  'PHPUnit_',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'php'},
);

ZyngaFrameworkAutoLoader::addPath(
  'File_Iterator',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'php'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\CodeUnitReverseLookup',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'php'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\Version',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'php'},
);

ZyngaFrameworkAutoLoader::addPath(
  'SebastianBergmann\RecursionContext',
  dirname(dirname(__FILE__)).'/vendor/zynga/phpunit/src',
  Vector {'php'},
);

// Registration of the autoloader.
ZyngaFrameworkAutoLoader::register();
