# Data Link Layer Protocols Simulation
## Description 
> This project simulate and tests the simplex stop-and-wait ARQ data link layer protocol
between peers that are connected with a noisy channel, where the transmission is not error-free,
packets may get corrupted, duplicated, delayed, or lost, and the buffers are of limited sizes.

---

**It is an omnet++ project where two basic modules:**
#### 1. Sender module:
- read the input given its name from the ini configuration file
-

#### 2. Receiver module:
- receive a message with certain sequence number and apply error detection on based on parity bit checksum
- if message is correct, then send acknowledge (ACK)
- otherwise send not aknowledge (NACK)
- if message has wrong sequence number then it is ignored

