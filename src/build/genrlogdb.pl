#/********************************************************************16**
#
#            (c) Copyright 2012 by RadiSys Corporation. All rights reserved.
#
#     This software is confidential and proprietary to RadiSys Corporation.
#     No part of this software may be reproduced, stored, transmitted, 
#     disclosed or used in any form or by any means other than as expressly
#     provided by the written Software License Agreement between Radisys 
#     and its licensee.
#
#     Radisys warrants that for a period, as provided by the written
#     Software License Agreement between Radisys and its licensee, this
#     software will perform substantially to Radisys specifications as
#     published at the time of shipment, exclusive of any updates or 
#     upgrades, and the media used for delivery of this software will be 
#     free from defects in materials and workmanship.  Radisys also warrants 
#     that has the corporate authority to enter into and perform under the 
#     Software License Agreement and it is the copyright owner of the software 
#     as originally delivered to its licensee.
#
#     RADISYS MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
#     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
#     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
#     MATERIALS.
#
#     IN NO EVENT SHALL RADISYS BE LIABLE FOR ANY INDIRECT, SPECIAL,
#     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
#     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
#     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
#     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
#     OF THE POSSIBILITY OF SUCH DAMAGE.
#
#                       Restricted Rights Legend:
#
#     This software and all related materials licensed hereby are
#     classified as "restricted computer software" as defined in clause
#     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
#     developed entirely at private expense for nongovernmental purposes,
#     are commercial in nature and have been regularly used for
#     nongovernmental purposes, and, to the extent not published and
#     copyrighted, are trade secrets and confidential and are provided
#     with all rights reserved under the copyright laws of the United
#     States.  The government's rights to the software and related
#     materials are limited and restricted as provided in clause
#     52.227-19 of the FAR.
#
#                    IMPORTANT LIMITATION(S) ON USE
#
#     The use of this software is limited to the use set
#     forth in the written Software License Agreement between Radisys and
#     its Licensee. Among other things, the use of this software
#     may be limited to a particular type of Designated Equipment, as 
#     defined in such Software License Agreement.
#     Before any installation, use or transfer of this software, please
#     consult the written Software License Agreement or contact Radisys at
#     the location set forth below in order to confirm that you are
#     engaging in a permissible use of the software.
#
#                    RadiSys Corporation
#                    Tel: +1 (858) 882 8800
#                    Fax: +1 (858) 777 3388
#                    Email: support@trillium.com
#                    Web: http://www.radisys.com 
# 
#*********************************************************************17*/
#!/usr/bin/perl -w
#use strict;
use POSIX;
use Time::HiRes qw(usleep);
local $| = 1;

