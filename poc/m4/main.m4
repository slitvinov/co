include(lib.m4)dnl
dnl Cons(abc,[de;fghi])
AppLambda(
g($1, q)
,x;y;z)dnl

print Splice(x;y;z)
