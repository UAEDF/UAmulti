#!/bin/bash

run_one(){
  acc=$1
  cen=$2
  hf=$3

  root  -l -q ../macro/BuildLibDico.C \
      NCHmakeCorrections.C+"($typeMC , \"$mcfile\" , \"$datafile\" , \"$output\" , \"cut$acc\" , \"$cen\" , \"HF$hf\" , $useData , $hyp , $niter , $syst , $syst_sign, $unfVersion, $zeroBias )"

}



###################################################
#                   defaults			  #
###################################################

hyp=1
useData=1 # 0: run MCcorrMC
niter=5
syst=0
syst_sign=0

unfVersion=1

Emc=7
Edata=7

typeMC=62
tr="genTr"
noweight="_allEffs" #"_noweight_allEffs" || "_allEffs"
usedata="_"
track_out=""

zeroBias=1 # turn to 0 for v40
inputver="v40_genTr"
outputver="v41_genTr" #"v40NoWeight"

###################################################
#                   File			  #
###################################################

nrunmc="5000000"
nrundata="342220"
dir="../macro/outputs_full/$inputver/"

mcstr="${typeMC}_${tr}_E_${Emc}_${nrunmc}${noweight}"
mcfile="${dir}output_MC${mcstr}.root"
datafile="${dir}output_data_${tr}_E_${Edata}_${nrundata}.root"
# datafile="${dir}output_MC60_${tr}_E_${Emc}_${nrunmc}${noweight}.root"   ### line for unf of MC with other MC
output="../macro/unfold_outputs/${outputver}/unf_MC${typeMC}${usedata}${noweight}${track_out}"



###################################################
#                   Running			  #
###################################################

acc_list="`seq 0 5`"
cen_list="nocut MBUEWG ALICE ATLAS1 ATLAS2 ATLAS6"
hf_list="`seq 0 1`"

for acc in $acc_list;do
  for cen in $cen_list;do
    for hf in $hf_list;do
      run_one $acc $cen $hf
    done
  done
done

#run_one 0 nocut 0
