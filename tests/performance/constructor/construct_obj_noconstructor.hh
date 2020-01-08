<?hh

class ConstructObj_NoConstructor {
  public int $a = -1;
  public string $b = '';
  public int $c = -1;
}

for ($i = 0; $i < 100000000; $i++) {
  $obj = new ConstructObj_NoConstructor();
}
