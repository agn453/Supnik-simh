# Supnik-simh
Bob Supnik's classic historic simulators - aka SimH V3.X [^1]

[^1]: SimH V3.X is available from http://simh.trailing-edge.com/

This repository contains my updates to re-instate build capabilities
under OpenVMS on VAX, Alpha, IA64 and x86_64.

Ethernet support via Pcap-VMS is currently available for OpenVMS
Alpha and IA64 only for the PDP11, VAX and VAX780 simulators.

I've built this using MMK V5.1-3 [^2] for the following configurations -

[^2]: MMK is available as an installable PCSI package from
https://vms.process.com/scripts/fileserv/fileserv.com?MMK

* OpenVMS VAX V7.3 with Compaq C V6.4-005
* VSI OpenVMS Alpha V8.4-2L1 with VSI C V7.4-002
* VSI OpenMMS x86_64 V9.2-2 with VSI C x86-64 V7.5-009 (GEM 50XBR)


Tony Nicholson 18-Aug-2024
