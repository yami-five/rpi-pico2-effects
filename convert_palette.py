def convert_color(color):
    r=(color>>16) & 0xff
    g=(color>>8) & 0xff
    b=color & 0xff
    r = max(0, min(255, r))
    g = max(0, min(255, g))
    b = max(0, min(255, b))
    r = r >> 3
    g = g >> 2
    b = b >> 3
    rgb565=(r << 11) | (g << 5) | b
    high_byte = f"0x{((rgb565 >> 8) & 0xFF):02X}"
    low_byte = f"0x{(rgb565 & 0xFF):02X}"
    return f"{high_byte},{low_byte},"

rgb_palette=[
    0x27032a,
    0x4b083d,
    0x731144,
    0x890c38,
    0xab0a2a,
    0xbe2028,
    0xcf492c,
    0xe36433,
    0xe3884e,
    0xecb55f,
    0xeed67b,
    0xf4efae,
    0xfffdd9,
    0xfbfbf2
]

for p in rgb_palette:
    print(convert_color(p))