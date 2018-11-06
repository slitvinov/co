set -eu

Vr=0.65
Kb=0.01
C0=0
Kad=0
DA0D=0
end=100000

off=$HOME/codes/he/data/prolate/laplace/Nt1280/prolate_volume_Vr

if test $# -ne 0
then
    Vr="$1"
    echo '************'  Vr = $Vr '************'
    #rm inter.dat
    ../../main meyer_xin $Vr 3 3 3 0 $Kb $C0 $Kad $DA0D $end < $off$Vr.off > prolate_Vr$Vr.msg
    mv end.off prolate_Vr$Vr.off
    mv inter.dat prolate_Vr$Vr.dat
else
    for Vr in 0.6 0.55 0.5 0.45 0.4 0.35 0.3 0.25 0.2
    do sh run_prolate.sh $Vr
    done
fi
