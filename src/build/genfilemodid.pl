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
# Author Vikram Korde, 
# Date Aug 2013

#!/usr/local/bin/perl
#use strict;
#use IO::File;

my $FILEID = "RLOG_FILE_ID";
my $MODULEID = "RLOG_MODULE_ID";
my $MODNAME = "RLOG_MODULE_NAME";

my %g_modmap = ();
my %g_modidmap = ();
my %g_filemap = ();
my $g_modid = 1;
my $ccfiles = "";
my @prod_exts = ( "fc_", "qf_", "ni_", "sq_" );

my $g_fileId = 0;
my @g_fidarr  = ( (0) x 256 );
my $g_fidx = 0;
my @g_flist;

my $prj = shift(@ARGV);

printf "\n\nPlease wait.. logging framework is checking source files for file & module id consistency..\n";

foreach my $arg (@ARGV) {
#	print "DIR ARG $arg\n";
	@files =  get_source_files($arg);
	parse_source_files($arg, @files);
}

assign_new_file_ids();
assign_new_module_ids();
write_to_logdb();

sub assign_new_module_ids() 
{
	foreach $k (keys %g_modidmap) {
		my $modname = $g_modidmap{$k};
		my $file = $k;
#		print "Validating file $k Module: $modname\n";
		my $modid = get_module_id($modname);
		system("sed -i \'\/RLOG_MODULE_NAME\/ a static int RLOG_MODULE_ID=$modid;' $file");
		printf("File: $k : MODULE: $modname : Inserted module id $modid\n");
	}
}

sub get_module_id
{
	my $modname = shift;
	my $modid=1, $cnt=0;

	if( exists($g_modmap{$modname}) ) {
#		print "Someone has tampered file MODNAME $modname\n";
		$modid = $g_modmap{$modname};
		return $modid;
	}

	for($cnt = 0; $cnt < 32; $cnt++) 
	{
		my $modidused=0;
		foreach $k (keys %g_modmap) 
		{
			if( $g_modmap{$k} == $modid ) {
				$modidused = 1;
			}
		}

		if( $modidused == 0 ) {
			$g_modmap{$modname} = $modid;
			return $modid;
		}
		$modid = $modid << 1;
	}

	die "Module Ids are exhausted..more than 32 modules !!";
}

sub assign_new_file_ids {

	foreach $file  (@g_flist) {
		my	$fileid = get_file_id();
#		print "Defining new file Id $fileid for $file\n";
		system("sed -i \'\/RLOG_MODULE_NAME\/ a static int RLOG_FILE_ID=$fileid;' $file");
		$g_filemap{$fileid} = $file;
	}
	
}

sub get_file_id {
	foreach $idx ( 1 .. 256 ) {
		 if ( $g_fidarr[$idx] ==  0) {
			$g_fidarr[ $idx ] = $idx;
			return $idx;
		}
	}
	die("System is havingmore than 256 files? please cross check\n");
}

sub write_to_logdb {
	store_file_info();
	store_module_info();

	my $nofiles = length($ccfiles);

	if( $nofiles > 1 ) {
		print "########## Please merge following files ##########\n";
		print "$ccfiles \n\n";
		print "##################################################\n";
	} 

}

sub store_file_info {
	my $filedb = $prj . "file.logdb";
	open(my $fh, ">$filedb") or die "Could not open file '$filedb' $!";
	#foreach $k (@file_map) {
	foreach $k (keys %g_filemap) {
		printf $fh "$k = $g_filemap{$k}\n";
	}
  	print "Logging: created $filedb..\n";
	close $fh;
}

sub store_module_info {
	my $moddb = $prj . "module.logdb";
	open(my $fh, ">$moddb") or die "Could not open file '$moddb' $!";
	foreach $k (keys %g_modmap) {
		printf $fh "$k=$g_modmap{$k}\n";
	}
  	print "Logging: created $moddb..\n";
	close $fh;
}

sub genlog_filemod_ids {
	my (@LDIR) = @_;
	foreach $dir_path (@LDIR) {
	#	print "Parsing dir $dir_path \n";
		@files =  get_source_files($dir_path);
		parse_source_files($dir_path, @files);
	}
}

