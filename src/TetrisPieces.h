#define PIECE_I 0
#define PIECE_O 1
#define PIECE_T 2
#define PIECE_S 3
#define PIECE_Z 4
#define PIECE_J 5
#define PIECE_L 6

const byte Pieces[7][4][16] PROGMEM = {
  // I
  {
    { 0x0, 0x0, 0x0, 0x0,
      0x1, 0x1, 0x1, 0x1,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x1, 0x0, 0x0,
      0x0, 0x1, 0x0, 0x0,
      0x0, 0x1, 0x0, 0x0,
      0x0, 0x1, 0x0, 0x0
    },

    { 0x0, 0x0, 0x0, 0x0,
      0x1, 0x1, 0x1, 0x1,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x1, 0x0, 0x0,
      0x0, 0x1, 0x0, 0x0,
      0x0, 0x1, 0x0, 0x0,
      0x0, 0x1, 0x0, 0x0
    },
  },
  // O
  {
    { 0x0, 0x2, 0x2, 0x0,
      0x0, 0x2, 0x2, 0x0,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x2, 0x2, 0x0,
      0x0, 0x2, 0x2, 0x0,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x2, 0x2, 0x0,
      0x0, 0x2, 0x2, 0x0,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x2, 0x2, 0x0,
      0x0, 0x2, 0x2, 0x0,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },
  },
  // T
  {
    { 0x0, 0x3, 0x0, 0x0,
      0x3, 0x3, 0x3, 0x0,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x3, 0x0, 0x0,
      0x0, 0x3, 0x3, 0x0,
      0x0, 0x3, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x0, 0x0, 0x0,
      0x3, 0x3, 0x3, 0x0,
      0x0, 0x3, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x3, 0x0, 0x0,
      0x3, 0x3, 0x0, 0x0,
      0x0, 0x3, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },
  },
  // S
  {
    { 0x0, 0x4, 0x4, 0x0,
      0x4, 0x4, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x4, 0x0, 0x0,
      0x0, 0x4, 0x4, 0x0,
      0x0, 0x0, 0x4, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x0, 0x0, 0x0,
      0x0, 0x4, 0x4, 0x0,
      0x4, 0x4, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x4, 0x0, 0x0, 0x0,
      0x4, 0x4, 0x0, 0x0,
      0x0, 0x4, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },
  },
  // Z
  {
    { 0x5, 0x5, 0x0, 0x0,
      0x0, 0x5, 0x5, 0x0,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x0, 0x5, 0x0,
      0x0, 0x5, 0x5, 0x0,
      0x0, 0x5, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x0, 0x0, 0x0,
      0x5, 0x5, 0x0, 0x0,
      0x0, 0x5, 0x5, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x5, 0x0, 0x0,
      0x5, 0x5, 0x0, 0x0,
      0x5, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },
  },
  // J
  {
    { 0x6, 0x0, 0x0, 0x0,
      0x6, 0x6, 0x6, 0x0,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x6, 0x6, 0x0,
      0x0, 0x6, 0x0, 0x0,
      0x0, 0x6, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x0, 0x0, 0x0,
      0x6, 0x6, 0x6, 0x0,
      0x0, 0x0, 0x6, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x6, 0x0, 0x0,
      0x0, 0x6, 0x0, 0x0,
      0x6, 0x6, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },
  },
  // L
  {
    { 0x0, 0x0, 0x7, 0x0,
      0x7, 0x7, 0x7, 0x0,
      0x0, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x7, 0x0, 0x0,
      0x0, 0x7, 0x0, 0x0,
      0x0, 0x7, 0x7, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x0, 0x0, 0x0, 0x0,
      0x7, 0x7, 0x7, 0x0,
      0x7, 0x0, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },

    { 0x7, 0x7, 0x0, 0x0,
      0x0, 0x7, 0x0, 0x0,
      0x0, 0x7, 0x0, 0x0,
      0x0, 0x0, 0x0, 0x0
    },
  }
};
