#include "lut.h"

const FIXED RECIPROCALLUT[700] = {
    -0xbb, -0xbb, -0xbc, -0xbc, -0xbd, -0xbd, -0xbe, -0xbf, 
    -0xbf, -0xc0, -0xc0, -0xc1, -0xc1, -0xc2, -0xc3, -0xc3, 
    -0xc4, -0xc4, -0xc5, -0xc5, -0xc6, -0xc7, -0xc7, -0xc8, 
    -0xc9, -0xc9, -0xca, -0xca, -0xcb, -0xcc, -0xcc, -0xcd, 
    -0xce, -0xce, -0xcf, -0xd0, -0xd0, -0xd1, -0xd2, -0xd2, 
    -0xd3, -0xd4, -0xd4, -0xd5, -0xd6, -0xd6, -0xd7, -0xd8, 
    -0xd9, -0xd9, -0xda, -0xdb, -0xdb, -0xdc, -0xdd, -0xde, 
    -0xde, -0xdf, -0xe0, -0xe1, -0xe1, -0xe2, -0xe3, -0xe4, 
    -0xe5, -0xe5, -0xe6, -0xe7, -0xe8, -0xe9, -0xea, -0xea, 
    -0xeb, -0xec, -0xed, -0xee, -0xef, -0xf0, -0xf0, -0xf1, 
    -0xf2, -0xf3, -0xf4, -0xf5, -0xf6, -0xf7, -0xf8, -0xf9, 
    -0xfa, -0xfb, -0xfc, -0xfd, -0xfe, -0xff, -0x100, -0x101, 
    -0x102, -0x103, -0x104, -0x105, -0x106, -0x107, -0x108, -0x109, 
    -0x10a, -0x10b, -0x10c, -0x10d, -0x10e, -0x10f, -0x111, -0x112, 
    -0x113, -0x114, -0x115, -0x116, -0x118, -0x119, -0x11a, -0x11b, 
    -0x11c, -0x11e, -0x11f, -0x120, -0x121, -0x123, -0x124, -0x125, 
    -0x127, -0x128, -0x129, -0x12b, -0x12c, -0x12e, -0x12f, -0x130, 
    -0x132, -0x133, -0x135, -0x136, -0x138, -0x139, -0x13b, -0x13c, 
    -0x13e, -0x13f, -0x141, -0x142, -0x144, -0x146, -0x147, -0x149, 
    -0x14a, -0x14c, -0x14e, -0x150, -0x151, -0x153, -0x155, -0x157, 
    -0x158, -0x15a, -0x15c, -0x15e, -0x160, -0x162, -0x164, -0x166, 
    -0x168, -0x16a, -0x16c, -0x16e, -0x170, -0x172, -0x174, -0x176, 
    -0x178, -0x17a, -0x17d, -0x17f, -0x181, -0x183, -0x186, -0x188, 
    -0x18a, -0x18d, -0x18f, -0x192, -0x194, -0x197, -0x199, -0x19c, 
    -0x19e, -0x1a1, -0x1a4, -0x1a6, -0x1a9, -0x1ac, -0x1af, -0x1b2, 
    -0x1b4, -0x1b7, -0x1ba, -0x1bd, -0x1c0, -0x1c3, -0x1c7, -0x1ca, 
    -0x1cd, -0x1d0, -0x1d4, -0x1d7, -0x1da, -0x1de, -0x1e1, -0x1e5, 
    -0x1e9, -0x1ec, -0x1f0, -0x1f4, -0x1f8, -0x1fc, -0x200, -0x204, 
    -0x208, -0x20c, -0x210, -0x214, -0x219, -0x21d, -0x222, -0x226, 
    -0x22b, -0x230, -0x234, -0x239, -0x23e, -0x243, -0x249, -0x24e, 
    -0x253, -0x259, -0x25e, -0x264, -0x26a, -0x270, -0x276, -0x27c, 
    -0x282, -0x288, -0x28f, -0x295, -0x29c, -0x2a3, -0x2aa, -0x2b1, 
    -0x2b9, -0x2c0, -0x2c8, -0x2d0, -0x2d8, -0x2e0, -0x2e8, -0x2f1, 
    -0x2fa, -0x303, -0x30c, -0x315, -0x31f, -0x329, -0x333, -0x33d, 
    -0x348, -0x353, -0x35e, -0x369, -0x375, -0x381, -0x38e, -0x39b, 
    -0x3a8, -0x3b5, -0x3c3, -0x3d2, -0x3e0, -0x3f0, -0x400, -0x410, 
    -0x421, -0x432, -0x444, -0x456, -0x469, -0x47d, -0x492, -0x4a7, 
    -0x4bd, -0x4d4, -0x4ec, -0x505, -0x51e, -0x539, -0x555, -0x572, 
    -0x590, -0x5b0, -0x5d1, -0x5f4, -0x618, -0x63e, -0x666, -0x690, 
    -0x6bc, -0x6eb, -0x71c, -0x750, -0x787, -0x7c1, -0x800, -0x842, 
    -0x888, -0x8d3, -0x924, -0x97b, -0x9d8, -0xa3d, -0xaaa, -0xb21, 
    -0xba2, -0xc30, -0xccc, -0xd79, -0xe38, -0xf0f, -0x1000, -0x1111, 
    -0x1249, -0x13b1, -0x1555, -0x1745, -0x1999, -0x1c71, -0x2000, -0x2492, 
    -0x2aaa, -0x3333, -0x4000, -0x5555, -0x8000, -0x10000, 0x10000, 0x10000, 
    0x8000, 0x5555, 0x4000, 0x3333, 0x2aaa, 0x2492, 0x2000, 0x1c71, 
    0x1999, 0x1745, 0x1555, 0x13b1, 0x1249, 0x1111, 0x1000, 0xf0f, 
    0xe38, 0xd79, 0xccc, 0xc30, 0xba2, 0xb21, 0xaaa, 0xa3d, 
    0x9d8, 0x97b, 0x924, 0x8d3, 0x888, 0x842, 0x800, 0x7c1, 
    0x787, 0x750, 0x71c, 0x6eb, 0x6bc, 0x690, 0x666, 0x63e, 
    0x618, 0x5f4, 0x5d1, 0x5b0, 0x590, 0x572, 0x555, 0x539, 
    0x51e, 0x505, 0x4ec, 0x4d4, 0x4bd, 0x4a7, 0x492, 0x47d, 
    0x469, 0x456, 0x444, 0x432, 0x421, 0x410, 0x400, 0x3f0, 
    0x3e0, 0x3d2, 0x3c3, 0x3b5, 0x3a8, 0x39b, 0x38e, 0x381, 
    0x375, 0x369, 0x35e, 0x353, 0x348, 0x33d, 0x333, 0x329, 
    0x31f, 0x315, 0x30c, 0x303, 0x2fa, 0x2f1, 0x2e8, 0x2e0, 
    0x2d8, 0x2d0, 0x2c8, 0x2c0, 0x2b9, 0x2b1, 0x2aa, 0x2a3, 
    0x29c, 0x295, 0x28f, 0x288, 0x282, 0x27c, 0x276, 0x270, 
    0x26a, 0x264, 0x25e, 0x259, 0x253, 0x24e, 0x249, 0x243, 
    0x23e, 0x239, 0x234, 0x230, 0x22b, 0x226, 0x222, 0x21d, 
    0x219, 0x214, 0x210, 0x20c, 0x208, 0x204, 0x200, 0x1fc, 
    0x1f8, 0x1f4, 0x1f0, 0x1ec, 0x1e9, 0x1e5, 0x1e1, 0x1de, 
    0x1da, 0x1d7, 0x1d4, 0x1d0, 0x1cd, 0x1ca, 0x1c7, 0x1c3, 
    0x1c0, 0x1bd, 0x1ba, 0x1b7, 0x1b4, 0x1b2, 0x1af, 0x1ac, 
    0x1a9, 0x1a6, 0x1a4, 0x1a1, 0x19e, 0x19c, 0x199, 0x197, 
    0x194, 0x192, 0x18f, 0x18d, 0x18a, 0x188, 0x186, 0x183, 
    0x181, 0x17f, 0x17d, 0x17a, 0x178, 0x176, 0x174, 0x172, 
    0x170, 0x16e, 0x16c, 0x16a, 0x168, 0x166, 0x164, 0x162, 
    0x160, 0x15e, 0x15c, 0x15a, 0x158, 0x157, 0x155, 0x153, 
    0x151, 0x150, 0x14e, 0x14c, 0x14a, 0x149, 0x147, 0x146, 
    0x144, 0x142, 0x141, 0x13f, 0x13e, 0x13c, 0x13b, 0x139, 
    0x138, 0x136, 0x135, 0x133, 0x132, 0x130, 0x12f, 0x12e, 
    0x12c, 0x12b, 0x129, 0x128, 0x127, 0x125, 0x124, 0x123, 
    0x121, 0x120, 0x11f, 0x11e, 0x11c, 0x11b, 0x11a, 0x119, 
    0x118, 0x116, 0x115, 0x114, 0x113, 0x112, 0x111, 0x10f, 
    0x10e, 0x10d, 0x10c, 0x10b, 0x10a, 0x109, 0x108, 0x107, 
    0x106, 0x105, 0x104, 0x103, 0x102, 0x101, 0x100, 0xff, 
    0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7, 
    0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0, 0xf0, 
    0xef, 0xee, 0xed, 0xec, 0xeb, 0xea, 0xea, 0xe9, 
    0xe8, 0xe7, 0xe6, 0xe5, 0xe5, 0xe4, 0xe3, 0xe2, 
    0xe1, 0xe1, 0xe0, 0xdf, 0xde, 0xde, 0xdd, 0xdc, 
    0xdb, 0xdb, 0xda, 0xd9, 0xd9, 0xd8, 0xd7, 0xd6, 
    0xd6, 0xd5, 0xd4, 0xd4, 0xd3, 0xd2, 0xd2, 0xd1, 
    0xd0, 0xd0, 0xcf, 0xce, 0xce, 0xcd, 0xcc, 0xcc, 
    0xcb, 0xca, 0xca, 0xc9, 0xc9, 0xc8, 0xc7, 0xc7, 
    0xc6, 0xc5, 0xc5, 0xc4, 0xc4, 0xc3, 0xc3, 0xc2, 
    0xc1, 0xc1, 0xc0, 0xc0, 0xbf, 0xbf, 0xbe, 0xbd, 
    0xbd, 0xbc, 0xbc, 0xbb, };