if(($ARGV[1] eq "INTEL") or ($ARGV[1] eq "QCOM"))
{
   if ($#ARGV == 3 ) 
   {
      print "Make DB Script ERROR: More than two arument is passed to script\n";
      exit;
   }
}
else
{
   if ($#ARGV == 4 ) 
   {
      print "Make DB Script ERROR: More than two arument is passed to script\n";
      exit;
   }
}
########################################################### 
# Reading File name and creating DB file name.
#my $OrgFileName   = $ARGV[0];  # Copied File name in to a varible.
#my @spltname      = split('/', $OrgFileName);
my $numelem       = $#spltname;
my $fileName      = $spltname[$numelem]; 

#variable declaration.
my @arryOfLogId      = 0;
#my $isModuleIdFound  = 0;
#my $isFileIdFound    = 0;
my $moduleId         = 0;
my $fileId           = 0;
my $moduleName       = 0;
my $lineNum          = 0;
my $test             = 0;
my $printString      = 0;
my @varibles         = 0;
my $logLvl           = 0;
my $splArgName       = 0;
my $splArgVal        = 0;
my $tokenID          = 0;
my $hexTokenID          = 0;
my $CStart           = 0;
my $CPattern         = 0;
my $CVarible         = 0;
my $CString          = 0;
my $CEnd             = 0;
my $NewVarible       = 0;
my $delimit          = '%8ld';
#my $isModuleNameFound = 0;
my $greatestTokenID  = 0;
my $DBFILE;
my $tmpfile;
my $tmpfile1;
my $dbentry;

###########################################################
# Declare the subroutines
sub trim($);
sub ltrim($);
sub rtrim($);
sub findDup;

# Perl trim function to remove whitespace from the start and end of the string
sub trim($)
{
   my $string = shift;
   $string =~ s/^\s+//;
   $string =~ s/\s+$//;
   return $string;
}

# Left trim function to remove leading whitespace
sub ltrim($)
{
   my $string = shift;
   $string =~ s/^\s+//;
   return $string;
}

# Right trim function to remove trailing whitespace
sub rtrim($)
{
   my $string = shift;
   $string =~ s/\s+$//;
   return $string;
}

my $prj = $ARGV[0];
#print "$prj\n";
my $tmp = $ARGV[1];
#print "$tmp\n";
my @dbFilesDir = ($ARGV[2]);
if(($tmp eq "INTEL") || ($tmp eq "QCOM"))
{
   @dbFilesDir = ($ARGV[2]);
}
else
{
   @dbFilesDir = ($ARGV[2],$ARGV[3]);
}

if (! -f "$prj.logdb")
{
   `touch $prj.logdb`;
   sleep(2);
   foreach my $arg (@dbFilesDir)
   {
      chomp($arg);
      print "*******Files are present in directory --> $arg*******\n";
      `touch $arg/*.i`;
   }
}

`cp -f $prj.logdb $prj.logdb.bak`;

#print "$prj1\n";

foreach my $arg (@dbFilesDir) {
  
   my $percentage = 0;
   my $filecount = 0;
   my $c = 0;

	print "Creating Database for $arg \n";
   #@filesindir = `ls $prj1/*.i`;
   #find ./obj/cpuh/ -newer rlog.logd
   @filesindir = `find $arg/*.i -newer $prj.logdb`;
   foreach $file (@filesindir) {
      $filecount++;
   }
   print "The number of newer .i files in $arg directory is: $filecount\n";

	print "please wait ... generating $prj.logdb..perecentage done:   ";
   
   #@files =  get_source_files($arg);
   @files =  `find $arg/*.i -newer $prj.logdb`;
	foreach $file (@files) {
      print ("\b" x (length($percentage) +1));
      chomp($file);
   	#parse_preproc_file($arg . '/' . $file);
   	parse_preproc_file($file);
      ++$c;      
      $percentage = ($c*100)/$filecount;
      $percentage = ceil($percentage);
      print "$percentage%";
      #usleep(100000);
      `cat $file.db >> $prj.logdb.bak 2>/dev/null`;
      `rm -f $file.db $file.bak`;
	}
	print "Database created for $arg \n";
}
`cp -f $prj.logdb.bak $prj.logdb`;
`rm -f $prj.logdb.bak`;
sub parse_preproc_file {
	my $OrgFileName = shift;
   chomp($OrgFileName);
	my $generatedb = 0;
	my $dbFileName  = sprintf("%s%s", $OrgFileName,".db");
	my $logNum           = 0;

	my $isModuleIdFound = 0;
	my $isFileIdFound = 0;
	my $isModuleNameFound = 0;
	$logNum = 0;
	$fileId = 0;


   #s/^[^\n]*\(logLev[0-9E].*\}\)[^\n]*/\1/
   #s/^[^\n]*\(RLOG_MODULE_ID.*\}\)/ \1/g
   #s/\\\n/ /gp
   # system("sed -i -e '/^#/d' -e '/logLev[0-4EHS]/{
   #system("sed -i  's/logLev[0-4EHS][ ]*\(/\1\(/g' $OrgFileName");
	#system("sed -i  's/\(logLev[0-4EHS]\)[ ]*\(\(\)/\1\3/g' $OrgFileName");
	#   system("sed -i -e 's/RLOG_ENDMARKER/\\\n/g' $OrgFileName");
	#system("sed -n -i -e 's/\(logLev[0-4EHS]\)[ ]*\(\(\)/\1\3/gp' $OrgFileName");
	##########################################################
   # Opening source.i file and DB file        
	#   print "FILE:$OrgFileName\n";
		`cp -f $OrgFileName $OrgFileName.bak`;
   if(($tmp eq "INTEL") || ($tmp eq "QCOM"))
   {
      #print "Removing # for $tmp";
	  system("sed -i '/^#/d' $OrgFileName");
   }
   else
   {
      #print "Don't Remove # for BRCM";
	  #system("sed -i '/^#/d' $OrgFileName");
   }
	
	open FILE, "<$OrgFileName" or die $!;
   open my $OUTPFILE, '>', "$OrgFileName.tmp" or die "Can't write new file: $!";
   my $fromFound =0;
   
   
   while(<FILE>)
   {  
   
	   if (( $_ =~ /logLev[0-4EHS]/ ) && ( $_ !~  /}[;]*/) && ( $_ !~  /R_LOG_LEVEL/))
	   { 
	      #$_ =~ s/(.*)(logLev0)(\s*)(.*)/$1+$2+$4/g;
	      #s/.*(logLev[0-4ehs]) +/$1/i;
          #print ;
	       #<STDIN>;
	       $fromFound = 1;
	       #print $OUTPFILE "/*int AARTI2*/";
	       $concatStr=$_;
	       chomp($concatStr);
          next;
      }
   
      if( ($_ =~ /}[;]*/) && ($fromFound == 1) )
      {  
         $concatStr =~ s/(.*)(logLev[E0-4HS]) +(.*)/$1$2$3/g;
         print $OUTPFILE $concatStr;
	      print $OUTPFILE $_;
	      $fromFound = 0;
	      #print $OUTPFILE "/*int AARTI3*/";
	      next;
      }
	   if ($fromFound == 1)
	   {
	      chomp($_);
	      $concatStr = $concatStr.$_;
	   }
      else
      {
         #print $OUTPFILE "/* AARTI4*/";
         $_ =~ s/(.*)(logLev[E0-4HS]) +(.*)/$1$2$3/g;
         print $OUTPFILE $_;
      }
   }
	close(FILE);
	close($OUTPFILE);
		`mv -f $OrgFileName.tmp $OrgFileName`;
	#open FILE, "<$OrgFileName" or die $!;
   #open my $OUTPFILE1, '>', "$OrgFileName.tmp1" or die "Can't write new file: $!";
	
	#while(<FILE>)
	#{
	
	#         $_ =~ s/(.*)(logLev[E0-4HS]) +(.*)/$1$2$3/g;
   #      print $OUTPFILE1 $_;
   #   }
   #close(FILE);
   #close($OUTPFILE1);
   #	`mv -f $OrgFileName.tmp1 $OrgFileName`;
   
  	open(INFILE, "< $OrgFileName") || die("Make DB Script ERROR: Cannot open file $OrgFileName for parse");
	open $tmpfile, "> tmp.i" || die("Data Base ERROR: Cannot Create temporary file");
	open ($dbfile, '>' , "tmp.db") || die("Data Base ERROR: Cannot create the file $dbFileName");
##########################################################
# Read each line and create the DB entries
	while (<INFILE>) 
	{
		if (($isModuleIdFound != 1) || ($isFileIdFound != 1) || ($isModuleNameFound !=1))
		{
			if (/(\s*.*)RLOG_MODULE_ID(\s*.*)=(\s*.*);/)
			{
				$moduleId = $3;
				$isModuleIdFound  = 1;
	#print "MOD ID $moduleId\n";
			}
			if (/(\s*.*)RLOG_FILE_ID(\s*.*)=(\s*.*);/)
			{
				$fileId = $3;
				$isFileIdFound = 1;
				#print "FILE $OrgFileName FILE ID $fileId [$1] [$2] [$3]\n";
			}
			if (/(\s*.*)*RLOG_MODULE_NAME(\s*.*)=(\s*.*);/)
			{
				$moduleName = $3;
				$moduleName = substr($moduleName,1);
				$moduleName = substr($moduleName, 0, -1);
				$isModuleNameFound = 1;
	#print "MODULE NAME $moduleName\n";
			}

			print $tmpfile $_;
		}
		elsif (/(\s*.*)(logLev[E0-4HS]\()(\s*.*\,)(\s*\".*\")(\s*.*)/)
		{
			$CStart   = $1;
			$CPattern = $2;
			$CVarible = $3;
			$CString  = $4;
			$CEnd     = $5;


	#printf "1-$1 2-$2 3-$3 4-$4 5-$5-vik\n";
#	printf "3-$3 4-$4 -vik\n";
			if( $3 =~ m/_LOGID/ ) {
				#print "LOG ID PRESENT\n";

				my	$fmtStr = $CString;
#				print "CVARIABLES= $CVarible\n";
				@variables = split(/,/, $CVarible);
				my $arrSize = $#variables;
				my $line = $variables[$arrSize];
				my $file = $variables[$arrSize-1];
#				print "FILE= $file\n";

				if ($fileId < 0x3FF)
				{
					$tokenID = ($fileId << 16)|(++$logNum); 
               if ($prj eq "l3")
               {
                  $tokenID |= (1 << 26);
               }
					$hexTokenID = sprintf("0x%x", $tokenID);
					#print $tokenID ;
				}
				else
				{
					print "SOME ERROR MODID:$moduleId FILEID:$fileId\n";
					return 1;
				}

				$CVarible =~ s/_LOGID/$hexTokenID/;

				print $tmpfile "$CStart$CPattern$CVarible$CString$CEnd\n";
				print $dbfile "$tokenID;$line;$moduleName;$file;$fmtStr\n";
				$generatedb = 1;
			}
		}
		else
		{
			print $tmpfile $_;
		}
# Counting each line in the file for line number
		$lineNum = $lineNum + 1;
	}

##########################################################################
#Close all files and create final output file
	close(INFILE);
	close ($tmpfile);
	close ($dbfile);

#die("Finished\n");

#close ($DBFILE);
	if( $generatedb  eq 1) {
		`mv -f tmp.i $OrgFileName`;
		`mv -f tmp.db $dbFileName`;
		`dos2unix -q $OrgFileName`;
		`dos2unix -q $dbFileName`;
	} else {
		'rm -f tmp.i';
	}
}


sub get_source_files {

   $dir_path = shift;

#   print "DIR PATH $dir_path\n";

   opendir(DIR, $dir_path) || die "ERR: Can't open directory $dir_path...exiting\n";
#  @files =  grep { -f && /\.c$/ || -f && /\.cpp$/ }  readdir (DIR);
   @files =  grep { /\.i$/ }  readdir (DIR);
#  @files =  readdir (DIR);
#   print "Files @files\n";
   closedir(DIR);

   return @files;
}

