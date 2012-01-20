#ifndef _PLOTS_STRUCT_
#define _PLOTS_STRUCT_ 

struct PlotInfo{
  TString Title;  
  TString SubTitle;   
  TString Xaxis;
  TString Yaxis;
  double Xmin;
  double Xmax;
  double Ymin;
  double Ymax;
  bool norm;
  bool norm2;
  bool Logy;
  bool Logx;
  bool scalewidth;
  vector<TString> legend;

  //Constructor
  PlotInfo(){
    Title = "Title";
    SubTitle = "";
    Xaxis = "Xaxis";
    Yaxis = "Yaxis";
    Xmin = 0.;
    Xmax = 0.;
    Ymin = 0.;
    Ymax = 0.;
    norm = false;
    norm2 = false;
    Logy = false;
    Logx = false;
    scalewidth = false;
  };
};

#endif
