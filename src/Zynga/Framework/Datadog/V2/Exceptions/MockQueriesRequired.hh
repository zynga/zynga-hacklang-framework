<?hh // strict

namespace Zynga\Framework\Datadog\V2\Exceptions;

use Zynga\Framework\Exception\V1\Exception;

/**
 * Exception encapsulating the concept that mock queries need to be implemented for a data dog driver
 * but none have been provided
 **/
class MockQueriesRequired extends Exception {}
