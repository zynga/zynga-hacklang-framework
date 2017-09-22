<?hh // strict

namespace Zynga\Framework\ELO\V1\Chess;

use Zynga\Framework\ELO\V1\Match\Base;

/**
 * Class embodying poker's Elo match
 *
 * @package     casino-platform
 * @copyright   Zynga, Inc.
 * @author      Jason Orcutt <jorcutt@zynga.com>
 */
class Match extends Base {

  /**
   * p1_transformed_rating
   */
  private float $_p1_transformed_rating;

  /**
   * p2_transformed_rating
   */
  private float $_p2_transformed_rating;

  /**
   * p1_expected_score
   */
  private float $_p1_expected_score;

  /**
   * p2_expected_score
   */
  private float $_p2_expected_score;

  /**
   * p1_new_rating: The calculated new rating for player 1.
   */
  private float $_p1_new_rating;

  /**
   * p2_new_rating: The calculated new rating for player 2.
   */
  private float $_p2_new_rating;

  public function __construct() {

    parent::__construct();

    $this->_p1_transformed_rating = 0.0;
    $this->_p2_transformed_rating = 0.0;

    $this->_p1_expected_score = 0.0;
    $this->_p2_expected_score = 0.0;

    $this->_p1_new_rating = 0.0;
    $this->_p2_new_rating = 0.0;

  }

  public function init(): void {

    parent::init();

    $this->_p1_transformed_rating = 0.0;
    $this->_p2_transformed_rating = 0.0;

    $this->_p1_expected_score = 0.0;
    $this->_p2_expected_score = 0.0;

    $this->_p1_new_rating = 0.0;
    $this->_p2_new_rating = 0.0;

  }

  /**
   * See https://wiki.corp.zynga.com/display/POKER/Poker+Player+Rating,
   * In that example, the transform value is Starting Score / 3
   *
   * @return integer The transform value used for calculating k
   */
  public function getTransformValue(): float {
    return (float) ($this->getInitialRating() / 3);
  }

  /**
   * See https://wiki.corp.zynga.com/display/POKER/Poker+Player+Rating,
   * In that example, the transformed rating is A(1)
   *
   * @param integer $player Player number to calculate the transformed rating of
   * @return float Transformed player rating used for calculating k
   */
  public function calculateTransformedRating(float $rating): float {
    $tr = pow(10, (max($rating, 1) / $this->getTransformValue()));
    return $tr;
  }

  /**
   * setPlayer1TransformedRating: Sets the transformed rating to a value, unit testing scaffolding.
   * @param float $tr
   * @return bool
   */
  public function setPlayer1TransformedRating(float $tr): bool {
    $this->_p1_transformed_rating = $tr;
    return true;
  }

  /**
   * getPlayer1TransformedRating: Calculates the transformed rating
   *
   * @return float The transformed rating for the user.
   */
  public function getPlayer1TransformedRating(): float {

    // check if we have already calced it.
    if ($this->_p1_transformed_rating != 0.0) {
      return $this->_p1_transformed_rating;
    }

    // calc it
    $this->_p1_transformed_rating =
      $this->calculateTransformedRating($this->getPlayer1Rating());
    return $this->_p1_transformed_rating;

  }

  /**
   * setPlayer2TransformedRating: Sets the transformed rating to a value, unit testing scaffolding.
   * @param float $tr
   * @return bool
   */
  public function setPlayer2TransformedRating(float $tr): bool {
    $this->_p2_transformed_rating = $tr;
    return true;
  }

  /**
   * getPlayer2TransformedRating: Calculates the transformed rating.
   *
   * @return float The transformed rating for the user.
   */
  public function getPlayer2TransformedRating(): float {

    // check if we have already calced it.
    if ($this->_p2_transformed_rating != 0.0) {
      return $this->_p2_transformed_rating;
    }

    // calc it
    $this->_p2_transformed_rating =
      $this->calculateTransformedRating($this->getPlayer2Rating());
    return $this->_p2_transformed_rating;
  }

  /**
   * Allows us to override the expected score for a user, useful for unit tests.
   * @param float $es The expected score value you want
   * @return bool
   */
  public function setPlayer1ExpectedScore(float $es): bool {
    $this->_p1_expected_score = $es;
    return true;
  }

  /**
   * getPlayer1ExpectedScore: Calculates the player 1 expected score.
   *
   * @return float
   */
  public function getPlayer1ExpectedScore(): float {

    if ($this->_p1_expected_score !== 0.0) {
      return $this->_p1_expected_score;
    }

    // calc it
    $tr1 = $this->getPlayer1TransformedRating();
    $tr2 = $this->getPlayer2TransformedRating();

    $es = 0.0;
    if (($tr1 + $tr2) == 0.0) {
      $es = 0.0;
    } else {
      $es = $tr1 / ($tr1 + $tr2);
    }

    $this->_p1_expected_score = $es;

    return $this->_p1_expected_score;

  }

  /**
   * Allows us to override the expected score for a user, useful for unit tests.
   * @param float $es The expected score value you want
   * @return bool
   */
  public function setPlayer2ExpectedScore(float $es): bool {
    $this->_p2_expected_score = $es;
    return true;
  }

