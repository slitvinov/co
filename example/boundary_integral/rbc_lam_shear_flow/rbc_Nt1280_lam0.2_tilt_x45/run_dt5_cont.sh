set -eu

Da1=0.143
pi=3.141592653589793115997964
A=12.5663706144
V=2.68919777043
#v=0.641998677173
v=0.642
R=1

Nt=1280
Kc0=20000

Kb=1.0
C0=0.0
alpha=$(echo $pi | gawk '{print 2.0/$1}')
Kad=$(echo $Kb $alpha | gawk '{print $1*$2}')
D=0.000898798148042

mub=139.260575205
Ka=$(echo $mub | awk '{print $1*2}')
a3=-2
a4=8
b1=0.7
b2=0.75

rho=1.0
eta_in=645.928652122
lambda=0.2
eta_out=$(echo ${eta_in} $lambda | awk '{print $1/$2}')
#echo "eta_in : "$eta_in
#echo "eta_out: "$eta_out
#exit
gamdot0=0.00143923833018
dt0=0.01
tscale=5

start=0
end=20000000
freq_out=1000
freq_stat=100

#number of threads
NT=1
export CO_ARG="-W 120:00"
     
if test $# -ne 0
then

    num="$1"

    #convert num to integer inum
    inum=$(printf "%.0f" $num)

    gamdot=$(echo $gamdot0, $num | awk '{print $1*$2}')
    dt=$(echo $dt0 $num $tscale | awk '{print $1*$3/$2}')

    if [ $inum -le 100 ]; then
	Kc=$Kc0
    else
	Kc=$(echo $inum $Kc0 | awk '{print $1*$2/100}')	
    fi

    Kga=$Kc
    Kv=$Kc

    Re=$(echo $gamdot $R $rho ${eta_out} | awk '{print $1*$2*$3/$4}')
    echo Re=$Re
    
    ref_file=../ref/ref_v0.95.off
    
    V=$(echo $A, $v | awk '{print 0.09403159725795977*$1**1.5*$2}')
    echo "A, V, v   :" $A, $V, $v
    
    Da0=$(echo $Da1 | awk '{print $1/100}')
    DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print $4*$7}')
    DA0D=$(echo $DA0, $D | awk '{print $1/$2}')

    flam=$(printf "%.1f" $lambda)
    fdt=$(printf "%.6f" $dt)
    
    echo "flam="$flam
    echo "num="$num
    echo "fdt="$fdt
    echo "Kc="$Kc

    folder=Da${Da1}_lam${flam}_num${num}_dt${fdt}_Kc${Kc}
    folder_cont="Da${Da1}_lam${flam}_num${num}_dt${fdt}_Kc${Kc}_cont"
    
    if [ ! -d $folder_cont ]; then
	mkdir $folder_cont
    fi
    cd $folder_cont

    in_file=$(ls ../${folder}/*.off | tail -n 1)
    
    fname=`basename $in_file`
    fnum=${fname%.*}
    start=$(echo $fnum | sed 's/^0*//')

    echo "folder:   "$folder
    echo "in_file:  "$in_file
    echo "ref file: "$ref_file
    echo "start:    "$start

    co.run ../../main8 volume $V $Kv garea $A $Kga juelicher_xin $Kb $C0 $Kad $DA0D strain $ref_file lim $Ka $mub $a3 $a4 $b1 $b2 cortez_zero $R $D $rho ${eta_out} $lambda $gamdot $dt $start $end $freq_out $freq_stat '<' $in_file '1>/dev/null' '2>/dev/null'
      
    
else

    inc=0.1
    for i in `seq 10 2 20`; #6
    do
	num=$(echo $i $inc | awk '{print $1*$2}')
	bash run_dt5_cont.sh $num
    done


fi
    
