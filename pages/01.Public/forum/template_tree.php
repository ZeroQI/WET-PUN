<?php global $data;
      if( empty($data) )
      {  $DYNAMIC="threads/";
         include_once "../../../modules/rss.inc";$_GET['BOARD']="board1";$PERE_LINE=0;
         $data=rss_read_array($_GET['BOARD']."/board.rss");
      }
      else $DYNAMIC="";
?>
<html> <head> <title>Liste des messages postés dans le forum de discussion</title>
              <META HTTP-EQUIV="Pragma" CONTENT="no-cache">
              <META HTTP-EQUIV="Expires" CONTENT="-1">
       </head>

<body bgcolor="#F5F5ED" text="#000000" link="#0000EE" vlink="#800080">

<h3> <center> <font face="verdana">Forum de discussion</font> </center> </h3>
<center> <font face="Verdana" size="-1">&gt;&nbsp;<a href="<?=$DYNAMIC?>../forum_post.php"> <b>Poster un nouveau message</b> </a>&nbsp;&lt;<br> </font> </center> <br>

<table border="0" cellspacing="0" cellpadding="0" width="90%" align="center" bgcolor="#000000"><tr><td>

<table border="0" width="100%" cellspacing="1" align="center" cellpadding="5">
<tr height="35">
    <td bgcolor="#4E4379" COLSPAN="2"> <font face="Verdana" size="2" color="#ffffff"> <b>Sujets         </b> </font> </td>
    <td bgcolor="#4E4379">             <font face="Verdana" size="2" color="#ffffff"> <b>Reponses       </b> </font> </td>
    <td bgcolor="#4E4379">             <font face="Verdana" size="2" color="#ffffff"> <b>Auteur         </b> </font> </td>
    <td bgcolor="#4E4379">             <font face="Verdana" size="2" color="#ffffff"> <b>Dernier message</b> </font> </td>
</tr>

<?php
$PERE_LINE=0; $COLOR = array( 1, "#e8e8e8", "#d8d8d8" );
$LAST_REPLY = array(); $REPLY_NB = array(); $PERE = array();
reset($data); next($data);
while( list($LINE_NB,$FIELD)=each($data) )
{  if( $FIELD[1]==1 )  {  $PERE_LINE=$LINE_NB; $REPLY_NB[$PERE_LINE]=0;   }
   else                {                       $REPLY_NB[$PERE_LINE]++;   }
   $LAST_REPLY[$PERE_LINE]=$FIELD[2];
   $PERE[$LINE_NB]=$PERE_LINE;
}reset($data); next($data);
while( list($LINE_NB,$FIELD)=each($data) )  {
?>

<TR bgColor='<?php echo $COLOR[$COLOR[0]]; $COLOR[0] = $COLOR[0]%(count($COLOR)-1)+1; ?>'>
    <TD width=03%>              <?php $red="";if($FIELD[1]==1) echo "<IMG SRC='$DYNAMIC../../../media/images/folder/folder$red.gif'>"; ?> </TD>
    <TD width= *%>              <B><A href='<?php if( !empty($DYNAMIC) ) echo "template_threads.php?root_id={$PERE[$LINE_NB]}#{$FIELD[0]}"; else echo $DYNAMIC.sprintf("msg_%04d.htm#{$FIELD[0]}", $FIELD[0]); ?>'><?php for($i=0;$i<$FIELD[1];$i++) echo"&nbsp;&nbsp;&nbsp;&nbsp;"; echo $FIELD[5]; ?></A></B> </TD>
    <TD width=05% align=RIGHT>  <?php if($FIELD[1]==1) echo $REPLY_NB[$LINE_NB] ?> </TD>
    <TD width=20% align=CENTER> <FONT face=Arial color=#000080 size=-2> <A href="mailto:<?=$FIELD[4] ?>"><?= $FIELD[3] ?></A> </FONT> </TD>
    <TD width=25%>              <FONT face=Arial color=#000080 size=-2> <?=$FIELD[2]?> </FONT> </TD>
</TR>

<?php  }  ?>

</table>

</td></tr></table>
</body>
</html>
