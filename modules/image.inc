<?php 

/*** Display picture galleries on 1 subdirectory ***/
function image_gallery_index($PATH, $TITLE="")
{  global $SESSION;
   article($TITLE);
   echo "<a href='index.php?page={$_GET['page']}&ACTION=.$SESSION'>Diplay all root galleries</a>\n";
   $HANDLE=opendir($PATH);
   echo "<ul>\n";
   while($FILE=readdir($HANDLE)) if(is_dir("$PATH/$FILE") && $FILE!="." && $FILE!="..") echo"<li><a href='index.php?page={$_GET['page']}&ACTION=$FILE$SESSION'>$FILE</a></li>\n";
   closedir($HANDLE);
   echo "</li>\n</ul>\n";
   if(!empty($_GET['ACTION'])) if($_GET['ACTION']==".") image_gallery($PATH,"",1); else image_gallery($PATH."/".$_GET['ACTION'], $_GET['ACTION'],3);
}

/*** Display picture gallery recursively ***/
function image_gallery($PATH, $TITLE="", $DIRNB=2, $HEIGHT=120)
{  static $DIR; if(empty($DIR)) $DIR=0;
   article($TITLE);
   echo"<center>";
   $HANDLE=opendir($PATH);
   while($FILE=readdir($HANDLE))
   {  if(is_dir("$PATH/$FILE") && $FILE!="." && $FILE!="..") $FOLDER[]=$FILE;
      if(is_file("$PATH/$FILE") && strncmp($FILE,".",1))
      {  list($x,$y,$ext,)=getimagesize("$PATH/$FILE");
         if(!file_exists("$PATH/.$FILE")) image_thumbnail("$PATH/$FILE", "$PATH/.$FILE", $x, $y, $HEIGHT, $ext);
         echo "<a href='$PATH/$FILE' title='$FILE\n{$x}x$y\n".round(filesize("$PATH/$FILE")/1024,1)."KB' target=_blank><img src='$PATH/.$FILE'></a>\n";
   }  }
   closedir($HANDLE);
   echo"</center>";
   if(!empty($FOLDER) && $DIR<$DIRNB) foreach($FOLDER as $FILE) { $DIR++; image_gallery("$PATH/$FILE",$TITLE==""?"$FILE":"$TITLE / $FILE",$DIRNB); $DIR--; }
}

/*** Generate a thumbnail from an image URL [GL library necessary] ***/
function image_thumbnail($URL_IN, $URL_OUT, $x, $y, $heigh, $TYPE)
{  if($type!="db")
   {  $EXT=array(1=>"gif",2=>"jpeg",3=>"png","jpg"=>"jpeg");
      $TYPE=$EXT[$TYPE];
      if(function_exists('imagecopyresampled'))
      {  eval("\$src=imagecreatefrom$TYPE('$URL_IN');");
         $dest=imagecreatetruecolor($x*$heigh/$y, $heigh);
         imagecopyresampled($dest, $src, 0, 0, 0, 0, $x*$heigh/$y, $heigh, $x, $y);
         imagedestroy($src);
         eval("image$TYPE(\$dest,'$URL_OUT');");
         imagedestroy($dest);
      }
      else echo"<strike>GD2 library</strike>";
}  }

/*** Display randomly a picture in a specified directory ***/
function image_random($TITLE, $PATH)
{  $HANDLE=opendir($PATH);
   while($FILE=readdir($HANDLE)) if(is_file("$PATH/$FILE")) $LIST[]=$FILE;
   closedir($HANDLE);

   article($TITLE); echo "<center><img src='$PATH/{$LIST[array_rand($LIST,1)]}'></center>\n"; article();
}

/*** Upload form ***/
function image_upload($PATH)
{  global $SESSION, $SECTION;
   article( "Upload (limited to 64KB)" );
   echo "<form method='post' enctype='multipart/form-data' action='index.php?page={$_GET['page']}$SESSION'>";
   echo "&nbsp;<input type='file' name='FILE'>&nbsp;&nbsp;<input type='submit'></form>\n";
   if(!empty($_FILES['FILE']) && $_FILES['FILE']['size']<65536) move_uploaded_file($_FILES["FILE"]["tmp_name"], "$PATH/{$_FILES['FILE']['name']}");
}
?>
