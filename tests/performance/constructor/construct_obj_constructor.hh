<?hh

class ConstructObj_Constructor {
  public int $a;
  public string $b;
  public int $c;

  public function __construct() {
    $this->a = -1;
    $this->b = '';
    $this->c = -1;
  }
}

for ($i = 0; $i < 100000000; $i++) {
  $obj = new ConstructObj_Constructor();
}
