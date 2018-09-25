<?hh

class ConstructObj_NoConstructor_ObjectRef_SpecialObject {
  public bool $aThing = false;
  public string $somethingElse = "some string that is important";
}

class ConstructObj_NoConstructor_ObjectRef {
  public int $a = -1;
  public string $b = '';
  public int $c = -1;
  public ?ConstructObj_NoConstructor_ObjectRef_SpecialObject $obj = null;

  public function setStuff(
    ConstructObj_NoConstructor_ObjectRef_SpecialObject $a,
  ): void {
    $this->obj = $a;
  }

}

$specialObject = new ConstructObj_NoConstructor_ObjectRef_SpecialObject();

for ($i = 0; $i < 100000000; $i++) {
  $obj = new ConstructObj_NoConstructor_ObjectRef();
  $obj->setStuff($specialObject);
}
