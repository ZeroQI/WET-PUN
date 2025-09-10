<?php
$LOGS_PATH="modules/access";    //Logs & Login base
$SKIN_PATH="modules/templates"; //Layout pages(layout.tpl), box(article.tpl), tags(tags.tpl), CSS(template.css)	('&SKIN=_Folder_name_' to load)
$PHP_PATH ="pages";             //User - Php pages shown in the left menu + news, quotes
//Global variables: $_GET['page', 'SKIN'], $P_RANK, $P_AUTH, $SESSION, $MENU, $SECTION
//Temp   variables: $ARRAY, $TEMP

/*** Time generation ***/
$starttime =  explode(' ', microtime());

/*** Menu Array make, URL checking, redirection ***/
$SESSION="";
$P_DIR=opendir($PHP_PATH);
while($DIR=readdir($P_DIR))
{  if(is_dir("$PHP_PATH/$DIR") && $DIR!="." && $DIR!="..")
   {  $P_FILE=opendir("$PHP_PATH/$DIR");
      while($FILE=readdir($P_FILE))
       if(is_file("$PHP_PATH/$DIR/$FILE"))
       {  $MENU[$DIR][strtok($FILE,".")]=$FILE;
          if(!empty($_GET['page']) && $_GET['page']==$FILE) $SECTION="$PHP_PATH/$DIR";
       }
      closedir($P_FILE);
      ksort($MENU[$DIR], SORT_REGULAR);
      if(empty($TEMP)) $TEMP=$MENU[$DIR]['01'];
}  }
closedir($P_DIR);
if(!empty($_GET['SKIN']) && file_exists("$SKIN_PATH/{$_GET['SKIN']}/article.tpl")) $SESSION.="&amp;SKIN={$_GET['SKIN']}"; else $_GET['SKIN']="_default";
if(empty($SECTION))
{  if(empty($_GET['page'])) { header("Location: index.php?page=$TEMP$SESSION"); exit; }
   elseif($_GET['page']=="99..Logout.php") { session_start(); session_destroy(); header("Location: {$_SERVER['HTTP_REFERER']}"); exit; }
}
list($P_EXT,$P_NAME,$P_AUTH,$P_RANK)=array_reverse(explode(".","..{$_GET['page']}"));

/*** Conditional get ***/
//header("Content-type: text/html; charset=iso-8859-1"); 
//$last_modified=gmdate('D, d M Y H:i:s \G\M\T', filemtime($SECTION."/".$_GET['page']) );
//header("Last-Modified: $last_modified");
//header('Cache-Control:public max-age=86400 must-revalidate');
//header('Expires: '.gmdate('D, d M Y H:i:s \G\M\T',time()+24*60*60));
//if( !empty($_SERVER['HTTP_IF_MODIFIED_SINCE']) && $_SERVER['HTTP_IF_MODIFIED_SINCE']==$last_modified ) { header("HTTP/1.0 304 Not Modified"); exit; }

/*** Login and rights ***/
session_start(); if($P_AUTH) $MSG="'$P_AUTH' right required";
if(!empty($_POST['LOGIN']))
{  require_once("modules/flat.inc"); $ARRAY=csv_read_array("$LOGS_PATH/access.csv",";");
   if(empty($ARRAY[$_POST['LOGIN']][0])) $MSG="Bad LOGIN"; elseif($ARRAY[$_POST['LOGIN']][1]!=md5($_POST['PASSWD'])) $MSG="Bad PASSWORD"; else
   { if(empty($_POST['COOKIE'])) { setcookie("LOGIN",""); setcookie("PASSWD",""); }
     else { setcookie("LOGIN",$_POST['LOGIN'],time()+3600*24*7); setcookie("PASSWD",$_POST['PASSWD'],time()+3600*24*7); }
     $_SESSION['LOGIN' ]=$_POST['LOGIN']; $_SESSION['PASSWD']=$_POST['PASSWD']; $_SESSION['RIGHTS']=$ARRAY[$_POST['LOGIN']][2];
   }
   csv_log_add("$LOGS_PATH/access.log", date("Y/m/d H:i:s").",".getenv("REMOTE_ADDR").", {$_POST['LOGIN']}, $MSG, {$_GET['page']}");
}
if(empty($_SESSION['RIGHTS'])) $_SESSION['RIGHTS']=""; else { $SESSION.="&amp;ID=".session_id(); $MENU['Administration'][99]="99..Logout.php"; } /*** ID propagation bug ***/

