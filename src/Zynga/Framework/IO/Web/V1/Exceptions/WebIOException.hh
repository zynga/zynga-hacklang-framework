<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Exceptions;

use Zynga\Framework\IO\V1\Exceptions\IO as IOException;

/**
 * Base Web Input/Output exception from which all others derive.
 * Represents that there was an exception with a web input or
 * output operation
 */
abstract class WebIOException extends IOException {}
