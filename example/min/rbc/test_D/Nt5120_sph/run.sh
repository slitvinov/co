set -eu

Vr=0.641998677173
Ka=0
Kga=1
Kv=5
Ke=0

Kb=0.001
C0=0
Kad=$(echo $Kb | awk '{print $1*0.636619772368}')
#echo $Kad
DA0D=0
D=0.000599198765361
Da0=0
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
#echo $A
end=200000
freq=200

mus=0.139260575205
Kas=$(echo $mus | awk '{print $1*2}')

a3=0
a4=0
b1=0
b2=0

off=$(he.path)/sph/laplace/Nt5120.off
ref=$(he.path)/sph/laplace/Nt5120.off


if test $# -ne 0
then
    Da1="$1"
    Da0=$(echo $Da1 | awk '{print $1/100}')
    echo '***' Da1=$Da1 '***' Da0 = $Da0 '***'

    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    #echo $Da0, $Da1, $Kb, $C0, $Kad, $DA0D
	  
    if [ ! -d $Da1 ]; then
    mkdir $Da1
    fi
    cd $Da1
    he.run ../../main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq strain $ref lim $Kas $mus $a3 $a4 $b1 $b2 '<' $off '>' Da$Da1.msg
else
    Da1=1.144
    bash run.sh $Da1

fi
