# Implementation Plan

A quick write-up from Claude on implementing a reduced-feature set MQTT Server

## Claude Response

Yes, absolutely — it's one of the best ways to deeply understand a protocol. MQTT is particularly well-suited for this kind of exercise because:

**The protocol is genuinely simple.** Unlike HTTP/2 or AMQP, MQTT was designed for constrained devices, so the spec is lean. The core packet types you'd need to implement are small enough to hold in your head.

**You'd learn things you can't get from just using a broker.** Writing one forces you to understand:

- How TCP framing works (MQTT's variable-length encoding is a great intro to binary protocols)
- What QoS actually means at the wire level
- How topic matching with wildcards (`+` and `#`) works under the hood
- Why retained messages and last will messages are designed the way they are

**Ruby is a reasonable choice for this.** You'd use `TCPServer` from the standard library to accept raw connections, then read and parse the binary MQTT packet format yourself. It's a good exercise in working with `IO`, `StringIO`, and binary string manipulation in Ruby — skills that transfer well.

## A Sensible Scope for a Learning Implementation

**Start with (MQTT 3.1.1):**

- `CONNECT` / `CONNACK`
- `PUBLISH` (QoS 0 only to start)
- `SUBSCRIBE` / `SUBACK`
- `UNSUBSCRIBE` / `UNSUBACK`
- `PINGREQ` / `PINGRESP`
- `DISCONNECT`

**Leave out initially:**

- QoS 1 and 2 (acknowledgement and exactly-once delivery)
- Retained messages
- Last Will and Testament
- Authentication
- TLS
- Persistence across restarts

That reduced scope is actually a functional pub/sub broker — clients can connect, subscribe to topics, and receive messages published by other clients. You could have it working in a few hundred lines of Ruby.

**Stretch goals once the basics work:**

- Add wildcard topic matching
- Add QoS 1
- Add retained messages
- Make it multi-threaded (one thread per client connection)
- Test it with a real client like Mosquitto's `mosquitto_pub` / `mosquitto_sub` CLI tools or the `mqtt` gem

The [MQTT 3.1.1 spec](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html) is actually quite readable — only about 80 pages and well-structured. It would be your primary reference throughout.

It's a great project for understanding both the protocol and lower-level network programming in Ruby.
