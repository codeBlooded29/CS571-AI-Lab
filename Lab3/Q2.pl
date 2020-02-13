type(a1,xor).
type(a2,xor).
type(a3,and).
type(a4,or).
type(a5,and).
type(a6,or).

and(0,0,0).
and(0,1,0).
and(1,0,0).
and(1,1,1).

or(0,0,0).
or(0,1,1).
or(1,0,1).
or(1,1,1).

xor(0,0,0).
xor(0,1,1).
xor(1,0,1).
xor(1,1,0).

connected(x1,in(1,a1)).
connected(x2,in(2,a1)).
connected(out(a1),in(1,a2)).
connected(c1,in(2,a2)).
connected(x1,in(1,a3)).
connected(x2,in(2,a3)).
connected(x1,in(1,a4)).
connected(x2,in(2,a4)).
connected(out(a4),in(1,a5)).
connected(c1,in(2,a5)).
connected(out(a3),in(1,a6)).
connected(out(a5),in(2,a6)).

xor(in(1,a1),in(2,a1),out(a1)).
xor(in(1,a2),in(2,a2),out(a2)).
and(in(1,a3),in(2,a3),out(a3)).
or(in(1,a4),in(2,a4),out(a4)).
and(in(1,a5),in(2,a5),out(a5)).
or(in(1,a6),in(2,a6),out(a6)).

connected(X,Y):-connected(Y,X).

signal(X,0):-signal(Z,0),connected(X,Z).
signal(X,1):-signal(Z,1),connected(X,Z).

