for cut in `seq 0 4`;do
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF0_ATLAS1_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF1_ATLAS1_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF0_ATLAS2_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF1_ATLAS2_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF0_ATLAS6_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF1_ATLAS6_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF0_ALICE_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF1_ALICE_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF0_MBUEWG_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF1_MBUEWG_RECO_cut$cut\")";   
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF0_nocut_RECO_cut$cut\")";
  root ../macro/BuildLibDico.C -b -q -l NCHMainSys.C+"(\"_partfull_HF1_nocut_RECO_cut$cut\")";
done