sub parse_source_files {
	$dir = shift;
	$files = shift;

	foreach $file (@files) {
		@lines = read_file($dir . '/' . $file);
#		printf "\n************ $file ************\n";

		my $fileid = 0;
		my $modid= 0;
		my $modname="NULL";
		my $rlog_macors_used = 0;

		for( @lines )
		{
			if ($_ =~ /$MODULEID/) {
				$modid = get_value($_, $MODULEID);
			}
			if ($_ =~ /$FILEID/) {
				$fileid = get_value($_, $FILEID);
			}
			if ($_ =~ /$MODNAME/) {
				$modname = get_value($_, $MODNAME);
            $rlog_macors_used = 1;
			}

			if ($_ =~ /RLOG\d/) {
				$rlog_macors_used = 1;
			}
		}

      $path = $dir . '/' . $file;

#		print "checking $path\n";
		if ( $modname =~ /NULL/ && $rlog_macors_used == 1) {
			print "\n\Module Name is not assigned to $path, insert following line at begining of file.\n";
			die("Example: static const char* RLOG_MODULE_NAME=\"UMM\";");
		}

		if( $rlog_macors_used == 1 ) 
		{
			my $invalid_fileid = scan_file_id($fileid,$path);

			my $new_modid = update_module_id($modid, $modname, $path);

#       print "$path MODID = $modid2 NEW MODID= $new_modid\n";
			if( $invalid_fileid || $new_modid == 0  ) {
				$ccfiles = $ccfiles . "\n" . $file;
			}
		} else {
		#	printf "RLOG Macros are not used in $path\n";
		}
	}
}

sub update_module_id {
	my $modid = shift;
	my $modname = shift;
	my $file = shift;

	my $emodid = 0;

	if( exists($g_modmap{$modname}) ) {
		$emodid = $g_modmap{$modname};
	}

	if( $modid > 0 &&  $modid == $emodid ) { 
		return $modid;
	}

	if( $modid > 0 && $emodid > 0 && $modid != $emodid ) { 
			system("sed -i \'\/RLOG_MODULE_ID=$modid\/d\' $file");
			print "$file: [$modname] correcting  module id $modid to $emodid\n";
			$g_modidmap{$file} = $modname;
			return 0;
	}

	if( $modid > 0 && $emodid == 0 ) { 
		$g_modmap{$modname} = $modid;
		return $modid;
	}

	$g_modidmap{$file} = $modname;

#	printf("$file : Module $modname entry does not exist ... will be added later\n");

	return 0;
}

sub scan_file_id {
	$fileid = shift;
	$file = shift;

	if( $fileid > 0 ) {
		if( exists($g_filemap{$fileid}) ) {
			print "File Id $fileid is already assigned to $g_filemap{$fileid}..deleting from $file\n";
			$g_flist[$g_fidx++] = $file;
			system("sed -i \'\/RLOG_FILE_ID=$fileid\/d\' $file");
			return 1;
		}
		$g_fidarr[$fileid] = $fileid;
		$g_filemap{$fileid} = $file;
	} else {
		$g_flist[$g_fidx++] = $file;
#		print "Not Defining new file Id for $file\n";
	}

	return 0;
}

sub get_value {
	$line = shift;
	$key = shift;

	my $val  = substr($line, index($line, '=')+1);

	chomp($val);
	chop($val);

	$val =~ s/\;//g;

	return $val;
}


# Routtine to read file contents
sub read_file {
	$file_name = shift;
	open(CFILE, "< $file_name") || die ("Couldnt open file $file_name\n");
	my @lines = <CFILE>;
	close(CFILE);
	return @lines;
}

sub get_source_files { 

	$dir_path = shift;

#	print "DIR PATH $dir_path\n";
	
	opendir(DIR, $dir_path) || die "ERR: Can't open directory $dir_path...exiting\n";
#	@files =  grep { -f && /\.c$/ || -f && /\.cpp$/ }  readdir (DIR);
	@files =  sort grep { /\.c$/ || /\.cpp$/ }  readdir (DIR);

	foreach $prod_ext (@prod_exts) { 
		@files = grep (!/$prod_ext/, @files);
	}
#	@files =  readdir (DIR);
#	print "Files @files\n";
	closedir(DIR);

	return @files;
}

1;

