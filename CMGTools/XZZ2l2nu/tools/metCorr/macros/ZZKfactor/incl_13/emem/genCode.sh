#!/bin/sh

input=$1

name=`echo $input | sed "s/\./_/g"`;

echo "{ ";
echo "  TFile* file = TFile::Open(\"${name}.root\", \"recreate\");";
echo "  TH1D* hh = new TH1D(\"$name\", \"$name\", 100, 0, 500);"; 

while read -r line
do
  bin=`echo $line | awk {'print $1'}`;
  var=`echo $line | awk {'print $2'}`;
  err=`echo $line | awk {'print $3'}`;
  echo "  hh->SetBinContent(hh->FindBin($bin-0.1),$var);"; 
  echo "  hh->SetBinError(hh->FindBin($bin-0.1),$err);"; 
done < $input

echo "  hh->Write();";
echo "}";
  

