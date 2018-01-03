<?hh //strict

namespace Zynga\ELO\V1\Chess;

use Zynga\Framework\ELO\V1\Chess\Match;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class MatchTest extends TestCase {
  public function testCreateChessMatch(): void {
    $result = new Match();
    $this->assertEquals(
      'Zynga\Framework\ELO\V1\Chess\Match',
      get_class($result),
    );
  }

  public function testUnitializedPlayer1Rating(): void {
    $match = new Match();
    $player1Rating = $match->getPlayer1Rating();
    $this->assertTrue(is_float($player1Rating));
    $this->assertEquals(0.0, $player1Rating);
  }

  public function testUnitializedPlayer2Rating(): void {
    $match = new Match();
    $player2Rating = $match->getPlayer2Rating();
    $this->assertTrue(is_float($player2Rating));
    $this->assertEquals(0.0, $player2Rating);
  }

  public function testInitialPlayerRating(): void {
    $match = new Match();
    $initialRating = $match->getInitialRating();
    $this->assertTrue(is_float($initialRating));
    $this->assertEquals(1200.0, $initialRating);
  }

  public function testInitialPlayer1Score(): void {
    $match = new Match();
    $player1Score = $match->getPlayer1Score();
    $this->assertTrue(is_float($player1Score));
    $this->assertEquals(0.0, $player1Score);
  }

  public function testInitialPlayer2Score(): void {
    $match = new Match();
    $player2Score = $match->getPlayer2Score();
    $this->assertTrue(is_float($player2Score));
    $this->assertEquals(0.0, $player2Score);
  }

  public function testSetPlayer1Score(): void {
    $match = new Match();
    $player1FirstSetScore = 0.0;
    $match->setPlayer1Score($player1FirstSetScore);
    $player1FirstGetScore = $match->getPlayer1Score();
    $player1SecondSetScore = 1.0;
    $match->setPlayer1Score($player1SecondSetScore);
    $player1SecondGetScore = $match->getPlayer1Score();
    $this->assertEquals($player1FirstSetScore, $player1FirstGetScore);
    $this->assertEquals($player1SecondSetScore, $player1SecondGetScore);
  }

  public function testSetPlayer2Score(): void {
    $match = new Match();
    $player2FirstSetScore = 0.0;
    $match->setPlayer2Score($player2FirstSetScore);
    $player2FirstGetScore = $match->getPlayer2Score();
    $player2SecondSetScore = 1.0;
    $match->setPlayer2Score($player2SecondSetScore);
    $player2SecondGetScore = $match->getPlayer2Score();
    $this->assertEquals($player2FirstSetScore, $player2FirstGetScore);
    $this->assertEquals($player2SecondSetScore, $player2SecondGetScore);
  }

  public function testSettingPlayer2ScoreDoesNotAffectPlayer1Score(): void {
    $match = new Match();
    $player2FirstSetScore = 0.0;
    $match->setPlayer2Score($player2FirstSetScore);
    $player2FirstGetScore = $match->getPlayer2Score();
    $player1FirstSetScore = 0.0;
    $match->setPlayer1Score($player1FirstSetScore);
    $player1FirstGetScore = $match->getPlayer1Score();
    $player2SecondSetScore = 1.0;
    $match->setPlayer2Score($player2SecondSetScore);
    $player2SecondGetScore = $match->getPlayer2Score();
    $player1SecondGetScore = $match->getPlayer1Score();
    $this->assertEquals($player2FirstSetScore, $player2FirstGetScore);
    $this->assertEquals($player2SecondSetScore, $player2SecondGetScore);
    $this->assertEquals($player1FirstSetScore, $player1FirstGetScore);
    $this->assertEquals($player1FirstSetScore, $player1SecondGetScore);
  }

  public function testSettingPlayer1ScoreDoesNotAffectPlayer2Score(): void {
    $match = new Match();
    $player1FirstSetScore = 0.0;
    $match->setPlayer2Score($player1FirstSetScore);
    $player1FirstGetScore = $match->getPlayer1Score();
    $player2FirstSetScore = 0.0;
    $match->setPlayer1Score($player2FirstSetScore);
    $player2FirstGetScore = $match->getPlayer2Score();
    $player1SecondSetScore = 1.0;
    $match->setPlayer1Score($player1SecondSetScore);
    $player1SecondGetScore = $match->getPlayer1Score();
    $player2SecondGetScore = $match->getPlayer2Score();
    $this->assertEquals($player1FirstSetScore, $player1FirstGetScore);
    $this->assertEquals($player1SecondSetScore, $player1SecondGetScore);
    $this->assertEquals($player2FirstSetScore, $player2FirstGetScore);
    $this->assertEquals($player2FirstSetScore, $player2SecondGetScore);
  }

  public function testGetK(): void {
    $match = new Match();
    // --
    // JEO: The getK functionality was refactored, to be getK() no args.
    // leaving this code here jic we need to go backwards.
    // $initialRating = $match->getInitialRating();
    // $k = $match->getK($initialRating, $initialRating);
    //--
    $k = $match->getK();
    $this->assertTrue(is_float($k));
    $this->assertEquals(32.0, $k);
  }

  public function testGetTransformValue(): void {
    $match = new Match();
    $transformValue = $match->getTransformValue();
    $expectedTransformValue = $match->getInitialRating() / 3;
    $this->assertTrue(is_float($transformValue));
    $this->assertEquals($expectedTransformValue, $transformValue);
  }

  public function testCalcEloPlayer1Rating1200Win(): void {
    $match = new Match();
    $match->setPlayer1Rating(1200.0);
    $match->setPlayer2Rating(1200.0);
    $match->setPlayer1Score(1.0);
    $match->setPlayer2Score(0.0);
    $result = $match->calcELO();
    $this->assertEquals(1216.0, $result[0]);
    $this->assertEquals(1184.0, $result[1]);
  }

  public function testCalcEloPlayer2Rating1200Win(): void {
    $match = new Match();
    $match->setPlayer1Rating(1200.0);
    $match->setPlayer2Rating(1200.0);
    $match->setPlayer1Score(0.0);
    $match->setPlayer2Score(1.0);
    $result = $match->calcELO();
    $this->assertEquals(1184.0, $result[0]);
    $this->assertEquals(1216.0, $result[1]);
  }

  public function testCalcEloPlayer1Rating0Win(): void {
    $match = new Match();
    $match->setPlayer1Rating(0.0);
    $match->setPlayer2Rating(1200.0);
    $match->setPlayer1Score(1.0);
    $match->setPlayer2Score(0.0);
    $result = $match->calcELO();
    $this->assertEquals(31.97, $result[0]);
    $this->assertEquals(1168.03, $result[1]);
  }

  public function testCalcEloPlayer1Rating0Loss(): void {
    $match = new Match();
    $match->setPlayer1Rating(0.0);
    $match->setPlayer2Rating(1200.0);
    $match->setPlayer1Score(0.0);
    $match->setPlayer2Score(1.0);
    $result = $match->calcELO();
    $this->assertEquals(0.0, $result[0]);
    $this->assertEquals(1200.0, $result[1]);
  }

  public function testCalcEloPlayer2Rating0Win(): void {
    $match = new Match();
    $match->setPlayer1Rating(1200.0);
    $match->setPlayer2Rating(0.0);
    $match->setPlayer1Score(1.0);
    $match->setPlayer2Score(0.0);
    $result = $match->calcELO();
    $this->assertEquals(1200.0, $result[0]);
    $this->assertEquals(0.0, $result[1]);
  }

  public function testCalcEloPlayer2Rating0Loss(): void {
    $match = new Match();
    $match->setPlayer1Rating(1200.0);
    $match->setPlayer2Rating(0.0);
    $match->setPlayer1Score(0.0);
    $match->setPlayer2Score(1.0);
    $result = $match->calcELO();
    $this->assertEquals(1168.03, $result[0]);
    $this->assertEquals(31.97, $result[1]);
  }

  public function testCalcEloPlayer1Rating1Loss(): void {
    $match = new Match();
    $match->setPlayer1Rating(1.0);
    $match->setPlayer2Rating(1200.0);
    $match->setPlayer1Score(0.0);
    $match->setPlayer2Score(1.0);
    $result = $match->calcELO();
    $this->assertEquals(0.97, $result[0]);
    $this->assertEquals(1200.03, $result[1]);
    // run it again just to make sure the values don't change due to running
    // twice.
    $result = $match->calcELO();
    $this->assertEquals(0.97, $result[0]);
    $this->assertEquals(1200.03, $result[1]);
  }

  public function testCalculateTransformedRating(): void {
    $match = new Match();
    $tranformedRating1 = $match->calculateTransformedRating(0.0);
    $tranformedRating2 = $match->calculateTransformedRating(0.01);
    $tranformedRating3 = $match->calculateTransformedRating(1.0);
    $tranformedRating4 = $match->calculateTransformedRating(1200.0);
    $tranformedRating5 = $match->calculateTransformedRating(2500.0);
    $this->assertEquals(1.005773063, round($tranformedRating1, 9));
    $this->assertEquals(1.005773063, round($tranformedRating2, 9));
    $this->assertEquals(1.005773063, round($tranformedRating3, 9));
    $this->assertEquals(1000.0, $tranformedRating4);
    $this->assertEquals(1778279.41, round($tranformedRating5, 2));
  }

  public function testGetPlayer1ExpectedScore(): void {
    $match = new Match();
    $match->setPlayer1Rating(1.0);
    $match->setPlayer2Rating(1200.0);
    $player1ExpectedScore = $match->getPlayer1ExpectedScore();
    $this->assertEquals(0.0010047625, $player1ExpectedScore);
  }

  public function testGetPlayer2ExpectedScore(): void {
    $match = new Match();
    $match->setPlayer1Rating(1.0);
    $match->setPlayer2Rating(1200.0);
    $player2ExpectedScore = $match->getPlayer2ExpectedScore();
    $this->assertEquals(0.9989952375, $player2ExpectedScore);
  }

  public function testGetPlayer1TransformedRating(): void {
    $match = new Match();
    $match->setPlayer1Rating(0.0);
    $tranformedRating1 = $match->getPlayer1TransformedRating();
    $match = new Match();
    $match->setPlayer1Rating(0.01);
    $tranformedRating2 = $match->getPlayer1TransformedRating();
    $match = new Match();
    $match->setPlayer1Rating(1.0);
    $tranformedRating3 = $match->getPlayer1TransformedRating();
    $match = new Match();
    $match->setPlayer1Rating(1200.0);
    $tranformedRating4 = $match->getPlayer1TransformedRating();
    $match = new Match();
    $match->setPlayer1Rating(2500.0);
    $tranformedRating5 = $match->getPlayer1TransformedRating();
    $this->assertEquals(1.005773063, round($tranformedRating1, 9));
    $this->assertEquals(1.005773063, round($tranformedRating2, 9));
    $this->assertEquals(1.005773063, round($tranformedRating3, 9));
    $this->assertEquals(1000.0, $tranformedRating4);
    $this->assertEquals(1778279.41, round($tranformedRating5, 2));
  }

  public function testGetPlayer2TransformedRating(): void {
    $match = new Match();
    $match->setPlayer2Rating(0.0);
    $tranformedRating1 = $match->getPlayer2TransformedRating();
    $match = new Match();
    $match->setPlayer2Rating(0.01);
    $tranformedRating2 = $match->getPlayer2TransformedRating();
    $match = new Match();
    $match->setPlayer2Rating(1.0);
    $tranformedRating3 = $match->getPlayer2TransformedRating();
    $match = new Match();
    $match->setPlayer2Rating(1200.0);
    $tranformedRating4 = $match->getPlayer2TransformedRating();
    $match = new Match();
    $match->setPlayer2Rating(2500.0);
    $tranformedRating5 = $match->getPlayer2TransformedRating();
    $this->assertEquals(1.005773063, round($tranformedRating1, 9));
    $this->assertEquals(1.005773063, round($tranformedRating2, 9));
    $this->assertEquals(1.005773063, round($tranformedRating3, 9));
    $this->assertEquals(1000.0, $tranformedRating4);
    $this->assertEquals(1778279.41, round($tranformedRating5, 2));
  }

  public function testSetAndGetPlayer1NewRating(): void {
    $match = new Match();
    $player1FirstSetNewRating = 0.0;
    $match->setPlayer1NewRating($player1FirstSetNewRating);
    $player1FirstGetNewRating = $match->getPlayer1NewRating();
    $player2FirstSetNewRating = 0.0;
    $match->setPlayer2NewRating($player2FirstSetNewRating);
    $player2FirstGetNewRating = $match->getPlayer2Score();
    $player1SecondSetNewRating = 1.0;
    $match->setPlayer1NewRating($player1SecondSetNewRating);
    $player1SecondGetNewRating = $match->getPlayer1NewRating();
    $player2SecondGetNewRating = $match->getPlayer2NewRating();
    $this->assertEquals($player1FirstSetNewRating, $player1FirstGetNewRating);
    $this->assertEquals(
      $player1SecondSetNewRating,
      $player1SecondGetNewRating,
    );
    $this->assertEquals($player2FirstSetNewRating, $player2FirstGetNewRating);
    $this->assertEquals(
      $player2FirstSetNewRating,
      $player2SecondGetNewRating,
    );
  }

  public function testSetAndGetPlayer2NewRating(): void {
    $match = new Match();
    $player2FirstSetNewRating = 0.0;
    $match->setPlayer2NewRating($player2FirstSetNewRating);
    $player2FirstGetNewRating = $match->getPlayer2NewRating();
    $player1FirstSetNewRating = 0.0;
    $match->setPlayer1NewRating($player1FirstSetNewRating);
    $player1FirstGetNewRating = $match->getPlayer1Score();
    $player2SecondSetNewRating = 1.0;
    $match->setPlayer2NewRating($player2SecondSetNewRating);
    $player2SecondGetNewRating = $match->getPlayer2NewRating();
    $player1SecondGetNewRating = $match->getPlayer1NewRating();
    $this->assertEquals($player2FirstSetNewRating, $player2FirstGetNewRating);
    $this->assertEquals(
      $player2SecondSetNewRating,
      $player2SecondGetNewRating,
    );
    $this->assertEquals($player1FirstSetNewRating, $player1FirstGetNewRating);
    $this->assertEquals(
      $player1FirstSetNewRating,
      $player1SecondGetNewRating,
    );
  }

  public function testExpectedScore(): void {
    $match = new Match();
    // we're trying to exercise the caching logic here.
    $this->assertTrue($match->setPlayer1ExpectedScore(1.0));
    $this->assertTrue($match->setPlayer2ExpectedScore(1.0));
    $this->assertEquals(1.0, $match->getPlayer1ExpectedScore());
    $this->assertEquals(1.0, $match->getPlayer2ExpectedScore());
    // drop it to zero again.
    $match->init();
    // if the two transformed values combined still net 0 don't do division
    $this->assertTrue($match->setPlayer1TransformedRating(-1.0));
    $this->assertTrue($match->setPlayer2TransformedRating(1.0));
    $this->assertEquals(0.0, $match->getPlayer1ExpectedScore());
    $this->assertEquals(0.0, $match->getPlayer2ExpectedScore());
  }
}
