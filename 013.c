#include <stdio.h>
#include <stdbool.h>
#include "src/go.h"

// Used for printing characters
Channel *print_chan;

// Used for synchronization when a fizz or buzz finishes printing
Channel *done_chan;

Value do_print() {
  const char c = receive(print_chan).asChar;
  putchar(c);
  return asU8(42); // Arbitrary value
}

void print_char(char c) {
  Channel *completion_channel = go(do_print);
  send(print_chan, asChar(c));
  receive(completion_channel);
}

Value do_fizz() {
  Channel *chan = me();
  while (true) {
    receive(chan);
    print_char('f');
    print_char('i');
    print_char('z');
    print_char('z');
    send(done_chan, asU8(42)); // Arbitrary value
  }
  return asU8(42); // Arbitrary value
}

Value do_buzz() {
  Channel *chan = me();
  while (true) {
    receive(chan);
    print_char('b');
    print_char('u');
    print_char('z');
    print_char('z');
    send(done_chan, asU8(42)); // Arbitrary value
  }
  return asU8(42); // Arbitrary value
}

int main() {
  // Setup
  Channel *fizz_chan = go(do_fizz);
  Channel *buzz_chan = go(do_buzz);
  print_chan = channel();
  done_chan = channel();

  // do fizzbuzz
  for (int i = 1; i <= 100; i++) {
    bool print_number = true;
    if (i % 3 == 0) {
      print_number = false;
      send(fizz_chan, asU8(42));
      receive(done_chan);
    }

    if (i % 5 == 0) {
      print_number = false;
      send(buzz_chan, asU8(42));
      receive(done_chan);
    }

    if (print_number) {
      char buf[16];
      snprintf(buf, 16, "%d", i);
      char *cur = buf;
      while (*cur != '\0') {
        print_char(*cur);
        cur += 1;
      }
    }

    print_char('\n');
  }
}
