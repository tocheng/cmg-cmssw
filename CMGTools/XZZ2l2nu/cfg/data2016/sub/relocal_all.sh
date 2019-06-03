#!/bin/sh

dir=$1
queue="1nd"


#n1=`rootls ${dd}/*/*.root | grep root | wc -l`;
#n2=`ls -l ${dd}/* | grep pck |wc -l`;

cd $dir
list=`ls -1`
for dd in $list;
do
    dataset=${dd%%_Chunk*};
    #echo "  dataset is $dataset";
    #echo "### check job $dd"
    #rr=`bjobs -o "name" -J ${dd} | grep -v JOB_NAME | grep ${dd}`;
    if [ -f $dd/vvTreeProducer/tree.root ]; 
    then
      echo "  job $dd needs to copy to eos space";
      cd $dd/vvTreeProducer/;
      index=${dd##*_Chunk};
      echo $index;
      eos cp tree.root /eos/cms/store/user/tocheng/X2l+MET+jets/$dir/$dataset/vvTreeProducer_tree_${index}.root;
      echo root://eoscms.cern.ch//eos/cms/store/user/tocheng/X2l+MET+jets/$dir/$dataset/vvTreeProducer_tree_${index}.root > tree.root.url
      rm tree.root
      cd -
    else 
      if [ ! -f $dd/vvTreeProducer/tree.root.url ];
      then
           echo "  job $dd needs to resubmit";
           cd $dd;

           python $CMSSW_BASE/src/PhysicsTools/HeppyCore/python/framework/looper.py pycfg.py config.pck --options=options.json > ${dd}.log 2>&1 &

           cd -
      fi;
    fi;

done 

cd ../

