member(a).
member(b).
member(c).

not_member(X):-not_mountain_climber(X),not_skier(X).

likes(a,rain).
likes(a,snow).

dislikes(b,X):-likes(a,X).
likes(a,X):-likes(b,X),!,fail.
likes(a,X).

mountain_climber(X):-likes(X,rain),!,fail.
mountain_climber(X).

not_skier(X):-dislikes(X,snow).

not_mountain_climber(X):-mountain_climber(X),!,fail.
not_mountain_climber(X).

skier(X):-not_skier(X),!,fail.
skier(X).

dislikes(X,Y):-likes(X,Y),!,fail.
dislikes(X,Y).

member(X):-not_member(X),!,fail.
member(X).

query(X):-member(X),mountain_climber(X),not_skier(X),!.
