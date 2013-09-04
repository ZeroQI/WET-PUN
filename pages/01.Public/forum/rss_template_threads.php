<?php global $rss_board, $rss_thread, $SECTION;
if(empty($rss_board))
{  $DYNAMIC=""; include_once "../../../modules/flat.inc";
   $rss_board =rss_read_array("{$_GET['BOARD']}/board.rss");
   $rss_thread=rss_read_array("{$_GET['BOARD']}/{$_GET['THREAD']}.rss");
}
else $DYNAMIC="../";
?>
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
   <title>Affichage d'un message posté</title>
   <style type="text/css">@import url('%SKIN_PATH%/template.css');</style>
   <style type="text/css">
      table { font: 1em  black      Verdana; background:#4E4379; width:90%; }
      th    { font-size:12px; font-weight:bold; font-family:Verdana; color:#FFFFFF; }
      td    { font-size:12px; font-weight:bold; font-family:Verdana; color:#000000; }
      th,td { padding:0.5em; }
   </style>
   <link rel="alternate" type="application/rss+xml" title="ZeroQI news feed" href="<?php echo "$DYNAMIC{$_GET['BOARD']}/{$_GET['THREAD']}.rss" ?>">

   <meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"/>
   <meta name="description"        content="ZeroQI Web Portal">
   <meta name="copyright"          content="Copyright (c) 2004 Authir or Company">
   <meta name="author"             content="Author Name">
   <meta http-equiv="Pragma"  content="no-cache">
   <meta http-equiv="Expires" content="-1">

</head>
<body>

<h3> <center> <font face="verdana"><?php echo $rss_thread[0]['TITLE'] ?></font> </center> </h3>
<center> <font face="Verdana" size="-1">&gt;&nbsp;<a href="<?PHP echo $DYNAMIC ?>template_post.php?BOARD=<?php echo $_GET['BOARD'] ?>&THREAD=<?php echo $rss_thread[0]['GUID'] ?>&object=<?php echo $rss_thread[0]['TITLE'] ?>"> <b>Poster un nouveau message</b> </a>&nbsp;&lt;<br> </font> </center> <br>

<TABLE cellSpacing=1 cellPadding=5 width="90%" align=center bgColor=#523b85 border=0>
<TR><TD><FONT face=Verdana color=#ffffff size=-1> <CENTER><B>Thread</B></CENTER></FONT> </TD> </TR>
<?php 
while( list ($LINE_NB, $FIELD) = each($rss_thread) ) 
{  echo "<tr> <td bgColor=#d8d8d8 height=20> <FONT face=Verdana color=#000000 size=-1>";
   echo "     <B>titre: {$rss_thread[$LINE_NB]['TITLE']}</B>&nbsp;by ";
   echo "     <A HREF=\"mailto:{$rss_thread[$LINE_NB]['AUTHOR']}?subject=[Board]%20{$rss_thread[$LINE_NB]['TITLE']}\">{$rss_thread[$LINE_NB]['AUTHOR']}</a></FONT>";
   echo "     <FONT face=Verdana color=#000000 size=-2>Date :        {$rss_thread[$LINE_NB]['PUBDATE']    }</FONT> </TD> </TR>\n";
   echo "<TR><TD bgColor=#e8e8e8><FONT face=Times color=#000000>".CSV_text_decode($rss_thread[$LINE_NB]['DESCRIPTION'])."</FONT> </TD> </TR>\n";
}
?>
</table>

</body>
</html>
