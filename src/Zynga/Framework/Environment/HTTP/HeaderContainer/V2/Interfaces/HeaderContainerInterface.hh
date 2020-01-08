<?hh // strict

namespace Zynga\Framework\Environment\HTTP\HeaderContainer\V2\Interfaces;

interface HeaderContainerInterface {
  public function getHeader(string $header): string;
  public function setHeader(string $header, string $value): bool;
  public function noCaching(): bool;
  public function contentIsJSON(): bool;
  public function send(): bool;
}
