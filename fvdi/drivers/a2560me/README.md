# A2560Me RGB565 driver

This is the canonical driver source. It uses the DDR video reservation
advertised by the EmuTOS FVRM cookie, write-through CPU caches and VDMA.
Diagnostics are in ../../a2560me-probe; host register tests are under
tests/a2560me at the repository root. System integration and deployment:
https://github.com/FoenixLabs/freemint-agent-workspace.
