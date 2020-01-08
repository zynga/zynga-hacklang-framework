<?hh // strict

namespace Zynga\Framework\Logging\V1;

class Level {
  const int CRITICAL  = 32;
  const int ERROR     = 16;
  const int WARNING   = 8; 
  const int INFO      = 4;
  const int DEBUG     = 2;

  public static function getLevelAsString(int $level): string {

    if ( $level === self::CRITICAL ) { 
      return 'CRITICAL';
    } else if ( $level === self::ERROR ) {
      return 'ERROR';
    } else if ( $level === self::WARNING ) {
      return 'WARNING';
    } else if ( $level === self::INFO ) {
      return 'INFO';
    } else if ( $level === self::DEBUG ) {
      return 'DEBUG';
    }

    return 'UNKNOWN';

  }

}
