# Barnsley’s Fractal Fern

The Barnsley’s fern is a fractal created by an iterated function system, in which a point (the seed) is repeatedly transformed by using one of four transformation functions.

## The transformations are affine transformations of form

x’ = ax + cy + e

y’ = bx + dy + f

## And so each transformation can be specified by six constants a, b, c, d, e, and f. These constants defined over 4 functions.

ƒ1: xn + 1 = 0 ; yn + 1 = 0.16 yn where (a= 0, b =0, c =0,d =0.16, e =0, f=0 )

ƒ2: xn + 1 = 0.85 xn + 0.04 yn; yn + 1 = −0.04 xn + 0.85 yn + 1.6

where (a= 0.85, b = -0.04, c =0.04, d =0.85, e =0, f=1.6 )

ƒ3: xn + 1 = 0.2 xn − 0.26 yn; yn + 1 = 0.23 xn + 0.22 yn + 1.6

where (a= 0.2, b =0.23, c =-0.26, d =0.22, e =0, f=1.6 )

ƒ4: xn + 1 = −0.15 xn + 0.28 yn; yn + 1 = 0.26 xn + 0.24 yn + 0.44

where (a= -0.15, b =0.26, c =0.28, d =0.24, e =0, f=0.44 )

## Map the pixels using glVertex3f(x, y, 0) for n iterations (ex: n = 200000) where

1% of the times choose coordinate transformation with ƒ1,

85% of the times choose coordinate transformation with ƒ2,

7% of the times choose coordinate transformation with ƒ3,

7% of the times choose coordinate transformation with ƒ4,

for a given random number.