/*** Menu ***/
ob_start();
foreach($MENU as $DIR=>$FILE_LISTING)
{  if(!empty($FILE)) echo "<br />\n\n";
   $item=explode(".",".$DIR");
   echo "<div class='menubox'><div class='title'>".array_pop($item)."</div>\n<ul>";
   foreach($FILE_LISTING as $ID=>$FILE)
   {  $ARRAY=array_reverse(explode(".","..$FILE"));
      if($len=substr_count($ARRAY[3],"-")) $TEMP=str_repeat("&nbsp;&nbsp;&nbsp;", $len); else $TEMP="";
      if(!$len || !strncmp($ARRAY[3],$P_RANK,3*$len-1) && "$PHP_PATH/$DIR"==$SECTION) echo "<li><a class='menu".($_GET['page']==$FILE?"2":"")."' href='index.php?page=$FILE$SESSION'>$TEMP{$ARRAY[1]}</a></li>\n";
   }
   echo "</ul></div>\n";
}
$MENU_STRING=ob_get_contents(); unset($FILE_LISTING, $FILE, $DIR); reset($MENU);
ob_end_clean();

/*** Article ***/
function article($TITLE="", $TITLE_CLASS="title", $TEXT_CLASS="content", $WIDTH="100%", $TD="")
{  global $SKIN_PATH; static $NB,$TEMPLATE;
   if(empty($TEMPLATE)) $TEMPLATE=explode("%TEXT%", implode("", file("$SKIN_PATH/{$_GET['SKIN']}/article.tpl")) );
   $TEMP = str_replace( array("%TITLE_CLASS%","%TEXT_CLASS%","%TITLE%","%WIDTH%","%TD%", "%PATH%" ), array( $TITLE_CLASS, $TEXT_CLASS, $TITLE, $WIDTH, $TD, $SKIN_PATH), $TEMPLATE );
   if(empty($NB)) $TEMP[1]=""; else $NB=0; if(empty($TITLE)) $TEMP[0]=""; else $NB=1; echo $TEMP[1].$TEMP[0];
}

/*** Page ***/
ob_start();
if(!empty($P_AUTH)&& strpos($_SESSION['RIGHTS'],$P_AUTH)===false )
{  article("Authentification");
   if(empty($_COOKIE['LOGIN'])) $_COOKIE['LOGIN' ]=$_COOKIE['PASSWD']=$CHECKED=""; else $CHECKED=' checked';
   echo"
    <table width=0% border=1 style='background:#4E6FD6;'>
    <form action='index.php?page={$_GET['page']}' method='post'>
    <tr> <td rowspan=5> <img src='$SKIN_PATH/wpakey.jpg' /> </td> 
         <td>Username</td> <td colspan='1'> <input type='text'     value='{$_COOKIE['LOGIN']}'  name='LOGIN' >          </td> </tr>
    <tr> <td>password</td> <td colspan='1'> <input type='password' value='{$_COOKIE['PASSWD']}' name='PASSWD'>          </td> </tr>
    <tr> <td>Remember</td> <td colspan='1'> <input type='checkbox' value='y' $CHECKED           name='COOKIE'> Cookie   </td> </tr>
    <tr> <td colspan='2' align='center'>    <strike>$MSG</strike><br />                                                                 </td> </tr>
    <tr> <td colspan='2' align='center'>    <input type='submit' value='Login'>                                         </td> </tr>
    </form>
    </table>";
}
else include($SECTION."/".$_GET['page']);
article();

//$endtime = explode(' ', microtime());
//$processtime= $endtime[0]+$endtime[1]-$starttime[0]-$starttime[1];
//echo "generated on ".gmdate('D, d M Y H:i:s \G\M\T',time())." in ";
//printf("%1.3f sec.",$processtime);

$PAGE_STRING=ob_get_contents(); ob_end_clean(); ob_start("ob_gzhandler"); //ob_start("zlib.output_handler");
echo str_replace( array("%MENU%", "%CONTENT%", "%SKIN_PATH%"), array($MENU_STRING, $PAGE_STRING, "$SKIN_PATH/{$_GET['SKIN']}" ), implode("", 	file("$SKIN_PATH/{$_GET['SKIN']}/layout.tpl")) );
?>
