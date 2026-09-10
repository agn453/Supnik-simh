# Supnik-simh
Bob Supnik's classic historic simulators - aka SimH V3.X [^1]

[^1]: SimH V3.X is available from http://simh.trailing-edge.com/

This repository contains my updates to re-instate build capabilities
under OpenVMS on VAX, Alpha, IA64 and x86_64.

I've added a modification to the PDP11 simulator to correct
MASSBUS controller clear vs rhwc register (as announced on the
SIMH mailing list by Bob Supnik on Aug 1, 2025) - affecting
sim/PDP11/pdp11_rh.c).  Also updated the PDP10 sources for 
sim/PDP10/pdp10_rp.c and sim/PDP10/pdp10_tu.c to fix the same
issue for the PDP10 simulator.

The PDP-11 DECtape TC11/TU56 simulator routine (sim/PDP11/pdp11_tc.c)
should be using local detach routine and not detach_unit() - from
Bob Supnik's Apr 6, 2026 update.

Fix parse_sim() to properly do the default address increment as
mentioned in Bob Supnik's posting to the SIMH mailing list on
Apr 18, 2026 (affecting sim/PDP10/pdp10_sys.c).

Included the update to sim/PDP18B/pdp18_lp.c from Bob Supnik's
Jul 1, 2026 patch to add a 1000 instruction delay on a type 647 line
printer initialization before the LPCB is actioned (enough time
for the diagnostic to progress).

The makefile for the pdp11 target was missing the pdp11_dh.c source
file for the DH11 asynchronous serial line interface.

And finally, I have merged all of Bob Supnik's updates for V3.2-6 from
https://simh.trailing-edge.com/sources/current including updates to

* GRI: additional loader formats
* PDP11: added DH11 controller

plus

* Bug fixes to  H316, 1401, PDP10, PDP11, PDP18B and Sigma simulators.

OpenVMS Ethernet support via Pcap-VMS is currently available for
Alpha and IA64 only for the PDP11, VAX and VAX780 simulators.

I've built this using MMK V5.1-3 [^2] for the following configurations -

[^2]: MMK is available as an installable PCSI package from
https://vms.process.com/scripts/fileserv/fileserv.com?MMK

* OpenVMS VAX V7.3 with Compaq C V6.4-005
* VSI OpenVMS Alpha V8.4-2L1 with VSI C V7.4-002
* VSI OpenVMS x86_64 V9.2-2 with VSI C x86-64 V7.5-009 (GEM 50XBR)


Tony Nicholson 10-Sep-2026
