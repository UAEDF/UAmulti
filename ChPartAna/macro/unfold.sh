#Bash Unfolding
#   nohup bash unfold.sh 0 0 true 0 > logUnf0_0_true.txt &
#   nohup bash unfold.sh 0 0 true 1 > logUnf0_0_true_AllSys.txt &

unfall() 
{   #true false is splitting up atlas1,2,6 from nocut,MBUEWG, ALICE, because there is otherwise a memory problem (max2GB limit)
    unf  $1 $2 $3 $4 true
    unf  $1 $2 $3 $4 false
}

unf()
{
    echo "unfolding cut $1 till $2"
    stringDir=v22/
    noWeight=false;
    for ((i = $1; i<=$2; i+=1));
    do 
       echo "unfolding cut $i "
       echo '----'
       echo "MC10"
       echo '----'      
       if [ $4 = 0 ]; then      
                                                                            #string         MC  HF   nMC      ndata                   nSys +/-
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 0, 0 , $5, $noWeight )" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 0, 0 , $5, $noWeight )" -q 
       else           
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 100,  1 , $5, $noWeight )" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 100, -1 , $5, $noWeight )" -q        
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 100,  1 , $5, $noWeight)" -q         
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 100, -1 , $5, $noWeight)" -q          
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 302,  1 , $5, $noWeight)" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 302, -1 , $5, $noWeight)" -q        
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 302,  1 , $5, $noWeight)" -q         
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 10, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 302, -1 , $5, $noWeight)" -q     
       fi
       echo '----'
       echo "MC15"
       echo '----'       
       if [ $4 = 0 ]; then      
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 0, 0 , $5, $noWeight)" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 0, 0 , $5, $noWeight)" -q 
       else           
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 100,  1 , $5, $noWeight)" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 100, -1 , $5, $noWeight)" -q        
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 100,  1 , $5, $noWeight)" -q         
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 100, -1 , $5, $noWeight)" -q          
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 302,  1 , $5, $noWeight)" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 302, -1 , $5, $noWeight)" -q        
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 302,  1 , $5, $noWeight)" -q         
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 15, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 302, -1 , $5, $noWeight)" -q      
       fi
       echo '----'
       echo "MC31"
       echo '----'       
       if [ $4 = 0 ]; then      
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 0, 0 , $5, $noWeight)" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 0, 0 , $5, $noWeight)" -q 
       else           
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 100,  1 , $5, $noWeight)" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 100, -1 , $5, $noWeight)" -q        
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 100,  1 , $5, $noWeight)" -q         
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 100, -1 , $5, $noWeight)" -q          
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 302,  1 , $5, $noWeight)" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 302, -1 , $5, $noWeight)" -q        
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 302,  1 , $5, $noWeight)" -q         
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 31, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 302, -1 , $5, $noWeight)" -q  
       fi
       echo '----'                                                    
       echo "MC60"
       echo '----'       
       if [ $4 = 0 ]; then
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 0, 0 , $5, $noWeight)" -q
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 0, 0 , $5, $noWeight)" -q      
       else
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 100,  1 , $5, $noWeight)" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 100, -1 , $5, $noWeight)" -q        
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 100,  1 , $5, $noWeight)" -q         
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 100, -1 , $5, $noWeight)" -q          
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 302,  1 , $5, $noWeight)" -q       
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 0, 5000000, 342220, $3, $i, 7.0, 7.0, 302, -1 , $5, $noWeight)" -q        
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 302,  1 , $5, $noWeight)" -q         
          root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"$stringDir\", 60, 1, 5000000, 342220, $3, $i, 7.0, 7.0, 302, -1 , $5, $noWeight)" -q  
       fi
    done 
}

#Run different line for each m machine
#unf cut_start cut_end  true/false   (true or false for useData or MCcorrMC...)
unfall $1 $2 $3 $4 

echo "job finished"   
#   nohup bash unfold.sh 0 0 true 0 > logUnf0_0_true.txt &
#   nohup bash unfold.sh 1 1 true 0 > logUnf1_1_true.txt &
#   nohup bash unfold.sh 2 2 true 0 > logUnf2_2_true.txt &
#   nohup bash unfold.sh 3 3 true 0 > logUnf3_3_true.txt &
#   nohup bash unfold.sh 4 4 true 0 > logUnf4_4_true.txt &
#   nohup bash unfold.sh 5 5 true 0 > logUnf5_5_true.txt &

#   nohup bash unfold.sh 0 5 true 1 > logUnf0_5_true_AllSys.txt &

# for MConMC
#   nohup bash unfold.sh 0 0 false 0 > logUnf0_0_false.txt &
#   nohup bash unfold.sh 1 1 false 0 > logUnf1_1_false.txt &
#   nohup bash unfold.sh 2 2 false 0 > logUnf2_2_false.txt &
#   nohup bash unfold.sh 3 3 false 0 > logUnf3_3_false.txt &
#   nohup bash unfold.sh 4 4 false 0 > logUnf4_4_false.txt &
#   nohup bash unfold.sh 5 5 false 0 > logUnf5_5_false.txt &

