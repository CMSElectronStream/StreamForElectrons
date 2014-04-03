#!/usr/bin/python 

import os
import glob
import math
import array
import ntpath
import sys
import subprocess

from subprocess import Popen
from optparse import OptionParser
from array import array

############################################
#              Job steering                #
############################################

parser = OptionParser()
parser.add_option('-s', '--listofsamples',action="store", type="string" ,dest="listofsamples",default="",help="file with the list of samples to be processed --> just the path on eos")
parser.add_option('-t', '--jobtemplate',  action="store", type="string", dest="jobtemplate"  ,default="step2_RAW2DIGI_L1Reco_RECO_template.py",help='name of the template cfg to be run')
parser.add_option('-p', '--outputpath',   action="store", type="string", dest="outputpath"   ,default="",help='output path to the eos directory where to store the files')
parser.add_option('-f', '--outputfilename', action="store", type="string", dest="outputfilename", default="step2reco", help='basic root of the output file name')
parser.add_option('-j', '--jobmodulo',    action="store", type=int,      dest="jobmodulo",        default=1, help='how many input files have to be processed in one job')
parser.add_option('-r', '--isalcastream',  action="store", type=int,      dest="isalcastream",     default=1, help='run on the alca stream output')
parser.add_option('-q', '--queque',       action="store", type="string", dest="queque",           default="cmscaf1nd", help='queque of the batch system at cern')


(options, args) = parser.parse_args()


if __name__ == "__main__":

 inPath = os.getcwd(); 
 os.chdir(inPath);
 

 sampleJobListFile = "lancia.sh";
 SAMPLEJOBLISTFILE = open(sampleJobListFile,"w");

 ## open the list of sample directory 
 with open(options.listofsamples) as input_list:
  for line in input_list:
   name = line.split();
   if name[0] == "#" : continue ;
   print "base dir ",name [0]," sub dir ",name[1] ;
   
   os.system("rm -r "+name[1]+"\n") ;
   os.system("mkdir "+name[1]+"\n") ;
      
  
   tempfilelist = "./List_"+name[1]+".txt" ;
   os.system("cmsLs -R "+name[0]+"/"+name[1]+" | grep root | awk '{print $5}' > "+tempfilelist+"\n") ;
   
   totNumber = 0;
   jobNumber = 0;
  
   with open (tempfilelist) as file_list:
    for file in file_list:
      totNumber = totNumber +1 ;

   jobNumber = int(totNumber/options.jobmodulo);
   if(totNumber%options.jobmodulo != 0):    
     jobNumber = jobNumber+1;
    
   print "NumberOfJobs = ",jobNumber,"\n";
    
 
   ################
   # loop over jobs
   ################
    
   for jobId in range(jobNumber):
    
    jobDir = name[1]+"/JOB_"+str(jobId) ;
    os.system("mkdir "+jobDir+" \n") ;
    
    tempBjob = jobDir+"/bjob_"+str(jobId)+".sh" ;
    command = "touch "+tempBjob ;
    os.system(command) ;

    command = "chmod 777 "+tempBjob ;
    os.system (command) ;
        
    tempo1 = "./tempo1" ;
    if options.isalcastream == 1 and not "_stream" in options.outputfilename : options.outputfilename = options.outputfilename + "_stream" ;
    os.system ("cat "+options.jobtemplate+" | sed -e s%MYOUTPUTFILE%"+options.outputfilename+"_"+str(jobId)+".root"+"%g > "+tempo1);
    it = 0;
    jobfilelist = "" ;
    with open (tempfilelist) as file_list_2: 
     for jobfile in file_list_2:
      namefile = jobfile.split();
      if it >= (jobId-1)*options.jobmodulo and  it < jobId*options.jobmodulo :
        jobfilelist = jobfilelist+"@"+namefile[0]+"@,";
      it = it +1
     tempo2 = "./tempo2" ;
     if jobfilelist == "" : continue ;
     os.system ("cat "+tempo1+" | sed -e s%MYINPUTFILE%"+jobfilelist+"%g | tr \"@\" \"'\" > "+tempo2) ;
     jobfilelist = "" ;
 
     jobcfgfile = jobDir+"/"+options.jobtemplate.replace("_template","") ;
     os.system ("mv "+tempo2+" "+jobcfgfile) ;
     os.system ("rm ./tempo*") ;
                    
    ######################
    # make job files
    ######################
    
    SAMPLEJOBFILE = open (tempBjob,"w");

    command = "#!/bin/sh" ;
    SAMPLEJOBFILE.write(command+"\n");

    command = "cd "+inPath ;
    SAMPLEJOBFILE.write(command+"\n");

    command = "export SCRAM_ARCH=slc5_amd64_gcc472 " ;
    SAMPLEJOBFILE.write(command+"\n");
    
    command = "eval `scramv1 runtime -sh`" ;
    SAMPLEJOBFILE.write(command+"\n");
    
    command = "cd -" ;
    SAMPLEJOBFILE.write(command+"\n");

    command = "cp "+inPath+"/"+jobcfgfile+" ./"
    SAMPLEJOBFILE.write(command+"\n");

    command = "cmsMkdir "+options.outputpath+"/"+name[1];
    SAMPLEJOBFILE.write(command+"\n");

    command = "cmsRun "+options.jobtemplate.replace("_template","") ;
    SAMPLEJOBFILE.write(command+"\n");

    command = "cmsStage "+options.outputfilename+"_"+str(jobId)+".root "+options.outputpath+"/"+name[1];
    SAMPLEJOBFILE.write(command+"\n");


    ############
    # submit job
    ############

    command = "bsub -cwd "+inPath+"/"+jobDir+" -q "+options.queque+" "+inPath+"/"+tempBjob;
    SAMPLEJOBLISTFILE.write(command+"\n");
    
 
 os.system("rm List_*.txt") ;
