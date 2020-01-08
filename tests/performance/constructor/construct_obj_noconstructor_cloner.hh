<?hh

class ConstructObj_NoConstructor_Template {
  public int $a = -1;
  public string $b = '';
  public int $c = -1;
}

$template = new ConstructObj_NoConstructor_Template();

for ($i = 0; $i < 100000000; $i++) {
  $obj = clone $template;
}
