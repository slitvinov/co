end=0026000.off
lambda=5.6
gamdot=0.01
dt=0.01

lam=$(printf "%.1f" $lambda)
echo "lam="$lam
gam=$(printf "%.2f" $gamdot)
echo "gam="$gam
ddt=$(printf "%.2f" $dt)
echo "ddt="$ddt

for i in `seq 0 7`;
do
    v=$(echo $i | awk '{print (0.6+$i*0.05)}')
    vv=$(printf "%.2f" $v)
    echo "vv="$vv
    echo "copying ../lam${lam}_v${vv}_g${gam}_dt${ddt}/$end"
    cp ../lam${lam}_v${vv}_g${gam}_dt${ddt}/$end lam${lam}_v${vv}_g${gam}_dt${ddt}.off
done
