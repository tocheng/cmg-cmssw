#!/bin/sh

dir=$1
queue="1nw"

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
      cp tree.root /eos/cms/store/user/tocheng/X2l+MET+jets/Run2016/$dir/$dataset/vvTreeProducer_tree_${index}.root;
      echo root://eoscms.cern.ch//eos/cms/store/user/tocheng/X2l+MET+jets/Run2016/$dir/$dataset/vvTreeProducer_tree_${index}.root > tree.root.url
      rm tree.root
      cd -
    else 
      #if [ ! -f $dd/vvTreeProducer/tree.root.url ];
      dataset=${dd%%_Chunk*};
      index=${dd##*_Chunk};
      if [ ! -f /eos/cms/store/user/tocheng/X2l+MET+jets/Run2016/$dir/$dataset/vvTreeProducer_tree_${index}.root ];
      then
         rr=`bjobs -o "name" -J ${dd} | grep -v JOB_NAME | grep ${dd}`;
         if [ "$rr" == "$dd" ];
         then
            echo "  job $dd is still running";
         else
           echo "job $dir/$dd needs to resubmit";
           cd $dd;
           echo "  bsub -q $queue -J $dd  < batchScript.sh" ;
           rm cmsRun*
           rm *.log
           rm -rf LSF*
           rm -rf Loop*
           bsub -q $queue -J $dd  < batchScript.sh &
           cd -
         fi
      fi;
    fi;

done 

cd ../

