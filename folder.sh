#!/bin/bash
NX=101
NY=41
KAPPA=(5.0);
#RUN=(for number in {1..10}; do echo -ne "$number " done); # to make different runs you can toggle the run number

for K in ${KAPPA[@]};
do
  for R in {1..10};
  do
                echo "**************************START SIMULATION LAUNCH***************************************"
                echo "NX $NX NY $NY Epsilon $E Kappa $K Run $R"

                L="../Sim_dump_ribbon/L${NX}"
                #echo $L
                W="../Sim_dump_ribbon/L${NX}/W${NY}"
                #echo $W
                kappa="../Sim_dump_ribbon/L${NX}/W${NY}/k${K}"
                #echo $kappa
                run="../Sim_dump_ribbon/L${NX}/W${NY}/k${K}/r${R}"
                #echo $run

                if [ ! -d "$L" ]; then
                        mkdir $L
                fi
                if [ ! -d "$W" ]; then
                        mkdir $W
                fi
                if [ ! -d "$kappa" ]; then
                        mkdir $kappa
                fi
                if [ ! -d "$run" ]; then
                        mkdir $run
                fi
    done
done
