# Implementation Plan

A quick write-up from Claude on implementing a reduced-feature set MQTT Server

## Claude Response

Yes, great idea — and testing your client against a real broker (Mosquitto) and your server against a real client gives you two independent verification points, which is really valuable for learning.

## MQTT Client — Learning Scope

**Core responsibilities of a client:**

- Open and manage a TCP connection to a broker
- Encode and send MQTT packets as binary
- Read and decode incoming MQTT packets
- Expose a clean Ruby API that hides the protocol details

This is actually a nice complement to the server project because you're implementing the *same* packet structures, but from the other side — serializing instead of parsing (or rather, doing both, but with reversed roles).

## A Sensible Scope

**Start with:**

- `CONNECT` / handle `CONNACK`
- `PUBLISH` (QoS 0)
- `SUBSCRIBE` / handle `SUBACK`
- `UNSUBSCRIBE` / handle `UNSUBACK`
- `PINGREQ` / handle `PINGRESP`
- `DISCONNECT`
- A simple blocking subscriber loop (read packets, dispatch to a callback)

**Leave out initially:**

- QoS 1 and 2
- TLS
- Reconnection logic
- Persistent sessions
- Last Will and Testament
- Async/non-blocking IO

## What a Nice Ruby API Might Look Like

```ruby
client = MQTT::Client.new(host: "localhost", port: 1883, client_id: "my-client")
client.connect

client.publish("sensors/temperature", "72.5")

client.subscribe("sensors/#") do |topic, message|
  puts "#{topic}: #{message}"
end

client.disconnect
```

The goal is that the internals deal with all the binary protocol details, but the public API feels like idiomatic Ruby.

## Packet Encoding vs. Decoding

One thing you'll notice quickly is that the client and server share a lot of the same logic — just inverted:

| | Server | Client |
|---|---|---|
| `CONNECT` | Decode (parse from client) | Encode (send to broker) |
| `CONNACK` | Encode (send to client) | Decode (parse from broker) |
| `PUBLISH` | Decode incoming, re-encode to subscribers | Encode outgoing, decode incoming |
| `SUBSCRIBE` | Decode (parse from client) | Encode (send to broker) |

This suggests a natural shared structure — a packet serializer/deserializer module that both projects can use, which is a good abstraction exercise in itself.

## Testing Matrix

The real value of building both is the cross-testing you get:

| Your Client | Your Server | Expected outcome |
|---|---|---|
| ✅ | Mosquitto | Validates your client against known-good broker |
| Mosquitto CLI | ✅ | Validates your server against known-good client |
| ✅ | ✅ | Full end-to-end with your own code |

Running `mosquitto_pub` and `mosquitto_sub` against your server, and your client against a real Mosquitto instance, gives you high confidence that you've implemented the spec correctly before you try connecting your own client to your own server.

## Stretch Goals

- QoS 1 (requires packet ID tracking and `PUBACK`)
- Auto-reconnect with backoff
- Keep-alive ping management (send `PINGREQ` automatically on a timer)
- Thread-safe publish from multiple threads
- TLS support via Ruby's `OpenSSL` library

## Suggested Build Order

1. TCP connection + `CONNECT`/`CONNACK` handshake
2. `PUBLISH` QoS 0 (send only)
3. `SUBSCRIBE` + incoming message loop
4. `PINGREQ`/`PINGRESP` keep-alive
5. `DISCONNECT`
6. Test against Mosquitto thoroughly
7. Then test your server with `mosquitto_pub`/`mosquitto_sub`
8. Finally wire your client to your server

The packet encoding work you do for the client will make the server-side parsing much more intuitive, and vice versa — they reinforce each other well.
