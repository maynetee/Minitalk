_This project has been created as part of the 42 curriculum by mteichma._

# Minitalk

A client and a server that exchange text using only two UNIX signals, SIGUSR1 and SIGUSR2.

## What it does

- `server` prints its PID and waits. Every byte it decodes is written to standard output; the end of a message is printed as a newline.
- `client <server_pid> "<message>"` sends the message one bit at a time, followed by a terminating NUL byte.
- Every bit is acknowledged. The server answers each signal with SIGUSR1, and the client sends the next bit only once that answer has arrived. After the last byte is acknowledged the client prints `Message sent and confirmed!`.
- UTF-8 text (accents, CJK, emoji) arrives intact because the transfer is byte-oriented. There is no Unicode-specific code.

## How it works

**Encoding.** Each byte goes out least-significant bit first: SIGUSR1 means 0, SIGUSR2 means 1 (`client.c`, `send_char` and `send_bit`).

**Server state.** The handler is installed with `sigaction` and `SA_SIGINFO`, so it receives a `siginfo_t` and reads the sender's PID from `si_pid`. The client never has to announce itself. The bit counter and the byte being assembled are `static` locals of the handler (`server.c`, `handle_signal`). After 8 bits the byte is written with `write(2)` and both are reset.

**Synchronisation.** Standard signals are not queued: if a second SIGUSR2 arrives while the first is still pending, the kernel merges them and a bit is lost. The protocol is therefore stop-and-wait. The server sends its acknowledgement at the end of the handler, after the bit is recorded. The client's own handler sets a flag (`g_ack`), and `send_bit` polls it with `usleep(1000)` for up to about 5 seconds before giving up with an error.

```
client                                   server
  |-- SIGUSR1 or SIGUSR2 (bit 0) -------->|  c |= bit << 0, bits = 1
  |<------------------------ SIGUSR1 -----|  ack to si_pid
  |-- bit 1 ----------------------------->|
  |<------------------------ SIGUSR1 -----|
  |   ... bits 2 to 7 ...                 |  bits == 8: write(1, &c, 1)
  |-- 8 x SIGUSR1 (NUL byte) ------------>|  c == 0: write "\n"
  |<------------------------ SIGUSR1 -----|
  prints "Message sent and confirmed!"
```

## Build and run

```sh
make                          # builds Libft, then client and server (clang -Wall -Wextra -Werror)
./server                      # Server PID: 12345
./client 12345 "Hello, world" # in another terminal
```

`make bonus` builds the same binaries: acknowledgement and Unicode are handled in the mandatory sources. `make clean`, `make fclean` and `make re` work as usual. The project links the author's own Libft, which includes `ft_printf` (`Libft/`).

## Design choices and hard parts

- **Acknowledge every bit instead of sleeping a fixed delay** (`client.c`, `send_bit`). A fixed `usleep` between signals is either slow or loses bits when the machine is busy. With a per-bit acknowledgement, speed follows the machine. In a local test on macOS, a 1,000-character message took under 0.1 s.
- **Acknowledge last** (`server.c`, `handle_signal`). `kill` is the last call in the handler, so the next bit cannot arrive before the current one has been stored.
- **No PID handshake.** `SA_SIGINFO` gives the server the sender's PID with every signal. That is all it needs to reply.
- **Minimal global state.** The decoder state lives in `static` variables inside the handler. The only global is the client's `g_ack` flag, within the subject's limit of one global per program.
- **Signal-safe output.** The handler prints each byte with `write(2)`, which is async-signal-safe, rather than a buffered printf.

## Limitations

- **One client at a time.** The server keeps a single decoding state and does not check which PID is sending. Two clients sending at once interleave their bits, and both messages come out corrupted.
- **No resynchronisation.** If a client dies mid-byte, the server keeps the partial byte and the next message starts misaligned.
- `g_ack` is a plain `int`. `volatile sig_atomic_t` is the correct type for a flag shared with a signal handler, and an earlier commit used it.
- Throughput is bounded by two signal deliveries per bit.

## Resources

- `sigaction(2)`, `kill(2)`, `pause(2)`, `signal(7)` and `signal-safety(7)` man pages.
- The 42 Minitalk subject.
