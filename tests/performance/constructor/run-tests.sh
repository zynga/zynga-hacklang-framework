echo "Timing for obj with constructor"
time hhvm construct_obj_constructor.hh
echo "Timing for obj with no constructor"
time hhvm construct_obj_noconstructor.hh
echo "Timing for obj with no constructor and using object cloning"
time hhvm construct_obj_noconstructor_cloner.hh
echo "Simulating object ref as part of the construction"
time hhvm construct_obj_noconstructor_objectref.hh
