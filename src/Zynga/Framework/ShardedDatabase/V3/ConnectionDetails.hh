<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3;

class ConnectionDetails {

  private string $_username;
  private string $_password;
  private string $_hostname;
  private int $_port;

  public function __construct(
    string $username,
    string $password,
    string $hostname,
    int $port,
  ) {
    $this->_username = $username;
    $this->_password = $password;
    $this->_hostname = $hostname;
    $this->_port = $port;
  }

  public function getUsername(): string {
    return $this->_username;
  }

  public function getPassword(): string {
    return $this->_password;
  }

  public function setPassword(string $password): bool {
    $this->_password = $password;
    return true;
  }

  public function getHostname(): string {
    return $this->_hostname;
  }

  public function getPort(): int {
    return $this->_port;
  }

}
