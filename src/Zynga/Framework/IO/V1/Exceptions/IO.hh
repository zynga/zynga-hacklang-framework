<?hh // strict

namespace Zynga\Framework\IO\V1\Exceptions;

use Zynga\Framework\Exception\V1\Exception;

/**
 * Base Input/Output exception from which all others derive.
 * Represents that there was an exception with an input or
 * output operation
 */
abstract class IO extends Exception {}
