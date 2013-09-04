<?php global $rss_board, $SECTION;

if(empty($rss_board))
{  $DYNAMIC=""; include_once "../../../modules/flat.inc";
   $rss_board=rss_read_array("{$_GET['BOARD']}/board.rss");
}
else $DYNAMIC="../";
?>
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
   <title>Liste des messages postés dans le forum de discussion</title>
   <style type="text/css">@import url('%SKIN_PATH%/template.css');</style>
   <link rel="alternate" type="application/rss+xml" title="ZeroQI news feed" href="$DYNAMICboard.rss">

   <meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"/>
   <meta name="description"        content="ZeroQI Web Portal">
   <meta name="copyright"          content="Copyright (c) 2004 Authir or Company">
   <meta name="author"             content="Author Name">

</head>
<body>

   <META HTTP-EQUIV="Pragma" CONTENT="no-cache">
   <META HTTP-EQUIV="Expires" CONTENT="-1">
   <style type="text/css">
	table { font: 1em  black      Verdana; background:#4E4379; width:90%; }
	th    { font-size:12px; font-weight:bold; font-family:Verdana; color:#FFFFFF; }
	td    { font-size:12px; font-weight:bold; font-family:Verdana; color:#000000; }
	th,td { padding:0.5em; }
   </style>
</head>
<body bgcolor="#F5F5ED" text="#000000" link="#0000EE" vlink="#800080">

<h3 align='center'> Forum de discussion </h3>
<div align='center' size='-1'> &gt; <a href='<?php echo $DYNAMIC ?>template_post.php?BOARD=<?php echo $_GET['BOARD']?>'> <b>Poster un nouveau message</b> </a> &lt; <br /> <br /> </div>

<table class='forum' align='center' cellspacing='1px'>
<tr><th width='*'> Sujets </th> <th width=15%> Auteur </th> <th width=10%> Reponses </th> <th width=30%> Dernier message </th> </tr>
<?php
next($rss_board); $COLOR = array( 1, "#e8e8e8", "#d8d8d8" );
while( list($LINE_NB,$FIELD)=each($rss_board) )
{  echo "<tr bgColor='{$COLOR[ $COLOR[0]=$COLOR[0]%(count($COLOR)-1)+1 ]}'>"; if( $rss_board[$LINE_NB]['CATEGORY'] >=5 ) $red="_red"; else $red="";
   echo "<td> <img src='/icons/small/dir.gif'> ";
   if(empty($DYNAMIC)) echo "<A href='rss_template_threads.php?BOARD={$_GET['BOARD']}&THREAD={$rss_board[$LINE_NB]['GUID']}'>{$rss_board[$LINE_NB]['TITLE']}</a> </td>";
   else                echo "<A href='{$rss_board[$LINE_NB]['GUID']}.htm'>{$rss_board[$LINE_NB]['TITLE']}</a> </td>";
   echo "<td align='center'> <a href=\"mailto:{$rss_board[$LINE_NB]['AUTHOR']}\">{$rss_board[$LINE_NB]['AUTHOR']}</a> </td>";
   echo "<td align='right' >".($rss_board[$LINE_NB]['CATEGORY']-1)."</td>";
   echo "<td align='left'  >{$rss_board[$LINE_NB]['PUBDATE']}</td> </tr>\n";
} 
?>
</table>

</body>
</html>
