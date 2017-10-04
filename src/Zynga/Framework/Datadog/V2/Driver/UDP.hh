<?hh // strict

namespace Zynga\Framework\Datadog\V2\Driver;

use Zynga\Framework\Datadog\V2\Driver\Base as DriverBase;
use Zynga\Framework\Datadog\V2\ServiceStatus;

class UDP extends DriverBase {

  private ?resource $_socket;

  public function __destruct(): void {
    $this->closeSocket();
  }

  /**
   * Log timing information
   *
   * @param string $stat The metric to in log timing info for.
   * @param float $time The elapsed time in seconds to log
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param ?Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   **/
  public function timing(
    string $stat,
    float $time,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return $this->microTiming($stat, $time * 1000, $sampleRate, $tags);
  }

  /**
   * Recording the elapsed time from microtime(true) - microtime(true) = elapsed (f)
   *
   * @param string $stat The metric name
   * @param float $time The elapsed time in microseconds to log
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param array|string $tags Key Value array of Tag => Value, or single tag as string
   **/
  public function microTiming(
    string $stat,
    float $time,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    $data = Map {};
    $data[$stat] = "$time|ms";
    return $this->send($data, $sampleRate, $tags);
  }

  /**
   * Gauge
   *
   * @param string $stat The metric
   * @param float $value The value
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param array|string $tags Key Value array of Tag => Value, or single tag as string
   **/
  public function gauge(
    string $stat,
    float $value,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    $data = Map {};
    $data[$stat] = "$value|g";
    return $this->send($data, $sampleRate, $tags);
  }

  /**
   * Increments one stat counter
   *
   * @param string $stat The metric to increment.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,mixed> $tags Key Value array of Tag => Value
   * @return bool
   **/
  public function increment(
    string $stat,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return $this->updateStat($stat, 1, $sampleRate, $tags);
  }

  /**
   * Increments multiple stat counters
   *
   * @param Vector<string> $stats The metrics to increment.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,mixed> $tags Key Value array of Tag => Value
   * @return bool
   **/
  public function incrementStats(
    Vector<string> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return $this->updateStats($stats, 1, $sampleRate, $tags);
  }

  /**
   * Decrements one stat counter
   *
   * @param string $stat The metric to decrement.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,mixed> $tags Key Value array of Tag => Value
   * @return bool
   **/
  public function decrement(
    string $stat,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return $this->updateStat($stat, -1, $sampleRate, $tags);
  }

  /**
   * Decrements multiple stat counters
   *
   * @param Vector<string> $stats The metrics to decrements.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,mixed> $tags Key Value array of Tag => Value
   * @return bool
   **/
  public function decrementStats(
    Vector<string> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    return $this->updateStats($stats, -1, $sampleRate, $tags);
  }

  /**
   * Histogram
   *
   * @param string $stat The metric
   * @param float $value The value
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @return bool
   **/
  public function histogram(
    string $stat,
    float $value,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    $data = Map {};
    $data[$stat] = "$value|h";
    return $this->send($data, $sampleRate, $tags);
  }

  /**
   * Histogram
   *
   * @param Map<string,float> $stats The metrics you would like to update
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @return bool
   **/
  public function histogramStats(
    Map<string, float> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    $data = Map {};
    foreach ($stats as $stat => $value) {
      $data[$stat] = "$value|h";
    }
    return $this->send($data, $sampleRate, $tags);
  }

  /**
   * set
   *
   * @param string $stat The metric
   * @param float $value The value
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @return bool
   **/
  public function set(
    string $stat,
    float $value,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    $data = Map {};
    $data[$stat] = "$value|s";
    return $this->send($data, $sampleRate, $tags);
  }

  public function event(string $title, string $text): bool {
    $data = Map {};
    $stat = '_e{'.strlen($title).','.strlen($text).'}';
    $data[$stat] = "$title|$text";
    return $this->send($data, 1.0, null);
  }

  /**
   * setStats
   *
   * @param Map<string, float> $stats The metrics you would like to update
   * @param float $value The value
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @return bool
   **/
  public function setStats(
    Map<string, float> $stats,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    $data = Map {};
    foreach ($stats as $stat => $value) {
      $data[$stat] = "$value|s";
    }
    return $this->send($data, $sampleRate, $tags);
  }

  /**
   * Updates one or more stats counters by arbitrary amounts.
   *
   * @param string $stats The metric(s) to update. Should be either a string or array of metrics.
   * @param int $delta The amount to increment/decrement each metric by.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string   *
   * @return bool
   **/
  public function updateStat(
    string $stat,
    int $delta = 1,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    $data = Map {};
    $data[$stat] = $delta.'|c';
    return $this->send($data, $sampleRate, $tags);
  }

