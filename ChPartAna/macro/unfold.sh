  #Bash Unfolding
unf ()
{
    echo "unfolding cut $1 till $2"
    for ((i = $1; i<=$2; i+=1));
    do 
        echo "unfolding cut $i "
       # root -l -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5\", 60, -1, -1, $3, $i )" -q
      # root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5\", 60, 500000, 500000, $3, $i )" -q
      echo'-----'
       echo "MC10"
       echo '----'      
       root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5b\", 10, 0, 2000000, 342220, $3, $i )" -q    
       root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5b\", 10, 1, 2000000, 342220, $3, $i )" -q
       echo'-----'
       echo "MC15"
       echo '----'
       root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5b\", 15, 0, 2000000, 342220, $3, $i )" -q
       root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5b\", 15, 1, 2000000, 342220, $3, $i )" -q
       echo'-----'
       echo "MC31"
       echo '----'       
       root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5b\", 31, 0, 1886500, 342220, $3, $i )" -q       
       root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5b\", 31, 1, 1886500, 342220, $3, $i )" -q
       echo'-----'
       echo "MC60"
       echo '----'
       root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5b\", 60, 0, 2000000, 342220, $3, $i )" -q
       root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5b\", 60, 1, 2000000, 342220, $3, $i )" -q
       # root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v5\", 15, -1, -1, false, $i )" -q
    done 
}

#Run different line for each m machine
#unf cut_start cut_end  true/false   (true or false for useData or MCcorrMC...)
unf $1 $2 $3

echo "job finished"   
#   nohup bash unfold.sh 0 0 true > logUnf0_0_true.txt &
