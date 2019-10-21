h0 = 10.6093909724609
h1 = 11.9543403107842

f0 = 1753.94243634426
f1 = -0.188361056855734
f2 = 0.000534150874992711
f3 = -5.94422618701948e-07

g0 = 1389.34545245195
g1 = 0.345793148161431
g2 = 0.00892003871520631
g3 = -2.22293197336991e-05

f(x) = f0 + f1*x + f2*x*x + f3*x*x*x
g(x) = g0 + g1*x + g2*x*x + g3*x*x*x
h(x) = g(x) + (f(x) - g(x))*(cos(2*pi*x/h0 + h1) + 1)/2

set sample 10000, 10000
plot [20:] "stat.dat" u 3:13 w dots, h(x)
