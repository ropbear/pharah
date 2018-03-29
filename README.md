# pharah

## Summary
The Pharah project was started as part of the CS401 Capstone Project titled Black Knight II, in which a Balloon Satellite payload of a Geiger counter was launched with Cube Satellite hardware transmitting its data. The purpose of the capstone was to ensure the equipment was working before actual prepartion for launch began. The purpose of the Pharah software was to locate and direct the rotator, to which was attached a directional antenna that communicated with the GOMSpace CubeSat hardware.

APRS was utilized for the geolocation of the BallonSat, and through the telemetry recieved the necessary adjustments the rotator would have to make were applied.

## Implementation
The hardware used to implement this program included:
- Raspberry Pi 3
- RTL-SDR
- Byonics Micro Trak 1000

## Index
pharah
  - aprs
    - direct
      - direwolf\_parse
        - a simple bash script to standardize the output of the Direwolf software (link below)
    - indirect
      - aprs\_fi\_parse
        - another simple bash script to retrieve and parse data from aprs.fi
    - raw
      - aprs\_parse
        - binary file
      - aprs\_parse.c
        - c program that parses the raw bytes of an APRS packet. This was not actually implemented, but simply made just in case.
      - test\_packet
        - a simple format of a APRS packet for testing purposes on the raw parser
 - ref
    - aprs\_ax25.md
      - a blurb about AX.25 UI frames extracted from the APRS guide.
    - aprs\_ax25.png
      - a PNG to complement the above reference.
    - direwolf\_output.txt
      - sample output from the Direwolf program.
	- rotator
		- angcalc.c
			- angle caculations
		- angcalc.h
			- header file containing APRS tuple
		- main.c
			- Main for the rotator
		- RC2800PRKX2SU
			- a CSV of commands for the model of rotator specified by the filename
		- rotate
			- compile with `gcc angcalc.c main.c -o rotate -lm`

## References
1. https://github.com/wb2osz/direwolf
  > Tool utilized to parse APRS packets coming from RTL-SDR on the Raspberry Pi station.