  /**
   * Updates multiple stats counters by arbitrary amounts.
   *
   * @param string $stats The metric(s) to update. Should be either a string or array of metrics.
   * @param int $delta The amount to increment/decrement each metric by.
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string   *
   * @return bool
   **/
  public function updateStats(
    Vector<string> $stats,
    int $delta = 1,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
  ): bool {
    $data = Map {};
    foreach ($stats as $stat) {
      $data[$stat] = $delta.'|c';
    }
    return $this->send($data, $sampleRate, $tags);
  }

  /**
   * Send a custom service check status.
   * @param string $name service check name
   * @param int $status service check status code (see static::OK, static::WARNING,...)
   * @param ?Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @param ?string $hostname hostname to associate with this service check status
   * @param ?string $message message to associate with this service check status
   * @param ?int $timestamp timestamp for the service check status (defaults to now)
   * @return bool
   **/
  public function serviceCheck(
    string $name,
    int $status,
    ?Map<string, string> $tags = null,
    ?string $hostname = null,
    ?string $message = null,
    ?int $timestamp = null,
  ): bool {

    if (ServiceStatus::isValidStatus($status) !== true) {
      return false;
    }

    // --
    // TODO: Again the escaping issue, this is getting kinda silly, they should
    // have a escaper that is generic.
    // --
    $msg = "_sc|$name|$status";

    if ($timestamp !== null) {
      $msg .= sprintf("|d:%s", $timestamp);
    }

    if ($hostname !== null) {
      $msg .= sprintf("|h:%s", $hostname);
    }

    if ($tags !== null && $tags->count() > 0) {
      $tagString = '';
      $isFirst = true;
      foreach ($tags as $key => $value) {
        // --
        // TODO: JEO- I am pretty sure the previous developers haven't allowed
        // for tags being something that breaks the wire protocol. Writing this
        // loop such we could insert escaping of the key and value pairs.
        // --
        if ($isFirst !== true) {
          $tagString .= ',';
        }
        $tagString .= $key.','.$value;
        $isFirst = false;
      }
      $msg .= sprintf('|#%s', $tagString);
    }
    if ($message !== null) {
      $msg .= sprintf(
        '|m:%s',
        str_replace("m:", "m\:", str_replace("\n", "\\n", $msg)),
      );
    }
    return $this->report($msg);
  }

  /**
   * Squirt the metrics over UDP
   * @param array $data Incoming Data
   * @param float $sampleRate the rate (0-1) for sampling.
   * @param Map<string,string> $tags Key Value array of Tag => Value, or single tag as string
   * @return bool
   **/
  public function send(
    Map<string, string> $data,
    float $sampleRate = 1.0,
    ?Map<string, string> $tags = null,
    bool $forceSample = false,
  ): bool {

    // client side sampling
    $sampledData = Map {};

    if ($sampleRate < 1) {
      foreach ($data as $stat => $value) {
        if ($this->shouldSample($sampleRate, $forceSample)) {
          $sampledData[$stat] = "$value|@$sampleRate";
        }
      }
    } else {
      $sampledData = $data;
    }

    if ($sampledData->count() == 0) {
      return false;
    }

    $tagString = '';

    if ($tags !== null && $tags->count() > 0) {
      // --
      // TODO: JEO- I am pretty sure the previous developers haven't allowed
      // for tags being something that breaks the wire protocol. Writing this
      // loop such we could insert escaping of the key and value pairs.
      // --
      // append the tag values to the wire protocol.
      foreach ($tags as $tag_key => $tag_val) {
        $tagString .= '#'.$tag_key.':'.$tag_val.',';
      }

      $tagString = substr($tagString, 0, -1);
    }

    foreach ($sampledData as $stat => $value) {
      if ($tagString !== '') {
        $value .= '|'.$tagString;
      }

      // send the metric out.
      $this->reportMetric("$stat:$value");
    }

    return true;

  }

  public function report(string $udp_message): bool {
    return $this->flush($udp_message);
  }

  public function reportMetric(string $udp_message): bool {
    return $this->report($udp_message);
  }

  public function openSocket(): bool {

    if (is_resource($this->_socket)) {
      return true;
    }

    // stand up the socket.
    $socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);

    // failed to set the socket to non-blocking.
    socket_set_nonblock($socket);

    $this->_socket = $socket;

    return true;

  }

  public function closeSocket(): bool {
    if (is_resource($this->_socket) === true) {
      socket_close($this->_socket);
      $this->_socket = null;
      return true;
    }
    return false;
  }

  public function flush(string $udp_message): bool {
    // --
    // JEO: Currently while php interacts with a udp socket it always returns
    // true state, so therefor this if statement never hits false.
    // --
    //if ( $this->openSocket() !== true ) {
    //  return false;
    //}
    $this->openSocket();
    $config = $this->getConfig();
    $server = $config->getServerHostname();
    $port = $config->getServerPort();
    $messageLength = strlen($udp_message);
    $sentBytes = socket_sendto(
      $this->_socket,
      $udp_message,
      $messageLength,
      0,
      $server,
      $port,
    );
    return true;
  }
}
