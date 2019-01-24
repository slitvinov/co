set -eu

. he.util

make

# Nt=5120
off=$(he.path)/sph/laplace/0.off
ref=$(he.path)/rbc/laplace/0.off

pi=3.141592653589793115997964
Vr=0.642
Ka=1
Kga=1
Kv=1

Kas=0
mus=0

dt=0.005
mu=0

Kb=0.1

C0=0
Kad=0
end=100000000
freq=100

DA0D=0

dir=o

./main juelicher_xin $Vr $Ka $Kga $Kv $Kb $C0 $Kad $DA0D $mu $dt $end $freq    $dir  \
       strain $ref lim $Kas $mus 0 0 0 0 < $off
