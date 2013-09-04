<?php

/*** Display a combo-box allowing to select files of a given extension in a specified directory ***/
function html_combo_box($PATH, $EXT, $ACTION="", $VALUE="" )
{  global $SESSION;
   echo "<form method='post' action='index.php?page={$_GET['page']}$SESSION'>\n<select name='ACTION'>";
   if(empty($VALUE)) echo "option value=''>Select file and press OK</option>";
   if(is_array($PATH))   foreach($PATH as $FILE) echo "<option value='$ACTION$FILE'".("$FILE"=="$VALUE"?" SELECTED":"").">$FILE</option>\n";
   else
   {  $HANDLE=opendir($PATH);
      if(!is_array($EXT)) $EXT=array($EXT);
      while($FILE=readdir($HANDLE)) $TEMP[]=$FILE;
      sort($TEMP);
      foreach($TEMP as $FILE)
      foreach($EXT as $EXTENSION)
         if(stristr($FILE,$EXTENSION)!==FALSE || is_dir("$PATH/$FILE")&&$EXTENSION=="DIR"&&$FILE!="."&&$FILE!=".." )
            echo "<option value='$ACTION$PATH/$FILE'".("$PATH/$FILE"=="$VALUE"?" SELECTED":"").">$FILE</option>\n";
      closedir($HANDLE);
   }
   echo "</select>&nbsp;<input type='submit' value='Go'>\n</form>\n";
}

/*** Add a favorite to Internet Explorer ***/
function html_add_favorite($TITLE, $URL)
{  echo "<SCRIPT LANGUAGE=javascript>var __w=window.external.AddFavorite('$URL','$TITLE');if(__w!=null)document.write(__w);</SCRIPT>\n";
}

/*** Close the page ***/
function html_close()
{  echo "<script language=javascript>setTimeout('self.close();',1000)</script>\n";
}

/*** Generic function to show sub-menus ***/
function html_page()
{  global $MENU, $P_RANK, $P_NAME, $SECTION, $SESSION;
   article($P_NAME);
   echo "Here is the different sections of this category :<BR>\n<UL>\n";
   $count=0; $RANK_ARRAY=explode("-",$P_RANK); reset($MENU);
   foreach($MENU[basename($SECTION)] as $ID=>$FILE)
   {  $TEMP=explode(".",$FILE);
      $RANK2_ARRAY=explode("-", $TEMP[0]); $len=count($RANK_ARRAY); $len2=count($RANK2_ARRAY);
      $result=1; for($i=0;$i<min($len,$len2);$i++) $result*=!strcmp($RANK2_ARRAY[$i],$RANK_ARRAY[$i]);
      if( $result && $len+1==$len2 ) { $count++; echo "<LI><A HREF='index.php?page=$FILE$SESSION'>{$TEMP[2]}</A>"; }
   }
   if (!$count) echo "<LI>No sub-categories were found</LI>";
   echo "</UL>\n";
}

/*** Generate a statical html page from a php page ***/
function html_static_generation($page_php, $page_html)
{  global $data; ob_start(); include($page_php); $fp=fopen($page_html,"w"); fwrite($fp, ob_get_contents()); fclose($fp); ob_end_clean();
}

function php_modules_display ($DIRECTORY)
{  $HANDLE = opendir($DIRECTORY);
   while( $FILENAME = readdir( $HANDLE ) )
   {  if( is_file("$DIRECTORY/$FILENAME") )
      {  article("PHP library <A HREF='$DIRECTORY/$FILENAME'>$FILENAME</A>");
         $FILE=file("$DIRECTORY/$FILENAME");
         echo "<UL>";
         while( list(,$LINE) = each( $FILE ) )
         {  if( !strncmp($LINE, "function ", 8) )   echo "<LI>$LINE<BR>\n";
            if( !strncmp($LINE, "/***"     , 2) )   echo "<BR>$LINE<BR>\n";
         }
         echo "</UL>";
   }  }
}

?>
