<?php

/*** Load CSV file into array, key is column number or -1 for numeric indexes ***/
function csv_read_array( $FILENAME, $DELIMITER=',', $COLUMN=0, $TYPE="ASC", $E='"' )
{  $ARRAY=$COUNT=array();
   if(file_exists("$FILENAME") && $FILE=fopen($FILENAME,"r"))
   {  while($LINE=fgetcsv($FILE,2048,$DELIMITER, $E))
         if($COLUMN=="-1") $ARRAY[]=$LINE;
         else if(!array_key_exists($LINE[$COLUMN], $ARRAY)) { $ARRAY[$LINE[$COLUMN]]=$LINE;                                    $COUNT[$LINE[$COLUMN]]=1; }
              else                                          { $ARRAY[$LINE[$COLUMN]."-".(string)$COUNT[$LINE[$COLUMN]]]=$LINE; $COUNT[$LINE[$COLUMN]]++; }
      fclose($FILE);
   }
   return $ARRAY ;
}

/*** Write a file, given the an array (multi or uni dimensionnal) and delimiter, fopen('php://output'  ***///fputcsv needs array only and closing character
function csv_write_array( $FILENAME, $ARRAY, $DELIMITER=';', $E='' )
{  if($FILE=fopen($FILENAME, "w"))
   {  foreach($ARRAY as $LINE)   fputs($FILE, $E.(is_array($LINE)?implode($E.$DELIMITER.$E,$LINE):$LINE)."$E\r\n" ); 
      fclose($FILE);
}  }

/*** Sort a multi-dimentional array by column (no duplicate per column sorted) ***/
function csv_sort_array(& $ARRAY, $COLUMN, $TYPE="ASC")
{ function ASC($a,$b) { return( strcmp( $a[$GLOBALS['COLUMN']], $b[$GLOBALS['COLUMN']] )); }
  function DES($a,$b) { return( strcmp( $b[$GLOBALS['COLUMN']], $a[$GLOBALS['COLUMN']] )); }
  $GLOBALS['COLUMN']=$COLUMN; usort($ARRAY,$TYPE);
}

/*** Parse a ini file without errors ***/
function ini_file($filename)
{  $ini_array = array();
   $lines     = file($filename);
   foreach($lines as $line)
   {  if( !($line[0] == "[" && $line[strlen($line)-1]=="]") && $line != "" )
      {  $pos=strpos($line, "=");
         $ini_array[trim(substr($line, 0, $pos))] = trim(substr($line, $pos + 1));
   }  }
   return $ini_array;
}

/*** Display a CSV array ***/
function csv_display_array($ARRAY, $TITLE=1)
{  global $SKIN_PATH; $table=ini_file("$SKIN_PATH/{$_GET['SKIN']}/tags.ini");
   echo "{$table['header']}\n";
   foreach($ARRAY as $LINE_NB => $LINE) 
   {  if(!is_array($LINE)) $LINE=array($LINE);
      if($TITLE-1==$LINE_NB) $temp="title-"; else $temp="";
      echo "{$table[$temp.'line-begin']}";
      foreach($LINE as $ELEMENT) echo " {$table[$temp.'element-begin']}".htmlspecialchars($ELEMENT, ENT_QUOTES)."{$table[$temp.'element-end']}";
      echo " {$table[$temp.'line-end']}\n";
   }
   echo "{$table['footer']}\n<br /><br />Lines: ".(count($ARRAY)-$TITLE);
}

/*** Encode text to include them in CSV files ***/
function csv_text_encode($TEXT)
{ //return htmlspecialchars(str_replace("<!-- BR -->", "\0\0", str_replace("\r\n","<br />",$TEXT)));
  return str_replace("\t","\r\n",htmlentities($TEXT));
}

/*** Decode text to include them in CSV files ***/
function csv_text_decode($TEXT)
{ return str_replace("\r\n","<br />",$TEXT);
}

/*** Find the key corresponding to a value in a two-dimentional array ***/
function array_find($array,$find)
{  foreach($array as $rank=>$element) foreach($element as $tag=>$value) if($value==$find) return $rank;
   return FALSE;
}

