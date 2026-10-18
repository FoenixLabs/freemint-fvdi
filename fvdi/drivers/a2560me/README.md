# A2560Me RGB565 / RGB888 driver

This is the canonical driver source. It uses the DDR video reservation
advertised by the EmuTOS FVRM cookie, write-through CPU caches and VDMA.
Diagnostics are in ../../a2560me-probe; host register tests are under
tests/a2560me at the repository root. System integration and deployment:
https://github.com/FoenixLabs/freemint-agent-workspace.

The default is RGB565. Add `rgb24` to the driver line to select 24-bit RGB
stored as big-endian 32-bit `00RRGGBB` pixels. GEM/VDI sees 32 planes, packed
pixels and software CLUT; colour precision is 8 bits per RGB component.
`half` and `960`/`1024` apply to both depths. The framebuffer reservation
already accommodates 1280x1024x32. CPU primitives and VDMA use the selected
pixel size, including overlap-safe copies and cursor save/restore.
