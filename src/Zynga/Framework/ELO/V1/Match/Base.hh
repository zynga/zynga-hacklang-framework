<?hh // strict

namespace Zynga\Framework\ELO\V1\Match;

/**
 * Base class to encapsulate the concept of a elo rated pvp match.
 *
 * @package     casino-platform
 * @copyright   Zynga, Inc.
 * @author      Jason Orcutt <jorcutt@zynga.com>
 */
abstract class Base {

  /**
   * _p1_rating: Player 1 rating
   */
  private float $_p1_rating;

  /**
   * _p2_rating: Player 2 rating
   */
  private float $_p2_rating;

  /**
   * _p1_score: Player 1 score on this match.
   */
  private float $_p1_score;

  /**
   * _p2_score: Player 2 score on this match.
   */
  private float $_p2_score;

  /**
   * __construct
   *
   * @access public
   * @return \\Zynga\ELO\V1\Match\Base
   */
  public function __construct() {

    $this->_p1_rating = 0.0;
    $this->_p2_rating = 0.0;

    $this->_p1_score = 0.0;
    $this->_p2_score = 0.0;

  }

  /**
   * init: Initializes all of the attributes about a match.
   *
   * @access public
   * @return void
   */
  public function init(): void {

    $this->_p1_rating = 0.0;
    $this->_p1_score = 0.0;

    $this->_p2_rating = 0.0;
    $this->_p2_score = 0.0;

  }

  /**
   * getPlayer1Rating: Fetches the current rating for Player 1
   *
   * @access public
   * @return float
   */
  public function getPlayer1Rating(): float {
    return $this->_p1_rating;
  }

  /**
   * setPlayer1Rating: Sets the current rating for Player 1
   *
   * @access public
   * @param float $rating
   * @return bool
   */
  public function setPlayer1Rating(float $rating): bool {
    $this->_p1_rating = $rating;
    return true;
  }

  /**
   * setPlayer2Rating: Sets the current rating for Player 2
   *
   * @access public
   * @param float $rating
   * @return bool
   */
  public function setPlayer2Rating(float $rating): bool {
    $this->_p2_rating = $rating;
    return true;
  }

  /**
   * getPlayer2Rating: Fetches the current rating for Player 2
   *
   * @access public
   * @return float
   */
  public function getPlayer2Rating(): float {
    return $this->_p2_rating;
  }

  /**
   * setPlayer1Score: Sets the score for player 1 on the match.
   *
   * @access public
   * @param float $score
   * @return bool
   */
  public function setPlayer1Score(float $score): bool {
    $this->_p1_score = $score;
    return true;
  }

  /**
   * getPlayer1Score: Gets the score for player 1 on the match.
   *
   * @access public
   * @return float
   */
  public function getPlayer1Score(): float {
    return $this->_p1_score;
  }

  /**
   * setPlayer2Score: Sets the score for player 2 on the match.
   *
   * @access public
   * @param float $score
   * @return bool
   */
  public function setPlayer2Score(float $score): bool {
    $this->_p2_score = $score;
    return true;
  }

  /**
   * getPlayer2Score: Gets the score for player 2 on the match.
   *
   * @access public
   * @return float
   */
  public function getPlayer2Score(): float {
    return $this->_p2_score;
  }

  /**
   * getInitialRating: What is the default value for users in this algo.
   *
   * @abstract
   * @access public
   * @return float
   */
  abstract public function getInitialRating(): float;

  /**
   * See https://wiki.corp.zynga.com/display/POKER/Poker+Player+Rating,
   * In that example, the transform value is Starting Score / 3
   *
   * @return float The transform value used for calculating k
   */
  abstract public function getTransformValue(): float;

  /**
   * getK: Calculates the K value for a specific player.
   *
   * @param mixed $player
   * @access public
   * @return float
   */
  abstract public function getK(): float;

  /**
   * calcELO: Calculates the ELO for both players.
   *
   * @access public
   * @return hash
   */
  abstract public function calcELO(): Vector<float>;

}
