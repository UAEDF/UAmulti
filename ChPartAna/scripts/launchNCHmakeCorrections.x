#!/bin/bash

run_one(){
  acc=$1
  cen=$2
  hf=$3

  root  -l -q ../macro_sten/BuildLibDico.C \
      NCHmakeCorrections_new.C+"($typeMC , \"$mcfile\" , \"$datafile\" , \"$output\" , \"cut$acc\" , \"$cen\" , \"HF$hf\" , $useData , $hyp , $niter , $syst , $syst_sign )"

}



###################################################
#                   defaults			  #
###################################################

hyp=1
useData=1
niter=5
syst=0
syst_sign=0

Emc=7
Edata=7

typeMC=31
tr="ferncTr"
noweight=""
usedata=""
track_out=""

ver="v25_test"



###################################################
#                   File			  #
###################################################

nrunmc=1886500
nrunmc=5000000
nrundata=342220
dir="../macro/outputs_full/$ver/"

mcstr="${typeMC}_${tr}_E_${Emc}_${nrunmc}${noweight}"
mcfile="${dir}output_MC${mcstr}.root"
datafile="${dir}output_data_${tr}_E_${Edata}_${nrundata}.root"
output="../macro/unfold_outputs/${ver}/unf_MC${typeMC}${usedata}${noweight}${track_out}"



###################################################
#                   Running			  #
###################################################

acc_list="`seq 0 5`"
cen_list="nocut MBUEWG ALICE ATLAS1 ATLAS2 ATLAS6"
hf_list="`seq 0 0`"

for acc in $acc_list;do
  for cen in $cen_list;do
    for hf in $hf_list;do
      echo run_one $acc $cen $hf
    done
  done
done

run_one 0 nocut 0