const FIXED CAMERAXLUT[120] = {
    -0x100, -0xfb, -0xf7, -0xf3, -0xee, -0xea, -0xe6, -0xe2, 
    -0xdd, -0xd9, -0xd5, -0xd1, -0xcc, -0xc8, -0xc4, -0xc0, 
    -0xbb, -0xb7, -0xb3, -0xae, -0xaa, -0xa6, -0xa2, -0x9d, 
    -0x99, -0x95, -0x91, -0x8c, -0x88, -0x84, -0x80, -0x7b, 
    -0x77, -0x73, -0x6e, -0x6a, -0x66, -0x62, -0x5d, -0x59, 
    -0x55, -0x51, -0x4c, -0x48, -0x44, -0x40, -0x3b, -0x37, 
    -0x33, -0x2e, -0x2a, -0x26, -0x22, -0x1d, -0x19, -0x15, 
    -0x11, -0xc, -0x8, -0x4, 0x0, 0x4, 0x8, 0xc, 
    0x11, 0x15, 0x19, 0x1d, 0x22, 0x26, 0x2a, 0x2e, 
    0x33, 0x37, 0x3b, 0x40, 0x44, 0x48, 0x4c, 0x51, 
    0x55, 0x59, 0x5d, 0x62, 0x66, 0x6a, 0x6e, 0x73, 
    0x77, 0x7b, 0x80, 0x84, 0x88, 0x8c, 0x91, 0x95, 
    0x99, 0x9d, 0xa2, 0xa6, 0xaa, 0xae, 0xb3, 0xb7, 
    0xbb, 0xc0, 0xc4, 0xc8, 0xcc, 0xd1, 0xd5, 0xd9, 
    0xdd, 0xe2, 0xe6, 0xea, 0xee, 0xf3, 0xf7, 0xfb, 
};


