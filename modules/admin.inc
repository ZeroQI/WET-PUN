<?php

/*** Administer logins, passwords and rights ***/
function admin_accounts($FILE)
{  function display_rights($RIGHTS_NAME, $ELEMENT, $TYPE=0 )
   {  global $ARRAY, $RIGHT;
      $STRING="";
      foreach($ARRAY as $LINE_KEY => $LINE)   
	  {  $RIGHT=trim($RIGHT);
         if(empty($RIGHT)) continue;
         if($TYPE==0) if( strpos($ELEMENT,$RIGHT)===false ) $STRING.="<StrIKE>$RIGHT</StrIKE>"; else $STRING.=$RIGHT;
         else $STRING.="<INPUT TYPE=checkbox NAME=NEW_RIGHTS[$LINE] VALUE=$RIGHT".(strpos($ELEMENT,$RIGHT)===false?"":" CHECKED").">$RIGHT";
         $STRING.="&nbsp;&nbsp;";
      }
      return $STRING;
   }
   if(empty($_GET['ACTION'])) $_GET['ACTION'  ]=""; if(empty($_POST['DATA' ][0])) $_POST['DATA'][0]=""; if(empty($_POST['NEW_RIGHTS'])) $_POST['NEW_RIGHTS']=array();
   require_once "modules/flat.inc"; $ARRAY=csv_read_array($FILE,";",0);

   global $MENU, $SESSION; $RIGHTS_NAME=array(); foreach($MENU as $FILE_LISTING) foreach($FILE_LISTING as $TEMP) { $TEMP=array_reverse(explode(".","..$TEMP")); $RIGHTS_NAME[$TEMP[2]]=$TEMP[2]; }

   if( $_GET['ACTION']=="add" || $_GET['ACTION']=="remove" || $_GET['ACTION']=="update" )
   {  $LINE_INSERT=-1; $LINE_KEY=0;
      foreach($ARRAY as $LINE)
      {  if( $LINE[0]==$_GET['DATA'][0] && empty($_POST['DATA'][0]) || $LINE[0]==$_POST['DATA'][0] )   $LINE_INSERT = $LINE_KEY;
         $LINE_KEY++;
      }
      if( $_GET['ACTION']=="remove" && $LINE_INSERT>= 0 ) array_splice ($ARRAY, $LINE_INSERT,1);
      if( $_GET['ACTION']=="add"    && $LINE_INSERT==-1 ) array_splice($ARRAY,count($ARRAY),0, array(array( $_POST['DATA'][0], md5($_POST['DATA'][1]), implode(",",$_POST['NEW_RIGHTS']) )) );
      if( $_GET['ACTION']=="update" && ($_POST['DATA'][0]==$_GET['DATA'][0] || $LINE_INSERT==-1) )
      {  $ARRAY[$_GET['DATA'][0]][0]=$_POST['DATA'][0];
         if( $_POST['DATA'][1]!="********" )    $ARRAY[$_GET['DATA'][0]][1]=md5($_POST['DATA'][1]);
         if( !is_array($_POST['NEW_RIGHTS']) )  $ARRAY[$_GET['DATA'][0]][2]=""; else $ARRAY[$_GET['DATA'][0]][2]=implode(",", $_POST['NEW_RIGHTS']); 
      }
      csv_write_array( $FILE, $ARRAY, ';'); 
      reset($ARRAY);
   }
   echo "<table>\n<tr class='title'> <th width=30%>User name</th> <th width=30%>Password</th> <th width=20%>Rights</th> <th width=20%>Action</th></tr>\n";
   if($_GET['ACTION']!="modify") $ARRAY[]="ADD";
   foreach($ARRAY as $LINE_KEY => $LINE)   
   {  echo "<tr CLASS='content'>";
      if( $LINE!="ADD" && !($_GET['ACTION']=="modify" && $LINE[0]==$_GET['DATA'][0]) ) /*** display row ***/
      {  foreach($LINE as $COL_KEY => $ELEMENT)   //PHP7, deprecated: while( list($COL_KEY,$ELEMENT)=each($LINE) )
         {  switch($COL_KEY)
            {  case 1: echo "<td>MD5 coded password</td> "; break;
               case 2: echo "<td>".display_rights($RIGHTS_NAME, $ELEMENT, 0)."</td> "; break;
               default: echo "<td>$ELEMENT</td> ";
         }  }
         echo "<td ALIGN=CENTER><A HREF='index.php?page=$_GET[page]$SESSION&ACTION=modify&DATA[0]={$LINE[0]}'>Modify</A>&nbsp;<A HREF='index.php?page=$_GET[page]$SESSION&ACTION=remove&DATA[0]={$LINE[0]}' onclick='if( !confirm('Delete account {$ARRAY[$LINE_KEY][0]}') ) {location.href='#';return false;}'>Delete</A></td></tr>\n";
      } 
      else /*** add & modify form ***/
      {  if($LINE=="ADD") $action=array("add", "", "", ""); else $action=array("update","********", $_GET['DATA'][0], $ARRAY[($_GET['DATA'][0])][2]);
         echo "<form method=POST action='index.php?page=$_GET[page]$SESSION&ACTION={$action[0]}&DATA[0]={$action[2]}'>".
              "<td><input type=text name='DATA[0]' value='{$action[2]}' size=*></td> <td><input type='password' name='DATA[1]' size=10 value='{$action[1]}'></td> <td>".display_rights( $RIGHTS_NAME, $action[3], 1 )."</td>".
              "<td ALIGN=CENTER><input type=submit value='{$action[0]}' size=*>".($_GET['ACTION']=="modify" ? " <A HREF='index.php?page=$_GET[page]$SESSION'>Cancel</A>" : "")."</td></FORM></tr>";
   }  }
   echo "</table><br />\n";
}

