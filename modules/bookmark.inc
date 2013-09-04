<?PHP 
function add_to_bookmark($LINK, $NAME) { echo "<a href=\"javascript:window.external.addfavorite('$LINK','$NAME')\">Ajouter aux favoris</a>"; }

/*** display a windows bookmark ***/
function bookmark($PATH, $TITLE="", $ROOT="")
{  global $SESSION, $SKIN_PATH;
   require_once "modules/flat.inc"; $table=ini_file("$SKIN_PATH/{$_GET['SKIN']}/tags.ini");
   
   if(empty($TITLE))
   {  if(empty($_POST['COL'])) $_POST['COL']=4;
      echo "<form method='POST' action='index.php?page=$_GET[page]$SESSION'><select name='COL'>";
      for($x=1;$x<6;$x++) echo "<option value='$x'".($_POST['COL']==$x?" selected":"").">Column number: $x</option>";
      echo "</select>\n<input type='submit' value='Go'></form>\n";
   }
   else echo "<div class='title'>$TITLE</div>\n";
   $nb=0;
   if(empty($ROOT)) $ROOT=$PATH;
   $FOLDER=array();
   $HANDLE=opendir($PATH);
   while($FILE=readdir($HANDLE))
   {  if(is_dir("$PATH/$FILE")) { if($FILE!="." && $FILE!="..") $FOLDER[]=$FILE; }
      elseif(!stristr($FILE,".ico") && $TITLE!="")
      {  if(!$nb)                     echo "<table align='center' width='100%'>\n<tr class='content'>";
         elseif($nb%$_POST['COL']==0 && $TITLE!="") echo" </tr>\n<tr class='content'>";
             else                     echo"    ";
         $ARRAY=ini_file("$PATH/$FILE");
         if( empty($ARRAY['IconFile']) )   $IMG="$ROOT/default.ico";
         else {  $IMG="$PATH/$FILE.ICO";
                 if(!file_exists("$PATH/$FILE.ICO")) if(!copy($ARRAY['IconFile'],$IMG)) copy("$PATH/$FILE.ICO", $IMG);
              }
         echo "<td width='".ceil(100/$_POST['COL'])."%'><a href='".trim($ARRAY['URL'])."' target='_blank'><img src='$IMG' width='16' heigh='16'>".substr($FILE,0,strlen($FILE)-4)."</a></td>\n";
         $nb++;
   }  }
   closedir($HANDLE);
   if(!empty($TITLE))
   {  for(;$nb%$_POST['COL']>0;$nb++) echo "    <td width='".ceil(100/$_POST['COL'])."%'>&nbsp;</td>\n";
      echo "</tr>\n</table><br /><br />\n\n";
   }
   if(!empty($FOLDER)) foreach($FOLDER as $FILE) bookmark("$PATH/$FILE", basename($FILE), $PATH);
}

?>