/*** Add entry to log file, create if needed, use when no info from the log is needed ***/
function csv_log_add($LOGS_PATH, $TEXT)
{  $log=fopen($LOGS_PATH,"a+"); fputs($log,"$TEXT\r\n"); fclose($log);
}

/*** manage a CSV file, parameters are: filename, if the column names are present, index columnn, separator ***/
function admin_csv($FILE,$TITLE=1,$COL=0, $SEPARATOR=";")
{  if(empty($_GET['ACTION'  ])) $_GET['ACTION'  ]="";
   if(empty($_GET['ID'      ])) $_GET['ID'      ]="";
   if(empty($_POST['DATA'][0])) $_POST['DATA'][0]="";
   $ARRAY=csv_read_array($FILE, $SEPARATOR, -1);
   
   if( $_GET['ACTION']=="remove" || $_GET['ACTION']=="add" || $_GET['ACTION']=="update" )    /*** Modify array ***/
   {  $OCCURENCE= 0; $LINE_INSERT=-1;
      foreach($ARRAY as $LINE_KEY => $LINE)
      {  if($LINE[0]==$_GET['ID']) { $LINE_INSERT=$LINE_NB; $OCCURENCE++; }
         if($LINE[0]==$_POST['DATA'][0]) $OCCURENCE++;
      }
      if($_GET['ACTION']=="remove" && $OCCURENCE==2                                     ) array_splice($ARRAY, $LINE_INSERT, 1);
      if($_GET['ACTION']=="update" && ($_POST['DATA'][0]==$_GET['ID'] || $OCCURENCE==1) ) array_splice($ARRAY, $LINE_INSERT, 1, array($_POST['DATA']));
      if($_GET['ACTION']=="add"    && $OCCURENCE==0 && $_POST['DATA'][0]!="ADD"         ) array_splice($ARRAY, count($ARRAY), 0, array($_POST['DATA']));
      csv_write_array($FILE, $ARRAY, $SEPARATOR);
      reset($ARRAY);
   }
   if($_GET['ACTION']!="modify")   $ARRAY[][0]="ADD";

   echo "<TABLE width='100%'>\n";
   foreach($ARRAY as $LINE_NB => $LINE)
   {  $LINE[0] = str_replace("\'", "\\\'", $LINE[0]);
      echo "<tr CLASS='content' valign='top'>";
      if( $LINE[0]!="ADD" && ($_GET['ACTION']!="modify" || $LINE[0]!=$_GET['ID']) ) /*** display row ***/
      {  if($TITLE==1 && $LINE_NB==0) { foreach($LINE as $COLUMN_KEY => $ELEMENT)  echo "<TH CLASS=title>$ELEMENT</TH>"; echo "<TH CLASS=title>Action</TH>"; }
         else 
         {  foreach($LINE as $ELEMENT) echo "<TD>$ELEMENT</td> ";
            echo "<TD ALIGN=CENTER> <A HREF='index.php?page={$_GET['page']}&amp;FILE=$FILE&amp;ACTION=modify&ID={$LINE[0]}'>Modify</A>&nbsp; <A HREF='index.php?page={$_GET['page']}&FILE=$FILE&ACTION=remove&ID={$LINE[0]}' onclick='if( !confirm('delete {$LINE[0]}') ) {location.href='#';return false;}'>Delete</A> </td>";
      }  }
      else /*** add or update form ***/
      {  echo "<form method=POST action='index.php?page={$_GET['page']}&FILE=$FILE&ACTION=".($LINE[0]=="ADD"?"add":"update")."&ID={$LINE[0]}'>";
         foreach($LINE as $FIELD_KEY => $FIELD)  echo "<TD><input type=text name=DATA[$FIELD_KEY] value=$FIELD></td>";
         echo "<TD ALIGN=CENTER VALIGN=TOP><input type=submit value=".($LINE[0]=="ADD"?"add":"update")."> <A HREF='index.php?page={$_GET['page']}&FILE=$FILE'>Cancel</A></td></FORM>";
      }
      echo "</tr>\n";
   }
   echo "</table><br />\n";
}
?>
