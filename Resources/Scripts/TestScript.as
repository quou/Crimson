int counter = 0;

void OnBegin() {
   print("Hello from AngelScript");
}

void OnUpdate(float delta) {
   counter++;

   if (counter > 5000 * delta) {
      print("Hello from update");
      counter = 0;
   }
}
