set -eu

Vr=0.642
Ka=0.5
Kga=1
Kv=1
Ke=0

Kb=0.001
C0=0
Kad=$(echo $Kb | awk '{print $1*0.63662}')
#echo $Kad
DA0D=0
D=0.000898798148042
Da0=0
pi=3.141592653589793115997964
A=$(echo  $pi | awk '{print $1*4.0}')
#echo $A
end=200000
freq=500

off=$(he.path)/sph/laplace/Nt1280.off

if test $# -ne 0
then
    Da1="$1"
    Da0=$(echo $Da1 | awk '{print $1/100}')
    echo '***' Da1=$Da1 '***' Da0 = $Da0 '***'

    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    #echo $Da0, $Da1, $Kb, $C0, $Kad, $DA0D

    na=$(echo ${Da1}_long)
    echo $na
    if [ ! -d $na ]; then
    mkdir $na
    fi
    cd $na
    ../../../../main meyer_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq < $off > Da$Da1.msg
else
    for i in `seq 0 5`;
    do
	Da1=$(echo $i | awk '{print (0.2+$1*0.02)}')
	#echo $Da1
	bash run_long.sh $Da1
    done

fi
