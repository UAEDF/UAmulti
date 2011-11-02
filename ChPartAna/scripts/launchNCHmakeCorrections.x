#!/bin/bash
#
#            parameters:      MC NoWght ZeroBias Trackign (Wght=0)
#
# bash launchNCHmakeCorrections.x 62 0 1 genTr
#
# no tracking parameter==mixedTr
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

typeMC=$1 # 62
useNoWeight=$2
zeroBias=$3 # turn to 0 for v40
tr=$4 # "genTr"

noweight="_allEffs" #"_noweight_allEffs" || "_allEffs"
usedata="_"
track_out=""

inputver="v40"
outputver="v41" #"v40NoWeight"
if [ $zeroBias -eq 1 ];then outputver="v40" ; fi

if [ "$tr" == "" ];then tr="mixedTr";
else 
  inputver=$inputver"_"${tr}
  outputver=$outputver"_"${tr}
fi
if [ $useNoWeight -eq 1 ];then
  inputver=$inputver"NoWeight"
  outputver=$outputver"NoWeight"
  noweight="_noweight_allEffs"
fi

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
