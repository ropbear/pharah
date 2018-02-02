# APRS AND AX.25
#### From http://www.aprs.org/doc/APRS101.PDF
Protocols At the link level, APRS uses the AX.25 protocol, as defined in Amateur
Packet-Radio Link-Layer Protocol (see Appendix 6 for details), utilizing
Unnumbered Information (UI) frames exclusively. This means that APRS
runs in connectionless mode, whereby AX.25 frames are transmitted without
expecting any response, and reception at the other end is not guaranteed.
At a higher level, APRS supports a messaging protocol that allows users to
send short messages (one line of text) to nominated stations, and expects to
receive acknowledgements from those stations.

#### The AX.25 Frame All APRS transmissions use AX.25 UI-frames, with 9 fields of data:
## AX.25 UI-FRAME FORMAT

![AX.25 UI](https://raw.githubusercontent.com/stonepresto/pharah/master/ref/aprs_ax25.png)

#### Flag — The flag field at each end of the frame is the bit sequence 0x7e
that separates each frame.

#### Destination Address — This field can contain an APRS destination
callsign or APRS data. APRS data is encoded to ensure that the field
conforms to the standard AX.25 callsign format (i.e. 6 alphanumeric
characters plus SSID). If the SSID is non-zero, it specifies a generic
APRS digipeater path.

#### Source Address — This field contains the callsign and SSID of the
transmitting station. In some cases, if the SSID is non-zero, the SSID
may specify an APRS display Symbol Code.

#### Digipeater Addresses — From zero to 8 digipeater callsigns may be
included in this field. Note: These digipeater addresses may be
overridden by a generic APRS digipeater path (specified in the
Destination Address SSID).

#### Control Field — This field is set to 0x03 (UI-frame).

#### Protocol ID — This field is set to 0xf0 (no layer 3 protocol).

#### Information Field — This field contains more APRS data. The first
character of this field is the APRS Data Type Identifier that specifies the
nature of the data that follows.

#### Frame Check Sequence — The FCS is a sequence of 16 bits used for
checking the integrity of a received frame.
