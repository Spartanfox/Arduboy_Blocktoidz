const unsigned char levels = 3;
  const unsigned char breakers  [][7*6] = {
     {
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, B_BOTTOM|B_RIGHT, B_BOTTOM|B_LEFT,0, 0,
     0, B_BOTTOM|B_RIGHT, B_SOLID, B_SOLID, B_BOTTOM|B_LEFT,0,
     B_BOTTOM|B_RIGHT, B_SOLID,B_TOP|B_LEFT,B_TOP|B_RIGHT, B_SOLID, B_BOTTOM|B_LEFT,
     },
     {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, B_BOTTOM|B_RIGHT, B_BOTTOM|B_LEFT, B_SOLID-B_TOP, 0,
     0, B_BOTTOM|B_RIGHT, B_SOLID, B_SOLID, B_SOLID, 0,
     B_BOTTOM|B_RIGHT, B_SOLID, B_SOLID, B_SOLID, B_SOLID, B_LEFT|B_BOTTOM,
     0, B_SOLID,0, B_SOLID, B_SOLID, 0,
     0, B_SOLID,0, B_SOLID, B_SOLID, 0,
     },

     {
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, B_BOTTOM|B_RIGHT, B_BOTTOM|B_LEFT, 0, 0,
     0, B_RIGHT, B_SOLID-B_RIGHT, B_SOLID-B_LEFT, B_LEFT, 0,
     0, 0, B_SOLID,B_SOLID, 0, 0,
     0, B_BOTTOM|B_LEFT,B_SOLID-B_LEFT,B_SOLID-B_RIGHT, B_BOTTOM|B_RIGHT, 0,
     B_BOTTOM|B_RIGHT,B_SOLID, B_SOLID, B_SOLID,B_SOLID,B_LEFT|B_BOTTOM,
     },
};
