<?php
require_once("filenames.php");

if(!empty($rss_board)) $DYNAMIC="../"; //html generated link,$rss_board array allready created
else
{  $_GET['BOARD']="test";
   $DYNAMIC="";
   include_once "../../../modules/rss.inc";
   $rss_board=rss_read_array("{$_GET['BOARD']}/$RSS_INDEX");
}

$TEMPLATE_VAR=array("%CSS-FILE%"   => "{$DYNAMIC}{$TPL_CSS}",
                    "%PHP_ENGINE%" => "{$DYNAMIC}{$PHP_ENGINE}?BOARD={$_GET['BOARD']}".(empty($_GET['THREAD'])?"":"&THREAD={$_GET['THREAD']}"),
                    "%OBJECT%"     => empty($_GET['THREAD'])?"":"Re: ".htmlspecialchars($_GET['OBJECT'])
                   );
echo str_replace( array_keys($TEMPLATE_VAR), $TEMPLATE_VAR, implode("", file("{$DYNAMIC}{$TPL_POST}")) );
?>