/*** Administer page name, rank into menu, right needed to view the page ***/
function admin_pages($DIRECTORY)
{  global $SESSION, $MENU;  
   if(empty($_GET['ACTION'])) $_GET['ACTION']=""; if(empty($_GET['ITEM'])) $_GET['ITEM']="";
      
   if($_GET['ACTION']=="add" || $_GET['ACTION']=="remove" || $_GET['ACTION']=="update" ) /*** array modification ***/
   {  if($_GET['ACTION']=="remove")  unlink( "$DIRECTORY/{$_GET['ITEM']}" );
      if($_GET['ACTION']=="add" && $_POST['DATA'][2]!="ADD")
      {  $_GET['ITEM']=implode(".",$_POST['DATA']);
         require_once "modules/flat.inc"; csv_write_array( "sections/".$_GET['ITEM'], array($_POST['TEXT']), "");
      }
      if($_GET['ACTION']=="update")
      {  $_GET['ACTION']="modify";
         $FILE=fopen("$DIRECTORY/".implode(".",$_POST['DATA']), "w"); fputs($FILE, $_POST['TEXT']); fclose($FILE); 
         if( "$DIRECTORY/{$_GET['ITEM']}"!="$DIRECTORY/".implode(".",$_POST['DATA']))
         {  unlink( "$DIRECTORY/{$_GET['ITEM']}" );
            $TEMP=array_reverse(explode(".","..{$_GET['ITEM']}")); $MENU[$_GET['FILE']][ $TEMP[3] ]= implode(".",$_POST['DATA']);
            $_GET['ITEM']=implode(".",$_POST['DATA']);
   }  }  }
   $RIGHTS_NAME=array(); foreach($MENU as $FILE_LISTING) foreach($FILE_LISTING as $TEMP) { $TEMP=array_reverse(explode(".","..$TEMP")); $RIGHTS_NAME[$TEMP[2]]=$TEMP[2]; }
   echo "<table width='100%>\n<tr class='title'> <th>Position</th> <th>Rights</th> <th>Section name</th> <th>Extension</th> <th>Action</th> </tr>\n";
   foreach($MENU[$_GET['FILE']] as $FILE)
   {  ECHO "<tr CLASS='content'>";
      $data=array_reverse(explode(".","..$FILE"));
      if( $FILE!="99..ADD.php" && !($_GET['ACTION']=="modify" && $FILE==$_GET['ITEM']) ) /*** Display row ***/
         echo " <td>{$data[3]}</td> <td>{$data[2]}</td> <td>{$data[1]}</td> <td>{$data[0]}</td>".
              " <td ALIGN=CENTER> <A HREF='index.php?page={$_GET['page']}$SESSION&ACTION=modify&ITEM={$FILE}'>Modify</A>&nbsp;".
              "                   <A HREF='index.php?page={$_GET['page']}$SESSION&ACTION=remove&ITEM={$FILE}' onclick='if( !confirm('Delete page $FILE') ) {location.href='#';return false;}'>Delete</A> </td>";
      else /*** add & modify form ***/
      {  $TEMP="\n\n<SELECT name=DATA[1]>";
         foreach($RIGHTS_NAME as $ELEMENT) $TEMP.="<option value='$ELEMENT'".($ELEMENT==$data[2]?"selected":"").">$ELEMENT</option>";
         $TEMP.="</SELECT>\n";
         echo "<form method='POST' action='index.php?page={$_GET['page']}&ACTION=".($FILE=="99..ADD.php"?"add":"update")."&ITEM={$FILE}$SESSION'>".
         "<td><input type='text'   name='DATA[0]' value='{$data[3]}' size='05'></td> <td>$TEMP</td>".
         "<td><input type='text'   name='DATA[2]' value='{$data[1]}' size='20'></td> <td><input type='text'   name='DATA[3]' value='{$data[0]}' size='01'></td>".
         "<td ALIGN='CENTER'><input type='submit' value='".($FILE=="99..ADD.php"?"add":"update")."'>".($_GET['ACTION']=="modify" ? " <A HREF='index.php?page=$_GET[page]$SESSION'>Cancel</A>" : "")."</td></tr>";
   }  }
   //if ($_GET['ACTION']=="modify" && file_exists("$DIRECTORY/{$_GET['ITEM']}")) echo "<tr CLASS=content><td COLSPAN=5><TEXTAREA wrap=off name=TEXT cols=80% rows=20%>".rtrim(str_replace("\r\r\n", "\n", implode("",file("$DIRECTORY/{$_GET['ITEM']}"))))."</TEXTAREA></FORM></td></tr>\n";
   echo "</table><br />\n";
}


?>