  /**
   * getPlayer2ExpectedScore: Calculates the player 2 expected score.
   *
   * @return float
   */
  public function getPlayer2ExpectedScore(): float {

    if ($this->_p2_expected_score !== 0.0) {
      return $this->_p2_expected_score;
    }

    $tr1 = $this->getPlayer1TransformedRating();
    $tr2 = $this->getPlayer2TransformedRating();

    $es = 0.0;
    if (($tr1 + $tr2) == 0.0) {
      $es = 0.0;
    } else {
      $es = $tr2 / ($tr1 + $tr2);
    }

    $this->_p2_expected_score = $es;

    return $this->_p2_expected_score;

  }

  /**
   * setPlayer1NewRating: sets the player 1 new rating value.
   *
   * @param mixed $r
   * @return boolean
   */
  public function setPlayer1NewRating(float $rating): bool {
    $this->_p1_new_rating = $rating;
    return true;
  }

  /**
   * getPlayer1NewRating: gets the player 1 new rating value.
   *
   * @return float
   */
  public function getPlayer1NewRating(): float {
    return $this->_p1_new_rating;
  }

  /**
   * setPlayer2NewRating: sets the player 2 new rating value.
   *
   * @param mixed $r
   * @return boolean
   */
  public function setPlayer2NewRating(float $rating): bool {
    $this->_p2_new_rating = $rating;
    return true;
  }

  /**
   * getPlayer2NewRating: gets the player 2 new rating value.
   *
   * @return float
   */
  public function getPlayer2NewRating(): float {
    return $this->_p2_new_rating;
  }

  /**
   * getK: Calculates the K value for a specific pair of players.
   *
   * @return float
   */
  public function getK(): float {
    // https://en.wikipedia.org/wiki/Elo_rating_system#Most_accurate_K-factor
    return 32.0;
  }

  /**
   * calcELO: Calculates the ELO for both players.
   *
   * @return hash
   */
  public function calcELO(): Vector<float> {

    $r1 = $this->getPlayer1NewRating();
    $r2 = $this->getPlayer2NewRating();

    if ($r1 !== 0.0 && $r2 !== 0.0) {
      $vec = Vector {};
      $vec[] = $this->getPlayer1NewRating();
      $vec[] = $this->getPlayer2NewRating();
      return $vec;
    }

    $k = $this->getK();

    $r1_Old = $this->getPlayer1Rating();
    $p1_s = $this->getPlayer1Score();
    $p1_es = $this->getPlayer1ExpectedScore();

    $r2_Old = $this->getPlayer2Rating();
    $p2_s = $this->getPlayer2Score();
    $p2_es = $this->getPlayer2ExpectedScore();

    // echo "p1k=$p1k p2k=$p2k\n";

    // --
    // calc p1 change
    // JEO: Broke this down into multiple variables to allow us to debug the steps within.
    // (old one liner) $r1 = $r1_Old + $k * ($p1_s - $p1_es);
    // --
    $p1_ses = $p1_s - $p1_es;
    $p1_k_ses = $k * $p1_ses;
    $r1 = $r1_Old + $p1_k_ses;
    // If $r1 < 0, it's corrected later on along with $r2.
    // If $r1 == 0, it means there was actually no change
    if ($r1 > 0.0) {
      $r1 = max(0.01, $r1);
    }
    $r1 = round($r1, 2);

    //echo "r1_Old=${r1_Old} k=${k} p1_s=${p1_s} p1_es=${p1_es} p1_ses=${p1_ses} p1_k_ses=${p1_k_ses} r1=$r1\n";

    // --
    // calc p2 change
    // JEO: Broke this down into multiple variables to allow us to debug the steps within.
    // (old one liner) $r2 = $r2_Old + $k * ($p2_s - $p2_es);
    $p2_ses = $p2_s - $p2_es;
    $p2_k_ses = $k * $p2_ses;
    $r2 = $r2_Old + $p2_k_ses;
    // If $r2 < 0, it's corrected later on along with $r1.
    // If $r2 == 0, it means there was actually no change
    if ($r2 > 0.0) {
      $r2 = max(0.01, $r2);
    }
    $r2 = round($r2, 2);

    // echo "r2_Old=${r2_Old} k=${k} p2_s=${p2_s} p2_es=${p2_es} p2_ses=${p2_ses} p2_k_ses=${p2_k_ses} r2=$r2\n";

    // This prevents a player from getting a negative rating.
    // It also prevents a player from gaining ratings from a change that would
    // make the loser have a negative rating.
    if ($r1 < 0.0) {
      $r2 = $r2 + $r1;
    } else if ($r2 < 0.0) {
      $r1 = $r1 + $r2;
    }
    $r1 = max($r1, 0.0);
    $r2 = max($r2, 0.0);
    $this->setPlayer1NewRating($r1);
    $this->setPlayer2NewRating($r2);

    $vec = Vector {};
    $vec[] = $this->getPlayer1NewRating();
    $vec[] = $this->getPlayer2NewRating();
    return $vec;

  }

  /**
   * getInitialRating: Gets the initial score for this rating system.
   *
   * @return void
   */
  public function getInitialRating(): float {
    return 1200.0;
  }

}