const FIXED TEXTURESTEPLUT[160] = {
    0x1000, 0x1000, 0x800, 0x555, 0x400, 0x333, 0x2aa, 0x249, 
    0x200, 0x1c7, 0x199, 0x174, 0x155, 0x13b, 0x124, 0x111, 
    0x100, 0xf0, 0xe3, 0xd7, 0xcc, 0xc3, 0xba, 0xb2, 
    0xaa, 0xa3, 0x9d, 0x97, 0x92, 0x8d, 0x88, 0x84, 
    0x80, 0x7c, 0x78, 0x75, 0x71, 0x6e, 0x6b, 0x69, 
    0x66, 0x63, 0x61, 0x5f, 0x5d, 0x5b, 0x59, 0x57, 
    0x55, 0x53, 0x51, 0x50, 0x4e, 0x4d, 0x4b, 0x4a, 
    0x49, 0x47, 0x46, 0x45, 0x44, 0x43, 0x42, 0x41, 
    0x40, 0x3f, 0x3e, 0x3d, 0x3c, 0x3b, 0x3a, 0x39, 
    0x38, 0x38, 0x37, 0x36, 0x35, 0x35, 0x34, 0x33, 
    0x33, 0x32, 0x31, 0x31, 0x30, 0x30, 0x2f, 0x2f, 
    0x2e, 0x2e, 0x2d, 0x2d, 0x2c, 0x2c, 0x2b, 0x2b, 
    0x2a, 0x2a, 0x29, 0x29, 0x28, 0x28, 0x28, 0x27, 
    0x27, 0x27, 0x26, 0x26, 0x25, 0x25, 0x25, 0x24, 
    0x24, 0x24, 0x23, 0x23, 0x23, 0x23, 0x22, 0x22, 
    0x22, 0x21, 0x21, 0x21, 0x21, 0x20, 0x20, 0x20, 
    0x20, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 0x1e, 0x1e, 
    0x1e, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1c, 0x1c, 
    0x1c, 0x1c, 0x1c, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 
    0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x1a, 0x19, 0x19, 
};
