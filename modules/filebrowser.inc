<?php
function filebrowser($DIRECTORY_ROOT)
{  global $SKIN_PATH, $TEMP;
   if(empty($_GET['DIRECTORY']) || strncmp(realpath($DIRECTORY_ROOT),realpath($_GET['DIRECTORY']),strlen(realpath($DIRECTORY_ROOT))) )   $_GET['DIRECTORY']=$DIRECTORY_ROOT;
   article ("File Browser: {$_GET['DIRECTORY']}");

   require_once("modules/flat.inc"); $ARRAY=csv_read_array("$SKIN_PATH/{$_GET['SKIN']}/_browser.csv", ",");
   foreach($ARRAY as $LINE)
   {  $TEMP=explode(" ", trim($LINE[1]));
      foreach($TEMP as $EXT) $ICONS[$EXT]=$LINE[0];
   }
      
   $FILELIST=$DIRLIST=array();
   $HANDLE  =opendir($_GET['DIRECTORY']);
   while( $FILENAME = readdir($HANDLE) )
   {  if(    is_dir ("{$_GET['DIRECTORY']}/$FILENAME"))   $DIRLIST[]=$FILENAME;
      else
      {  list($EXT)=array_reverse(explode(".",$FILENAME));
         $FILELIST[ $ICONS[strtolower($EXT)] ][]=$FILENAME;
   }  }
   closedir($HANDLE) ;
   ksort($DIRLIST);ksort($FILELIST);

   echo "\n\n<table>\n";
   foreach($DIRLIST as $FILE)
   {  if($FILE==".." && $_GET['DIRECTORY']!=$DIRECTORY_ROOT)   echo "<tr> <td size='0%'><img SRC='{$ICONS['..']}'    ></td><td><a href='index.php?page={$_GET['page']}&DIRECTORY=".dirname($_GET['DIRECTORY'])."'>.. (parent directory)</a></td></tr>\n";
      elseif($FILE!=".." && $FILE!= ".")                       echo "<tr> <td size='0%'><img SRC='{$ICONS['FOLDER']}'></td><td><a href='index.php?page={$_GET['page']}&DIRECTORY={$_GET['DIRECTORY']}/$FILE'>$FILE</a></td></tr>\n";
   }
   foreach($FILELIST as $ICON=>$FILE_ARRAY)
     foreach($FILE_ARRAY as $FILE) echo "<tr> <td><img src='{$ICON}'></td><td><a href='{$_GET['DIRECTORY']}/$FILE'>$FILE</a></td><td>".round(filesize("{$_GET['DIRECTORY']}/$FILE")/1024,2)."KB</td></tr>\n";
   echo "</table>\n\n";
}
?>
