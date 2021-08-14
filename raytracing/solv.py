from sympy import *

pox, poy, poz, ux, uy, uz, vx, vy, vz, ox, oy, oz, rx, ry, rz, u, v, d = var(
    "pox poy poz ux uy uz vx vy vz ox oy oz rx ry rz u v d"
)
E1 = pox + u * ux + v * vx - ox - d * rx
E2 = poy + u * uy + v * vy - oy - d * ry
E3 = poz + u * uz + v * vz - oz - d * rz

solutions = solve([E1, E2, E3], [u, v, d])
pprint(factor(solutions[d]))
