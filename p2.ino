void setup() {
  // put your setup code here, to run once:
  // Redefine the newlib libc _write() function so you can use printf in your code
}

void loop() {
  // put your main code here, to run repeatedly:

}

  extern "C" {
    int _write(int fd, const void *buf, size_t count) {
      // STDOUT
      if (fd == 1)
        SerialUSB.write((char*)buf, count);
      return 0;
    }
  }
